// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Default_simulator.h $
// $Id: Default_simulator.h 32205 2006-07-05 08:01:31Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_SIMULATOR_BASE_H_
#define CGAL_KINETIC_SIMULATOR_BASE_H_
#include <CGAL/Kinetic/basic.h>
#include <CGAL/Kinetic/Heap_pointer_event_queue.h>
#include <vector>
#include <CGAL/Interval_arithmetic.h>
#include <CGAL/Kinetic/Ref_counted.h>
#include <CGAL/Kinetic/Multi_listener.h>

CGAL_KINETIC_BEGIN_NAMESPACE;

#ifdef CGAL_KINETIC_CHECK_EXPENSIVE
#define CGAL_KINETIC_SIMULATOR_AUDITING
//#warning auditing kds
#endif


//! The simulator
/*!  The simulator is responsible for maintaining the event queue and
  processing events in the proper order. It takes a PolynomialKernel
  and an EventQueue as arguments as well as a flag saying whether the
  computations are performed exactly or not. This flag affects how it
  treats the output of the solvers. The PolynomialKernel is used to
  generate solvers as well as evaluate some predicates on the
  functions being solved.

  The Simulator creates two types of notifications, one when time is
  reversed (DIRECTION_OF_TIME) and one when there is a time at which
  the kinetic data structures can efficient audit themselves
  (HAS_AUDIT_TIME). The notification is done through a standard
  interface describe in CGAL::Listener.

  The solvers defined by the PolynomialKernel should not be the
  CGAL::Polynomial::GSL_solver or CGAL::Polynomial::Linear_solver. Use
  CGAL::Kinetic::GSL_solver or CGAL::KDS::Linear_solver instead as they
  are more robust in the case of kinetic data structures.

  Even though time can be "reversed" the future is always with
  increasing time. When time is reversed the current_time() becomes
  the negative of the old current_time(). In some cases, this means
  processing some events (since there needs to be a value of time
  after the last event and before the next one in order to reverse).

  Auditing occurs once between each pair of disjoint events. It occurs
  at a time close to the midpoint between the two event times. It is
  triggered by the time being advanced beyond the midpoint between the
  two event times.

  The Root_enumerator type is not that defined by the
  Polynomial_kernel since the polynomial kernel needs to have a real
  solver which returns all roots, where as the Simulator solver can
  check various invariants and skip even roots and things. This is
  especially important when performing numeric computations.

  I cannot put the Instantaneous_kernel in here since that might
  depend on the static data structure being used.
*/
template <class Polynomial_kernel_t,
	  class Queue/*= CGAL::Kinetic::Heap_pointer_event_queue<typename Polynomial_kernel_t::Root> */>
class Default_simulator: public Ref_counted<Default_simulator<Polynomial_kernel_t,  Queue> >
{
protected:
  typedef Default_simulator<Polynomial_kernel_t, Queue> This;
  //typedef typename Polynomial_kernel_t::Function Poly;

  struct Listener_core
  {
    typedef typename This::Handle Notifier_handle;
    typedef enum {HAS_AUDIT_TIME, DIRECTION_OF_TIME}
      Notification_type;
  };

  typedef typename Queue::Priority Queue_time;

public:
  //typedef Kinetic_kernel_t Kinetic_kernel;
  //! The polynomial kernel.
  /*!
    I don't know that there is any actual use for this, so
    maybe it should not be exposed.
  */
  typedef Polynomial_kernel_t Function_kernel;

  //! The base class to extend if you want to recieve notifications of events
  /*!
    See CGAL::Listener for a description of the
    notification framework and
    CGAL/Kinetic/notification_helpers.h for some classes to
    aid in using notifications.
  */
  typedef Multi_listener<Listener_core> Listener;
  friend class  Multi_listener<Listener_core>;

  //! The solver.
  //typedef typename Function_kernel::Root_stack Root_stack;

  //! The current time.
  /*!  This is a Root from the Polynomial package and supports
    whatever operations they support. See
    CGAL::Polynomial::Simple_interval_root for an example.
  */
  typedef typename Function_kernel::Root Time;

  //! A key which uniquely identifies a scheduled event
  /*!  If an event is not actually schedule (for example if it occurs
    past the last time of interest), it is assigned a special
    Event_key called null_event().
  */
  //#ifdef NDEBUG
  typedef typename Queue::Key Event_key;
  /*#else
    typedef typename Queue::Key Queue_key;
    struct Event_key: public Queue_key {
    Event_key(){}
    Event_key(Queue_key k): Queue_key(k){}
    };
    #endif*/

  //! The basic number type
  typedef typename Function_kernel::NT NT;

  //! Create a simulator passing the start time and the end time.
  Default_simulator(const Time &start_time=Time(0.0),
            const Time &end_time= internal::infinity_or_max(Time()),
	    Function_kernel fk=Function_kernel()):queue_(start_time, end_time, fk, 100),
				cur_time_(start_time),
				last_event_time_(-internal::infinity_or_max(Time())),
				mp_(fk.rational_between_roots_object()),
				ir_(fk.is_rational_object()),
				tr_(fk.to_rational_object()),
				is_forward_(true) {
    number_of_events_=0;
#ifdef CGAL_KINETIC_SIMULATOR_AUDITING
    // make it less than the current time.
    //std::pair<double, double> ival= to_interval(cur_time_);
    //audit_time_=NT(ival.first-10.0);
    audit_time_= -internal::infinity_or_max(NT());
#endif
  };

  //! Return the current time
  /*!  If an event is currently being processed, then this is the
    failure time of that event. Otherwise it is between the last event
    which was processed and the next event to be processed.
  */
  Time current_time() const
  {
    return cur_time_;
  }

  //! Set the current time to t
  /*!
    t must be after (or equal to) current_time(). Any events
    between current_time() and t are processed in the proper
    order.
  */
  void set_current_time(const Time &t) {
    CGAL_precondition(t >=cur_time_);
    /*#ifdef CGAL_KINETIC_CHECK_EXPENSIVE
    if (current_time() < audit_time_ && t >= audit_time_) {
      audit_all_kdss();
    }
    #endif*/
    while (next_event_time() < t && !queue_.empty()) {
      process_next_event();
      /*#ifdef CGAL_KINETIC_CHECK_EXPENSIVE
      if (current_time() < audit_time_ && t >= audit_time_) {
	audit_all_kdss();
      }
      cur_time_=audit_time_;
      #endif*/
    }
    cur_time_=std::min(t, end_time());
  }

  //! Not clear if I want two methods
  void set_back_current_time(const Time &t) {
    CGAL_precondition(t <=cur_time_);
    cur_time_=t;
  }

  //! Return a rational number for current time.
  /*!  This returns a ration number representing the current
    time. This only returns a valid time if
    has_rational_current_time() is true.
  */
  NT rational_current_time() const
  {

    //CGAL_exactness_precondition_code(CGAL::Interval_nt_advanced ub= CGAL::to_interval(next_event_time()));
    //CGAL_exactness_precondition(ub.inf() > lb.sup());
    //	NT ret;
    if (ir_(cur_time_) && cur_time_ != std::numeric_limits<Time>::infinity()) {

    //if (ir_(cur_time_)) {
      /*CGAL_KINETIC_ERROR( "rational_current_time() called at time "
	<< current_time() << " which is not rational.\n");*/
      return tr_(cur_time_);
    }
    else {
      double ub= to_interval(current_time()).second;
      if (Time(ub) < next_event_time()) {
	return NT(ub);
      }
      else {
	//typename Function_kernel::Rational_between_roots bet= kernel_.rational_between_roots_object();
	return mp_(last_event_time(), next_event_time());
      }
    }

    //}
  }

  //! Return true if the current time is a rational number.
  /*!
   */
  bool has_rational_current_time() const
  {
    return last_event_time() < next_event_time();
  }

  //! Returns true if the current time is a rational number and there are no events at the current time
  /*!
    precondition that has_rational_current_time().
  */
  bool has_audit_time() const
  {
    CGAL_precondition(has_rational_current_time());
    return last_event_time() < next_event_time();
  }

  const NT& audit_time() const
  {
    CGAL_precondition(has_audit_time());
#ifdef CGAL_KINETIC_SIMULATOR_AUDITING
    return audit_time_;
#else
    CGAL_precondition(0);
    static NT z(0);
    return z;
#endif
  }

  //! The time of the next event
  /*!
    Or end_time() if the queue is empty.
  */
  Time next_event_time() const
  {
    if (queue_.empty()) return end_time();
    else return Time(queue_.front_priority());
  }

  //! The last time of interest
  /*!
    If time is running backwards, then this returns Time::infinity()
  */
  Time end_time() const
  {
    if (is_forward_) {
      return queue_.end_priority();
    }
    else {
      return std::numeric_limits<Time>::infinity();
    }
  }

  //! Set the last time to track events for
  /*!
    The current_time() must equal the end_time() in order
    to ensure that no events were missed.
  */
  void set_end_time(const Time &t) {
    CGAL_precondition(cur_time_==end_time());
    //end_time_= t;
    queue_.set_end_priority(t);
  }

  //! Create a new event and return its key
  /*!  The only interaction with the actual event is through the
    EventQueue template paramenter. See CGAL::Kinetic::Pointer_event_queue
    for the requirements of the default event queue. If the event's
    failure time is past end_time() then a key equal to null_event()
    is returned.
  */
  template <class E>
  Event_key new_event(const Time &t, const E& cert) {
    //CGAL_exactness_precondition(!(t < current_time()));
    //if (cert.time() == Time::infinity()) return final_event();

    if (!( t < end_time())){
      return null_event();
    } /*else {*/
    //CGAL_assertion(cert.time() < end_time());
    //if (0) cert.process();
    //std::cout << "Requested to schedule "; cert->write(std::cout);
    //std::cout<< " at time " << t << std::endl;
    Event_key key= queue_.insert(t, cert);
    //write_eventqueue(log().stream(Log::DEBUG));
    //log()->stream(Log::SOME) << key;
    //log()->stream(Log::SOME)

    CGAL_KINETIC_LOG(LOG_SOME, "Created event " << key << std::endl);
    //if (log()->is_output(Log::LOTS)) write(log()->stream(Log::LOTS));

    return key;
    //}
  }

 
  //! The key corresponding to events which never occur.
  /*!  Events can never occur if their time is past the end time or
    their certificate function has no roots.
  */
  Event_key null_event() const
  {
    return queue_.end_key();
  }
  /*const Event_key& final_event() const {
    return queue_.final_event();
    }*/

  //! Register an event to be processed after all events.
  /*!  I am not sure if I use this capability, so it may be droppable
   */
  template <class E>
  void new_final_event(const E &cert) {
    bool do_I_use_this;
    // this should be in the simulator anyway,
    return queue_.set_final_event(cert);
  }

  //! Delete the event with key k
  /*!
    This is just passed to the event queue.
  */
  void delete_event(const Event_key &k) {
    CGAL_KINETIC_LOG(LOG_SOME, "Deleting event " << k << std::endl);
    //if (k== final_event()) return;
    //#ifdef NDEBUG
    if (!k.is_valid()) {
      return;
    }
    //#endif
    queue_.erase(k);
    //CGAL_KINETIC_LOG(LOG_LOTS , *this);
  }

  //! The type for a handle to an event.
  /*template <class E>
    struct Event_handle: public Queue::template Event_info<E>::Handle {
    Event_handle(typename Queue::template Event_info<E>::Pointer p): Queue::template Event_info<E>::Handle(p) {
    }
    Event_handle(){}
    };*/

  //! Access an event
  /*!  The type of the returned pointer is an Event_pointer<E> where E
    is the template argument. I recommend just fetching the event each
    time you need it, rather than trying to store the pointer
    type. This method is effectively free (it is just a cast in the
    default implementation).

    You cannot change the event using this pointer, it is simply for
    recovering cached data (like Solvers).

    If you need to store it, wrap the return value in an Event_handle
    to get reference counting, otherwise the event might be processed
    and the object for it go away.

    The second argument really shouldn't be there, but gcc seems to
    have issues sometimes if you try to specify the template value
    directly.
  */
  /*template <class Ev>
    typename Queue::template Event_pointer<Ev>::Pointer event(const Event_key &k, const Ev& e) const {
    CGAL_KINETIC_LOG(LOG_LOTS, "Accessing event for key " << k << std::endl);
    return queue_.event(k,e);
    }*/

  template <class Event_type>
  const Event_type& event(const Event_key &k) const
  {
    CGAL_precondition(k.is_valid());
    CGAL_precondition(k != null_event());
    CGAL_KINETIC_LOG(LOG_LOTS, "Accessing event for key " << k << std::endl);
    return queue_.template get<Event_type>(k);
  }

  /*template <class Event_type>
    const Event_type& event(const Event_key &k, const Event_type&) const {
    CGAL_KINETIC_LOG(LOG_LOTS, "Accessing event for key " << k << std::endl);
    return queue_.template get<Event_type>(k);
    }*/

  //! Access the time of an event
  const Time &event_time(const Event_key &k) const
  {
    return queue_.priority(k);
  }

  //! Change an event to have a new object
  /*!
   */
  template <class Ev>
  Event_key set_event(const Event_key &k, const Ev& ev) {
    CGAL_KINETIC_LOG(LOG_SOME, "Changed event " << k << std::flush);
    Event_key rk= queue_.set(k, ev);
    CGAL_KINETIC_LOG(LOG_SOME, " to event " << rk << std::endl);
    CGAL_KINETIC_LOG(LOG_LOTS, *this);
    return rk;
  }

  //! Set which direction time is running
  /*!  Set it to CGAL::NEGATIVE to make time run backwards,
    CGAL::POSITIVE to make it run forwards. CGAL::ZERO is an error.
  */
  void set_direction_of_time(Sign sn);

  //! Return the direction of time.
  CGAL::Sign direction_of_time() const
  {
    if (is_forward_) return CGAL::POSITIVE;
    else return CGAL::NEGATIVE;
  }

  //! Return the number of events which has been processed.
  unsigned int current_event_number() const
  {
    return number_of_events_;
  }
  //! Process all events up to event i
  /*!
    i must be greater than current_event_number().
  */
  void set_current_event_number(unsigned int i) {
    while (!queue_.empty() && number_of_events_ < i) {
      /*#ifdef CGAL_KINETIC_CHECK_EXPENSIVE
      if (current_time() < audit_time_ && next_event_time() >= audit_time_) {
	audit_all_kdss();
      }
      #endif*/
      process_next_event();
    }
  }

  void write(std::ostream &out) const
  {
    out << queue_ << std::endl;
  }

  void print() const
  {
    write(std::cout);
  }

protected:

  //! Return a time between the current and the next event to use for validating
  /*!  This is a time between the last certificate failure time and
    the next.  Return true if there is such a time.
  */
  //NT compute_free_time() const;

  //! Process the next event
  void process_next_event() {
    CGAL_KINETIC_LOG(LOG_LOTS, *this);
    /*#ifndef NDEBUG
      if (cur_time_ < audit_time_){
      audit_all_kdss();
      }
      #endif*/

    CGAL_exactness_precondition(next_event_time() >= last_event_time_);
    if (next_event_time() > cur_time_) {
      cur_time_= next_event_time();
      last_event_time_= cur_time_;
    }
    //_last_event_time=cur_time_;
    //if (queue_.front_priority() < end_time()) {
    queue_.process_front();
    /*for (typename KDSs::iterator it= kdss_.begin(); it != kdss_.end(); ++it){
      (*it)->write(std::cout);
      }*/

    ++number_of_events_;
#ifdef CGAL_KINETIC_SIMULATOR_AUDITING
    if (cur_time_ != next_event_time()) {
      //typename Function_kernel::Rational_between_roots bet= kernel_.rational_between_roots_object();
      audit_time_= mp_(cur_time_, next_event_time());
      CGAL_KINETIC_LOG(LOG_SOME, "Next audit is at time " << audit_time_ << std::endl);
      //if (current_time() < audit_time_ && t >= audit_time_) {
	audit_all_kdss();
	//}
    } else {
      CGAL_KINETIC_LOG(LOG_SOME, "Can't audit between events.\n");
    }
#endif
    /*} else {
      queue_.clear();
      }*/

    //std::cout << queue_<< std::endl;
    CGAL_expensive_postcondition_code(if (cur_time_== next_event_time()))
      CGAL_expensive_postcondition_code(std::cerr << "Warning: two concurrent events.\n");
    //CGAL_expensive_postcondition_code(validate_all_kinetic_data_structures());
    CGAL_KINETIC_LOG(LOG_LOTS, *this);

  }

  void audit_all_kdss() ;

  const Time &last_event_time() const
  {
    return last_event_time_;
  }

private:
  //! add a kds to the simulator
  /*!
    Note, these use Prof. Cheritons naming convention (sort of).
  */
  void new_listener(Listener *sk) {
    //std::cout << "new sim listener.\n";
#ifndef NDEBUG
    for (unsigned int i=0; i< kdss_.size(); ++i){
      CGAL_precondition(kdss_[i] != sk);
    }
#endif
    kdss_.push_back(sk);
  }

  //! Remove a kds
  void delete_listener(Listener *kds) {
    //std::cout << "delete sim listener.\n";
    //kdss_.erase(kds);
    for (unsigned int i=0; i< kdss_.size(); ++i){
      if (kdss_[i] == kds) {
	std::swap(kdss_[i], kdss_.back());
	kdss_.pop_back();
	return;
      }
    }
  }

protected:
  Queue queue_;
  std::vector<Listener*> kdss_;
  Time cur_time_,  last_event_time_;
  unsigned int number_of_events_;
  typename Function_kernel::Rational_between_roots mp_;
  typename Function_kernel::Is_rational ir_;
  typename Function_kernel::To_rational tr_;
  bool is_forward_;
#ifdef CGAL_KINETIC_SIMULATOR_AUDITING
  NT audit_time_;
#endif
};

/*template <class S, bool E, class PQ>
  typename Default_simulator<S, E, PQ>::NT Default_simulator<S, E, PQ>::compute_free_time() const {
  if (!(last_time_ < next_event_time())) {
  std::cerr << "No time between " << current_time()  << " and " << next_event_time() << std::endl;
  return current_time_nt()- CGAL::abs(current_time_nt());
  } else {
  if (current_time() != last_time_){
  double dt= CGAL::to_double(current_time());
  NT ntdt(dt);
  Time tdt(ntdt);
  if (tdt  > last_time_ && tdt < next_event_time()) return NT(dt);
  }

  return mp_(current_time(), next_event_time());
  }
  }*/

template <class S, class PQ>
void Default_simulator<S, PQ>::set_direction_of_time(CGAL::Sign dir)
{
  if (dir != direction_of_time()) {
    while (next_event_time() == current_time()) {
      std::cerr << "Processing event in order to reverse time.\n";
      process_next_event();
    }

    Time oct= cur_time_;
    NT tnt= NT(to_interval(cur_time_).second);// was CGAL::

    Time tdt(tnt);
    if (tdt == cur_time_) {
      tnt= tnt+ NT(.000001);
      tdt= Time(tnt);
    }

    if (tdt >= next_event_time()) {
      //typename Function_kernel::Rational_between_roots rbr= kernel_.rational_between_roots_object();
      tnt= mp_(cur_time_, next_event_time());
      tdt= Time(tnt);
    }

    cur_time_= Time(-tnt);

    is_forward_= !is_forward_;
    //end_time_=-end_time_;
    last_event_time_= -std::numeric_limits<Time>::infinity();
    CGAL_KINETIC_LOG(LOG_SOME, "Current time is " << cur_time_ << " and was " << oct
		 << ", end_time() is " << end_time() << std::endl);
    for (typename std::vector<Listener*>::iterator it= kdss_.begin(); it != kdss_.end(); ++it) {
      (*it)->new_notification(Listener::DIRECTION_OF_TIME);
      //std::cout << "called on something.\n";
    }
  }
  else {
    CGAL_KINETIC_LOG(LOG_SOME, dir << " " << end_time() << " " << cur_time_ << std::endl);
  }
}


template <class S, class PQ>
void Default_simulator<S, PQ>::audit_all_kdss()
{
#ifdef CGAL_KINETIC_CHECK_EXPENSIVE
  cur_time_= Time(audit_time_);
  CGAL_KINETIC_LOG(LOG_SOME, "Auditing KDSs at time " << rational_current_time() << std::endl);
  for (typename std::vector<Listener*>::iterator it= kdss_.begin(); it != kdss_.end(); ++it) {
    //CGAL_exactness_postcondition_code((*it)->new_notification(Listener::HAS_VERIFICATION_TIME));
    CGAL_postcondition_code((*it)->new_notification(Listener::HAS_AUDIT_TIME));
  }
#endif
}


template <class S, class PQ>
std::ostream &operator<<(std::ostream &out, const Default_simulator<S, PQ> &s)
{
  s.write(out);
  return out;
}


CGAL_KINETIC_END_NAMESPACE;
#endif
