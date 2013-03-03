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
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Kinetic_data_structures/include/CGAL/Kinetic/Simulation_traits.h $
// $Id: Simulation_traits.h 32205 2006-07-05 08:01:31Z drussel $
// 
//
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_SIMULATION_TRAITS_H
#define CGAL_KINETIC_SIMULATION_TRAITS_H
#include <CGAL/Kinetic/basic.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Filtered_kernel.h>
//#include <CGAL/Gmpq.h>
#include <CGAL/Kinetic/Active_objects_vector.h>
#include <CGAL/Kinetic/Cartesian_instantaneous_kernel.h>
#include <CGAL/Kinetic/Cartesian_kinetic_kernel.h>
#include <CGAL/Kinetic/Derivitive_filter_function_kernel.h>
#include <CGAL/Kinetic/Handle_degeneracy_function_kernel.h>
#include <CGAL/Kinetic/Default_simulator.h>
#include <CGAL/Kinetic/Two_list_pointer_event_queue.h>
#include <CGAL/Polynomial/Sturm_root_stack.h>
#include <CGAL/Polynomial/Sturm_root_stack_traits.h>
#include <CGAL/Polynomial/Kernel.h>
#include <CGAL/Polynomial/Numeric_root_stack.h>
#include <CGAL/Polynomial/Root_stack_default_traits.h>
#include <CGAL/Polynomial/Upper_bound_root_stack.h>
#include <CGAL/Polynomial/Upper_bound_root_stack_Descartes_traits.h>
//#include <CGAL/Polynomial/CORE_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Kinetic/Heap_pointer_event_queue.h>

CGAL_KINETIC_BEGIN_NAMESPACE

template <class StaticKernel,
	  class KineticKernel,
	  class SimulatorC>
struct Simulation_traits
{
public:

  typedef typename StaticKernel::FT NT;
  struct Static_kernel: public StaticKernel {};

  struct Kinetic_kernel: public KineticKernel {};

  typedef SimulatorC Simulator;

  typedef typename KineticKernel::Function_kernel Function_kernel;

  typedef typename KineticKernel::Function_kernel::Root Time;

  Simulation_traits(const Time &lb, const Time &ub): sp_(new Simulator(lb, ub, kk_.function_kernel_object())){}
  Simulation_traits(): sp_(new Simulator()) {
  }

  Simulator* simulator_handle(){ return sp_.get();}
  const Simulator* simulator_handle() const { return sp_.get();}
  Static_kernel& static_kernel_object(){return k_;}
  Kinetic_kernel& kinetic_kernel_object(){return kk_;}

  const Static_kernel& static_kernel_object() const {return k_;}
  const Kinetic_kernel& kinetic_kernel_object() const {return kk_;}
  const Function_kernel& function_kernel_object() const {return fk_;}

protected:
  Static_kernel k_;
  Kinetic_kernel kk_;
  Function_kernel fk_;
  typename Simulator::Handle sp_;
};

struct Suggested_exact_simulation_traits_types
{
  //typedef CGAL::Simple_cartesian<CGAL::Gmpq> Static_kernel;
  typedef CGAL::Exact_predicates_exact_constructions_kernel Static_kernel;
     typedef Static_kernel::FT NT;
     typedef CGAL::POLYNOMIAL::Polynomial<NT> Function;
  //typedef CGAL::POLYNOMIAL::Upper_bound_root_stack_Descartes_traits<Function> Root_stack_traits;
  //typedef CGAL::POLYNOMIAL::Upper_bound_root_stack<Root_stack_traits> Root_stack;
  typedef CGAL::POLYNOMIAL::Sturm_root_stack_traits<Function> Root_stack_traits;
    typedef CGAL::POLYNOMIAL::Sturm_root_stack<Root_stack_traits> Root_stack;
    typedef CGAL::POLYNOMIAL::Kernel<Function, Root_stack> Function_kernel;

  /*typedef CGAL::Simple_cartesian<CORE::Expr> Static_kernel;
  typedef Static_kernel::FT NT;
  typedef CGAL::POLYNOMIAL::CORE_kernel Function_kernel;*/

  struct Simulator_function_kernel: public CGAL::Kinetic::Handle_degeneracy_function_kernel<Function_kernel> {};
  typedef CGAL::Kinetic::Cartesian_kinetic_kernel<Simulator_function_kernel> Kinetic_kernel;
  typedef  Simulator_function_kernel::Root Time;
  typedef CGAL::Kinetic::Two_list_pointer_event_queue<Function_kernel> Queue_base;
  //typedef CGAL::Kinetic::Heap_pointer_event_queue<Function_kernel> Queue_base;

  struct Event_queue: public Queue_base
  {
    Event_queue(const Time &start, const Time &end, Function_kernel fk, int num): Queue_base(start, end, fk, num){}
  };

  typedef CGAL::Kinetic::Default_simulator<Simulator_function_kernel, Event_queue > Simulator;
};


struct Suggested_exact_simulation_traits_base: public Simulation_traits<Suggested_exact_simulation_traits_types::Static_kernel,
									Suggested_exact_simulation_traits_types::Kinetic_kernel,
									Suggested_exact_simulation_traits_types::Simulator> {
  typedef Simulation_traits<Suggested_exact_simulation_traits_types::Static_kernel,
			    Suggested_exact_simulation_traits_types::Kinetic_kernel,
			    Suggested_exact_simulation_traits_types::Simulator> P;
  Suggested_exact_simulation_traits_base(const P::Time &lb, const P::Time &ub): P(lb, ub) {}
  Suggested_exact_simulation_traits_base() {}
};

struct Suggested_inexact_simulation_traits_types
{
  typedef CGAL::Exact_predicates_inexact_constructions_kernel Static_kernel;
  typedef Static_kernel::FT NT;
  typedef CGAL::POLYNOMIAL::Polynomial<NT> Function;
  typedef CGAL::POLYNOMIAL::Root_stack_default_traits<Function> Root_stack_traits;
  typedef CGAL::POLYNOMIAL::Numeric_root_stack<Root_stack_traits> Root_stack;
  typedef CGAL::POLYNOMIAL::Kernel<Function, Root_stack> Function_kernel;
  typedef CGAL::Kinetic::Derivitive_filter_function_kernel<Function_kernel> Simulator_function_kernel;
  typedef CGAL::Kinetic::Cartesian_kinetic_kernel<Simulator_function_kernel> Kinetic_kernel;
  typedef Simulator_function_kernel::Root Time;
  typedef CGAL::Kinetic::Heap_pointer_event_queue<Function_kernel> Queue_base;

  struct Event_queue: public Queue_base
  {
    Event_queue(const Time &start, const Time &finish, Function_kernel fk, int num): Queue_base(start, finish, fk, num){}
  };
  typedef CGAL::Kinetic::Default_simulator<Simulator_function_kernel, Event_queue > Simulator;
};

struct Suggested_inexact_simulation_traits_base: public Simulation_traits<Suggested_inexact_simulation_traits_types::Static_kernel,
									  Suggested_inexact_simulation_traits_types::Kinetic_kernel,
									  Suggested_inexact_simulation_traits_types::Simulator> {
  typedef Simulation_traits<Suggested_inexact_simulation_traits_types::Static_kernel,
			    Suggested_inexact_simulation_traits_types::Kinetic_kernel,
			    Suggested_inexact_simulation_traits_types::Simulator> P;
  Suggested_inexact_simulation_traits_base(const P::Time &lb, const P::Time &ub): P(lb, ub) {}
  Suggested_inexact_simulation_traits_base() {}
};


CGAL_KINETIC_END_NAMESPACE
#endif
