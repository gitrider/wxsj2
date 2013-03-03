////////////////////////////////////////////////////////////////////////////////
//
// Thread.cpp
//
// 
//
// Last Updated : 2005.03.22
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "stdafx.h"
#include "Thread.h"

uint g_QuitThreadCount = 0 ;
MyLock g_thread_lock ;

////////////////////////////////////////////////////////////////////////////////
//
// constructor
//
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
Thread::Thread ( ) 
{
__ENTER_FUNCTION

	m_TID		= 0 ;
	m_Status	= Thread::READY ;

#if defined(__WINDOWS__)
	m_hThread = NULL ;
#endif

__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
// destructor (virtual)
//
////////////////////////////////////////////////////////////////////////////////
Thread::~Thread () 
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
VOID Thread::start () 
{ 
__ENTER_FUNCTION
		
	if ( m_Status != Thread::READY )
		return ;

#if defined(__LINUX__)
	pthread_create( &m_TID, NULL , MyThreadProcess , this );
#elif defined(__WINDOWS__)
	m_hThread = ::CreateThread( NULL, 0, MyThreadProcess , this, 0, &m_TID ) ;
#endif

__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
VOID Thread::stop ()
{
	__ENTER_FUNCTION
		
		
	__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
VOID Thread::exit( VOID * retval )
{
	_MY_TRY
	{
		#if defined(__LINUX__)
			pthread_exit( retval );
		#elif defined(__WINDOWS__)
			::CloseHandle( m_hThread ) ;
		#endif
	}
	_MY_CATCH
	{

	}
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
#if defined(__LINUX__)
VOID * MyThreadProcess ( VOID * derivedThread )
{
__ENTER_FUNCTION

	Thread * thread = (Thread *)derivedThread;
	if( thread==NULL )
		return NULL;
	
	// set thread's status to "RUNNING"
	thread->setStatus(Thread::RUNNING);

	// here - polymorphism used. (derived::run() called.)
	thread->run();
	
	// set thread's status to "EXIT"
	thread->setStatus(Thread::EXIT);
	
	//INT ret = 0;
	//thread->exit(&ret);

	g_thread_lock.Lock() ;
	g_QuitThreadCount++ ;
	g_thread_lock.Unlock() ;

__LEAVE_FUNCTION

	return NULL;	// avoid compiler's warning
}
#elif defined (__WINDOWS__)
DWORD WINAPI MyThreadProcess(  VOID* derivedThread )
{
__ENTER_FUNCTION

	Thread * thread = (Thread *)derivedThread;
	if( thread==NULL )
		return 0;
	
	// set thread's status to "RUNNING"
	thread->setStatus(Thread::RUNNING);

	// here - polymorphism used. (derived::run() called.)
	thread->run();
	
	// set thread's status to "EXIT"
	thread->setStatus(Thread::EXIT);

	thread->exit(NULL);

	g_thread_lock.Lock() ;
	g_QuitThreadCount++ ;
	g_thread_lock.Unlock() ;

__LEAVE_FUNCTION

	return 0;	// avoid compiler's warning
}
#endif

VOID Thread::run( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

