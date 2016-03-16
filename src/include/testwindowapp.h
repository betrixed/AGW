/////////////////////////////////////////////////////////////////////////////
// Name:        testwindowapp.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     31/10/2013 06:54:39
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _TESTWINDOWAPP_H_
#define _TESTWINDOWAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "mainframe.h"
////@end includes


#include "database.hpp"
/*!
 * Forward declarations
 */

class MyThread;
WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);


////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * TestWindowApp class declaration
 */

class TestWindowApp: public wxApp
{
    DECLARE_CLASS( TestWindowApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    TestWindowApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin TestWindowApp event handler declarations

	/// wxEVT_IDLE event handler for ID_UNIDENTIFIED
	void OnIdle( wxIdleEvent& event );

////@end TestWindowApp event handler declarations

////@begin TestWindowApp member function declarations

////@end TestWindowApp member function declarations

////@begin TestWindowApp member variables
////@end TestWindowApp member variables

    // This looks like a good central place to keep track of everything.

    AppData*  theStore;

    // copied from samples/thread
    // critical section
    wxCriticalSection m_critsect;

    // all the threads currently alive -

    wxArrayThread m_threads;

    // semaphore used to wait for the threads to exit, see MyFrame::OnQuit()
    wxSemaphore m_semAllDone;

    // indicates that we're shutting down and all threads should exit
    bool m_shuttingDown;

    bool m_firstIdle;

    wxWindow* GetSplash(bool timeout);

};

/*!
 * Application instance declaration
 */

////@begin declare app
DECLARE_APP(TestWindowApp)
////@end declare app

#endif
    // _TESTWINDOWAPP_H_
