#ifndef __UTIL_TEMPLATE_POINTER_H__
#define __UTIL_TEMPLATE_POINTER_H__

/*!	
	Author : dfhu
	Data: 2003-10
	Des: a smart ptr template
*/


namespace FI_UTIL
{

#pragma warning ( disable : 4390 )
	template <class Type> class ut_TemplatePointer
	{

	private:
		Type *pointerE;
	//! simply put ptrs to zero
		void zeroPtrs();
	//! resets pointer and deletes object if the are no more references 
		void deletePtrs();
		
	protected:


	public:
		ut_TemplatePointer();

	//! constructor, which takes a pointer as argument 
		ut_TemplatePointer(Type *itemA);
	//! constructor, which takes a reference to an instance as argument 
		ut_TemplatePointer(Type& itemA);
	//! copy constructor 
		ut_TemplatePointer(const ut_TemplatePointer<Type>& templatePointerA);
	//! destructor 
		virtual ~ut_TemplatePointer();
	//! assignment operator 
		ut_TemplatePointer<Type>& operator=(
							const ut_TemplatePointer<Type>&  templatePointerA);
	//! assigns ordinary pointer to \textit{this} pointer 
		ut_TemplatePointer<Type>& operator=(Type*               itemA);
	//! class member access operator 
		Type*                  operator->() const;
	//! dereference operator 
		Type&                  operator*()  const;
	//! equality operator 
		bool                   operator==(
						const ut_TemplatePointer<Type>&  templatePointerA)  const;
	//! equality operator with ordinary pointer 
		bool                   operator==(Type*           itemA)             const;
	//! unequality operator 
		bool                   operator!=(
						const ut_TemplatePointer<Type>&  templatePointerA)  const;
	//! unequality operator with ordinary pointer 
		bool                   operator!=(Type*  		  itemA)             const;
	//! sets pointer and updates reference counters of old and new object 


	//	operator bool () const	{		return pointer!=NULL;	}  ;

	public:
		Type* pointer() const;
		void  pointer(Type* pointerA);
	};

	/*!
	This member function sets the member \texttt{pointerE} to
	\texttt{NULL}. 
	*/

	template <class Type>
		inline void ut_TemplatePointer<Type>::zeroPtrs()
	{
		pointerE = 0;
	};

	// -------------------------------------------------------------------------- ;

	/*!
	If no more references exist, this member function deletes the
	instance \textit{this} pointer points to. Then \textit{this} pointer
	is set to \texttt{NULL}. 
	*/

	template <class Type>
		inline void ut_TemplatePointer<Type>::deletePtrs()
	{
		if (pointerE != 0)
		{
		//	if (--(pointerE->m_nRef) <= 0)
		//		delete pointerE;
			if( pointerE->ReleaseRef() <= 0 )
				delete pointerE;
		};
		zeroPtrs();
	};

	// -------------------------------------------------------------------------- ;

	/*!
	Inits the member \texttt{pointerE} with the special value
	\texttt{NULL}, in order to indicate that this object isn't pointing
	to any object. 
	*/

	template <class Type>
		inline ut_TemplatePointer<Type>::ut_TemplatePointer()
		:	pointerE(0)
	{
//		zeroPtrs(); done in the member initialization list!
	};

	// -------------------------------------------------------------------------- ;

	/*!
			Inits \textit{this} reference counting pointer by the passed
			ordinary pointer. If the pointer isn't \texttt{NULL} the
			reference counter of the instance pointed to will be incremented. 

	\param   itemA: ordinary pointer 

	*/

	template <class Type>
		inline ut_TemplatePointer<Type>::ut_TemplatePointer(
			Type *itemA)
		:	pointerE(itemA)
	{
		if (pointerE != 0)
			pointerE->AddRef();
	//		(pointerE->m_nRef)++;
	};

	// -------------------------------------------------------------------------- ;

	/*!
			Inits \textit{this} reference counting pointer by the passed
			reference of an object. The reference counter of that object
			to will be incremented. 

	\param   itemA: reference to an instance 

	*/

	template <class Type>
		inline ut_TemplatePointer<Type>::ut_TemplatePointer(
			Type& itemA)
		:	pointerE(&itemA)
	{
		if (pointerE != 0)
			pointerE->AddRef();
		//	(pointerE->m_nRef)++;
	};

	// -------------------------------------------------------------------------- ;

	/*!
			Lets the pointer of \textit{this} instance point to the same
			object (or \texttt{NULL}) as the pointer in the passed
			instance. If the pointer isn't \texttt{NULL} the reference
			counter of that object will be increased. 

	\param   templatePointerA: refernce to another reference counting pointer 

	*/

	template <class Type>
		inline ut_TemplatePointer<Type>::ut_TemplatePointer(
			const ut_TemplatePointer<Type>& templatePointerA)
		:	pointerE(templatePointerA.pointerE)
	{
		if (pointerE != 0)
			pointerE->AddRef();
		//	(pointerE->m_nRef)++;
	};

	// -------------------------------------------------------------------------- ;

	/*!
	Ensures to decrease the reference counter of the object pointed to
	by \textit{this} pointer if not \texttt{NULL}. Deletes the object if
	this was the last reference, before deleting \texttt{this} pointer. 
	*/

	template <class Type>
	#if !defined(__GNUC__) && defined(__sgi)
		inline
	#endif
		ut_TemplatePointer<Type>::~ut_TemplatePointer()
	{
		deletePtrs();
	};
	 
	// -------------------------------------------------------------------------- ;

	/*!
			Decreases the reference counter of the object \textit{this}
			pointer was pointing to if it isn't \texttt{NULL}. Deletes
			that object if the reference counter became zero. Then this
			method assigns the passed pointer to \textit{this} pointer.
			If the passed pointer refers to an object the reference
			counter of that object will be increased. 

	\param   templatePointerA: refernce to another reference counting pointer 

	\return  reference to \textit{this} class 

	*/

	template <class Type>
		inline ut_TemplatePointer<Type>& ut_TemplatePointer<Type>::operator=(
			const ut_TemplatePointer<Type>& templatePointerA)
	{
		deletePtrs();
		
		pointerE = templatePointerA.pointerE;
		
		if (pointerE != 0)
			pointerE->AddRef();
		//	(pointerE->m_nRef)++;

		return *this;
	};

	// -------------------------------------------------------------------------- ;

	/*!
			This member function is a special assignment operator: It
			takes an ordinary pointer as argument, which is copied to
			\textit{this} reference counting pointer. Before changing an
			old non-zero value of \textit{this} pointer it makes sure to
			decrease the reference counter of the object pointed and
			deleting the object if the counter became zero. If the new
			pointer value isn't \texttt{NULL}, the reference counter
			will be increased for the object \textit{this} pointer refers now. 

	\param   itemA: ordinary pointer 

	\return  reference to \textit{this} class 

	*/

	template <class Type>
		inline ut_TemplatePointer<Type>& ut_TemplatePointer<Type>::operator=(
				Type* itemA)
	{
		deletePtrs();
		
		pointerE = itemA;
		
		if (pointerE != 0)
			pointerE->AddRef();
		//	(pointerE->m_nRef)++;

		return *this;
	};

	// -------------------------------------------------------------------------- ;

	/*!
			Gives access to a named member of the class pointed to by
			\textit{this} pointer. If the pointer is \texttt{NULL} the
			function \texttt{error} will be called. 

	\return  a pointer to the instance for which the specified member
			will be accessed 

	*/

	template <class Type>
		inline Type* ut_TemplatePointer<Type>::operator->() const
	{
        return pointerE;
	};


	template <class Type>
		inline Type& ut_TemplatePointer<Type>::operator*() const
	{
	   return (*pointerE);
	};

	template <class Type>
		inline bool ut_TemplatePointer<Type>::operator==(
			const ut_TemplatePointer<Type>& templatePointerA) const
	{
		return (pointerE == templatePointerA.pointerE);
	};

	template <class Type>
		inline bool ut_TemplatePointer<Type>::operator==(
			Type* itemA) const
	{
		return (pointerE == itemA);
	};


	template <class Type>
		inline bool ut_TemplatePointer<Type>::operator!=(
			const ut_TemplatePointer<Type>& templatePointerA) const
	{
		return (pointerE != templatePointerA.pointerE);
	};


	template <class Type>
		inline bool ut_TemplatePointer<Type>::operator!=(
			Type* itemA) const
	{
		return (pointerE != itemA);
	};

	// -------------------------------------------------------------------------- ;

	
	template <class Type>
		inline Type* ut_TemplatePointer<Type>::pointer() const
	{
		return pointerE;
	};

	// -------------------------------------------------------------------------- ;


	template <class Type>
		inline void ut_TemplatePointer<Type>::pointer(
			Type* pointerA)
	{
		deletePtrs();
		
		pointerE = pointerA;
		
		if (pointerE != 0)
			pointerE->AddRef();
		//	(pointerE->m_nRef)++;
	};



	class ut_BaseTemplateRep
	{
	protected:
		long		m_nRef;
	
	public:
		ut_BaseTemplateRep()
			: m_nRef(0){}

		void	AddRef() {++m_nRef;}
		int	ReleaseRef()	{--m_nRef;return m_nRef;}
		
		int		GetRef()	{return m_nRef;}

		virtual ~ut_BaseTemplateRep(){}
	};

	class ut_VBaseTmplateRep
	{
	protected:
		long		m_nRef;

	public:
		ut_VBaseTmplateRep() : m_nRef(0){}

		virtual ~ut_VBaseTmplateRep() {}

		virtual void	AddRef() {++m_nRef;}
		virtual int	ReleaseRef()	{--m_nRef;return m_nRef;}
		int		Release()	{ if( (--m_nRef) < 1 )  delete this;}
		int		GetRef()	{return m_nRef;}
	};

	class ut_NoWrapRep
	{
		protected:
			long		m_nRef;

		public:
			ut_NoWrapRep() : m_nRef(1){}

			virtual ~ut_NoWrapRep() {}

			void	AddRef()		{++m_nRef;}
			int		ReleaseRef()	{--m_nRef;return m_nRef;}
			virtual void		Release()		{ if( (--m_nRef) < 1 )  delete this;}
			int		GetRef()		{return m_nRef;}
	};

	template < class T >
		class ut_sharePtrWrap : public T
	{
	public:
		long	m_nRef;
	public:
		ut_sharePtrWrap()
			: m_nRef(0)
			, T(){}

		~ut_sharePtrWrap(){}

		void	AddRef() {++m_nRef;}
		int	ReleaseRef()	{--m_nRef;return m_nRef;}
	};


	template< typename T >	
		class	ut_PtrWrap
		{
		public:
			T *mp_Ptr;
			ut_PtrWrap( T *p )
			{
				mp_Ptr = p;
			}

			T* GetPointer()
			{
				return mp_Ptr;
			}
		};

	template < class T >
		class FI_SharedPtr
	{
		template < class T >
			struct __FI_WrapPtr
			{
				T *	_FI_Value;
				int	_FI_Ref;

				void AddRef()
				{
					++_FI_Ref;
				}

				void ReleaseRef()
				{
					--_FI_Ref;
					if( _FI_Ref < 1 )
					{
						delete this;
					}
				}

			protected:
				~__FI_WrapPtr()
				{
					delete _FI_Value;
				}

			public:
				__FI_WrapPtr()
				{
					_FI_Value = 0;
					_FI_Ref = 0;
				}
			};

	public:
		__FI_WrapPtr<T>	*m_FI_pValue;
		FI_SharedPtr( T *p )
		{
			m_FI_pValue = new __FI_WrapPtr;
			if( p!= NULL )
			{
				m_FI_pValue->_FI_Ref = 1;
				m_FI_pValue->_FI_Value = p;
			}
			else
			{
				m_FI_pValue->_FI_Ref = 0;
				m_FI_pValue->_FI_Value = NULL;
			}
		}

		FI_SharedPtr &operator = ( FI_SharedPtr<T> &other )
		{
			if( m_FI_pValue )
			{
				m_FI_pValue->ReleaseRef();
			}
			m_FI_pValue = other.m_FI_pValue;
			m_RFI_pValue->Addef();
		}

		T * operator  -> ()
		{
			return m_FI_pValue->_FI_Value;
		}

		bool operator ! ()
		{
			return m_FI_pValue->_FI_Value == NULL;
		}
	};
	#pragma warning ( default : 4390 )
}

#endif
