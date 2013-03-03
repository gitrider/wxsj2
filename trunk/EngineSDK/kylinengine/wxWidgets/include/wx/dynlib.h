/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dynlib.h
// Purpose:     Dynamic library loading classes
// Author:      Guilhem Lavaux, Vadim Zeitlin, Vaclav Slavik
// Modified by:
// Created:     20/07/98
// RCS-ID:      $Id: dynlib.h,v 1.53 2005/05/31 09:18:15 JS Exp $
// Copyright:   (c) 1998 Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DYNLIB_H__
#define _WX_DYNLIB_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#   pragma interface "dynlib.h"
#endif

#include "wx/defs.h"

#if wxUSE_DYNLIB_CLASS

#include "wx/string.h"
#include "wx/dynarray.h"

// FIXME: can this go in private.h or something too??
#if defined(__WXPM__) || defined(__EMX__)
#define INCL_DOS
#include <os2.h>
#endif

#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

#if defined(HAVE_DLERROR) && !defined(__EMX__)
    #define wxHAVE_DYNLIB_ERROR
#endif

class WXDLLIMPEXP_BASE wxDynamicLibraryDetailsCreator;

// ----------------------------------------------------------------------------
// conditional compilation
// ----------------------------------------------------------------------------

// Note: WXPM/EMX has to be tested first, since we want to use
// native version, even if configure detected presence of DLOPEN.
#if defined(__WXPM__) || defined(__EMX__) || defined(__WINDOWS__)
    typedef HMODULE             wxDllType;
#elif defined(HAVE_DLOPEN)
    #include <dlfcn.h>
    typedef void               *wxDllType;
#elif defined(HAVE_SHL_LOAD)
    #include <dl.h>
    typedef shl_t               wxDllType;
#elif defined(__DARWIN__)
    typedef void               *wxDllType;
#elif defined(__WXMAC__)
    #include <CodeFragments.h>
    typedef CFragConnectionID   wxDllType;
#else
    #error "Dynamic Loading classes can't be compiled on this platform, sorry."
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

enum wxDLFlags
{
    wxDL_LAZY       = 0x00000001,   // resolve undefined symbols at first use
                                    // (only works on some Unix versions)
    wxDL_NOW        = 0x00000002,   // resolve undefined symbols on load
                                    // (default, always the case under Win32)
    wxDL_GLOBAL     = 0x00000004,   // export extern symbols to subsequently
                                    // loaded libs.
    wxDL_VERBATIM   = 0x00000008,   // attempt to load the supplied library
                                    // name without appending the usual dll
                                    // filename extension.
    wxDL_NOSHARE    = 0x00000010,   // load new DLL, don't reuse already loaded
                                    // (only for wxPluginManager)

    wxDL_DEFAULT    = wxDL_NOW      // default flags correspond to Win32
};

enum wxDynamicLibraryCategory
{
    wxDL_LIBRARY,       // standard library
    wxDL_MODULE         // loadable module/plugin
};

enum wxPluginCategory
{
    wxDL_PLUGIN_GUI,    // plugin that uses GUI classes
    wxDL_PLUGIN_BASE    // wxBase-only plugin
};

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// when loading a function from a DLL you always have to cast the returned
// "void *" pointer to the correct type and, even more annoyingly, you have to
// repeat this type twice if you want to declare and define a function pointer
// all in one line
//
// this macro makes this slightly less painful by allowing you to specify the
// type only once, as the first parameter, and creating a variable of this type
// called "pfn<name>" initialized with the "name" from the "dynlib"
#define wxDYNLIB_FUNCTION(type, name, dynlib) \
    type pfn ## name = (type)(dynlib).GetSymbol(_T(#name))

// ----------------------------------------------------------------------------
// wxDynamicLibraryDetails: contains details about a loaded wxDynamicLibrary
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxDynamicLibraryDetails
{
public:
    // ctor, normally never used as these objects are only created by
    // wxDynamicLibrary::ListLoaded()
    wxDynamicLibraryDetails() { m_address = NULL; m_length = 0; }

    // get the (base) name
    wxString GetName() const { return m_name; }

    // get the full path of this object
    wxString GetPath() const { return m_path; }

    // get the load address and the extent, return true if this information is
    // available
    bool GetAddress(void **addr, size_t *len) const
    {
        if ( !m_address )
            return false;

        if ( addr )
            *addr = m_address;
        if ( len )
            *len = m_length;

        return true;
    }

    // return the version of the DLL (may be empty if no version info)
    wxString GetVersion() const
    {
        return m_version;
    }

private:
    wxString m_name,
             m_path,
             m_version;

    void *m_address;
    size_t m_length;

    friend class wxDynamicLibraryDetailsCreator;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxDynamicLibraryDetails,
                                  wxDynamicLibraryDetailsArray,
                                  WXDLLIMPEXP_BASE);

// ----------------------------------------------------------------------------
// wxDynamicLibrary: represents a handle to a DLL/shared object
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxDynamicLibrary
{
public:
    // return a valid handle for the main program itself or NULL if back
    // linking is not supported by the current platform (e.g. Win32)
    static wxDllType         GetProgramHandle();

    // return the platform standard DLL extension (with leading dot)
    static const wxChar *GetDllExt() { return ms_dllext; }

    wxDynamicLibrary() : m_handle(0) { }
    wxDynamicLibrary(const wxString& libname, int flags = wxDL_DEFAULT)
        : m_handle(0)
    {
        Load(libname, flags);
    }

    // NOTE: this class is (deliberately) not virtual, do not attempt
    //       to use it polymorphically.
    ~wxDynamicLibrary() { Unload(); }

    // return true if the library was loaded successfully
    bool IsLoaded() const { return m_handle != 0; }

    // load the library with the given name (full or not), return true if ok
    bool Load(const wxString& libname, int flags = wxDL_DEFAULT);

    // raw function for loading dynamic libs: always behaves as if
    // wxDL_VERBATIM were specified and doesn't log error message if the
    // library couldn't be loaded but simply returns NULL
    static wxDllType RawLoad(const wxString& libname, int flags = wxDL_DEFAULT);

    // detach the library object from its handle, i.e. prevent the object from
    // unloading the library in its dtor -- the caller is now responsible for
    // doing this
    wxDllType Detach() { wxDllType h = m_handle; m_handle = 0; return h; }

    // unload the given library handle (presumably returned by Detach() before)
    static void Unload(wxDllType handle);

    // unload the library, also done automatically in dtor
    void Unload() { if ( IsLoaded() ) { Unload(m_handle); m_handle = 0; } }

    // Return the raw handle from dlopen and friends.
    wxDllType GetLibHandle() const { return m_handle; }

    // check if the given symbol is present in the library, useful to verify if
    // a loadable module is our plugin, for example, without provoking error
    // messages from GetSymbol()
    bool HasSymbol(const wxString& name) const
    {
        bool ok;
        DoGetSymbol(name, &ok);
        return ok;
    }

    // resolve a symbol in a loaded DLL, such as a variable or function name.
    // 'name' is the (possibly mangled) name of the symbol. (use extern "C" to
    // export unmangled names)
    //
    // Since it is perfectly valid for the returned symbol to actually be NULL,
    // that is not always indication of an error.  Pass and test the parameter
    // 'success' for a true indication of success or failure to load the
    // symbol.
    //
    // Returns a pointer to the symbol on success, or NULL if an error occurred
    // or the symbol wasn't found.
    void *GetSymbol(const wxString& name, bool *success = NULL) const;

    // low-level version of GetSymbol()
    static void *RawGetSymbol(wxDllType handle, const wxString& name);
    void *RawGetSymbol(const wxString& name) const
    {
#if defined (__WXPM__) || defined(__EMX__)
        return GetSymbol(name);
#else
        return RawGetSymbol(m_handle, name);
#endif
    }

    // return all modules/shared libraries in the address space of this process
    //
    // returns an empty array if not implemented or an error occurred
    static wxDynamicLibraryDetailsArray ListLoaded();

    // return platform-specific name of dynamic library with proper extension
    // and prefix (e.g. "foo.dll" on Windows or "libfoo.so" on Linux)
    static wxString CanonicalizeName(const wxString& name,
                                     wxDynamicLibraryCategory cat = wxDL_LIBRARY);

    // return name of wxWidgets plugin (adds compiler and version info
    // to the filename):
    static wxString
    CanonicalizePluginName(const wxString& name,
                           wxPluginCategory cat = wxDL_PLUGIN_GUI);

    // return plugin directory on platforms where it makes sense and empty
    // string on others:
    static wxString GetPluginsDirectory();


#if WXWIN_COMPATIBILITY_2_2
    operator bool() const { return IsLoaded(); }
#endif

protected:
    // common part of GetSymbol() and HasSymbol()
    void *DoGetSymbol(const wxString& name, bool *success = 0) const;

#ifdef wxHAVE_DYNLIB_ERROR
    // log the error after a dlxxx() function failure
    static void Error();
#endif // wxHAVE_DYNLIB_ERROR


    // platform specific shared lib suffix.
    static const wxChar *ms_dllext;

    // the handle to DLL or NULL
    wxDllType m_handle;

    // no copy ctor/assignment operators (or we'd try to unload the library
    // twice)
    DECLARE_NO_COPY_CLASS(wxDynamicLibrary)
};


// ----------------------------------------------------------------------------
// wxDllLoader: low level DLL functions, use wxDynamicLibrary in your code
// ----------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_2 && wxUSE_DYNAMIC_LOADER

/*
    wxDllLoader is a class providing an interface similar to unix's dlopen().
    It is used by wxDynamicLibrary wxLibrary and manages the actual loading of
    DLLs and the resolving of symbols in them. There are no instances of this
    class, it simply serves as a namespace for its static member functions.
*/
class WXDLLIMPEXP_BASE wxDllLoader
{
public:
    /*
      This function loads the shared library libname into memory.

      libname may be either the full path to the file or just the filename in
      which case the library is searched for in all standard locations
      (use GetDllExt() to construct the filename)

      if success pointer is not NULL, it will be filled with true if everything
      went ok and false otherwise
     */
    static wxDllType LoadLibrary(const wxString& name, bool *success = NULL);

    /*
      This function unloads the shared library previously loaded with
      LoadLibrary
     */
    static void UnloadLibrary(wxDllType dll);

    /*
       This function returns a valid handle for the main program
       itself or NULL if back linking is not supported by the current platform
       (e.g. Win32).
     */
    static wxDllType GetProgramHandle() { return wxDynamicLibrary::GetProgramHandle(); }

    /*
       This function resolves a symbol in a loaded DLL, such as a
       variable or function name.

       dllHandle Handle of the DLL, as returned by LoadDll().
       name Name of the symbol.

       Returns the pointer to the symbol or NULL on error.
     */
    static void *GetSymbol(wxDllType dllHandle, const wxString &name, bool *success = 0);

    // return the standard DLL extension (with leading dot) for this platform
    static wxString GetDllExt() { return wxDynamicLibrary::GetDllExt(); }

private:

    wxDllLoader();                    // forbid construction of objects
};


// ----------------------------------------------------------------------------
// wxLibrary
// ----------------------------------------------------------------------------

#include "wx/hash.h"

class WXDLLIMPEXP_BASE wxLibrary : public wxObject
{
public:
    wxLibrary(wxDllType handle);
    virtual ~wxLibrary();

    // Get a symbol from the dynamic library
    void *GetSymbol(const wxString& symbname);

    // Create the object whose classname is "name"
    wxObject *CreateObject(const wxString& name);

protected:
    void PrepareClasses(wxClassInfo *first);

    wxDllType m_handle;

public:
    wxHashTable classTable;
};

// ----------------------------------------------------------------------------
// wxLibraries
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxLibraries
{
public:
    wxLibraries();
    ~wxLibraries();

    // caller is responsible for deleting the returned pointer if !NULL
    wxLibrary *LoadLibrary(const wxString& basename);

    wxObject *CreateObject(const wxString& name);

protected:
    wxList m_loaded;
};

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

extern WXDLLIMPEXP_DATA_BASE(wxLibraries) wxTheLibraries;

#endif // WXWIN_COMPATIBILITY_2_2 && wxUSE_DYNAMIC_LOADER

// ----------------------------------------------------------------------------
// Interesting defines
// ----------------------------------------------------------------------------

#define WXDLL_ENTRY_FUNCTION() \
extern "C" WXEXPORT const wxClassInfo *wxGetClassFirst(); \
const wxClassInfo *wxGetClassFirst() { \
  return wxClassInfo::GetFirst(); \
}

#endif // wxUSE_DYNLIB_CLASS

#endif // _WX_DYNLIB_H__
