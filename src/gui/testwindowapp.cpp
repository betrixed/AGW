/////////////////////////////////////////////////////////////////////////////
// Name:        testwindowapp.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     31/10/2013 06:54:39
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "testwindowapp.h"
#include <wx/stdpaths.h>
////@begin XPM images
////@end XPM images

#include <wx/splash.h>
#include <wx/filename.h>

#include "appdata.h"
/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( TestWindowApp )
////@end implement app


/*
 * TestWindowApp type definition
 */

IMPLEMENT_CLASS( TestWindowApp, wxApp )


/*
 * TestWindowApp event table definition
 */

BEGIN_EVENT_TABLE( TestWindowApp, wxApp )

////@begin TestWindowApp event table entries
	EVT_IDLE( TestWindowApp::OnIdle )
////@end TestWindowApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for TestWindowApp
 */

TestWindowApp::TestWindowApp()
{
    Init();
}


/*
 * Member initialisation
 */

void TestWindowApp::Init()
{
////@begin TestWindowApp member initialisation
////@end TestWindowApp member initialisation

    m_firstIdle = true;
}

/*
 * Initialisation for TestWindowApp
 */

bool TestWindowApp::OnInit()
{
////@begin TestWindowApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	MainFrame* mainWindow = new MainFrame( NULL, NULL );
	mainWindow->Show(true);
////@end TestWindowApp initialisation




    //wxIdleEvent();

    theStore = new AppData();

    theStore->initPaths(this, mainWindow);

    mainWindow->Show(true);

    GetSplash(true);


    wxWakeUpIdle();
    return true;
}


/*
 * Cleanup for TestWindowApp
 */

int TestWindowApp::OnExit()
{
    theStore->exit();
////@begin TestWindowApp cleanup
	return wxApp::OnExit();
////@end TestWindowApp cleanup


}

wxWindow* TestWindowApp::GetSplash(bool timeout)
{
    wxBitmap bitmap;
    wxWindow* result;


    wxFileName splashPath;

    splashPath.Assign(theStore->appDataDir(),_T("ClimateAppSplash"), "png");
    splashPath.AppendDir("images");
    if (bitmap.LoadFile(splashPath.GetFullPath(), wxBITMAP_TYPE_PNG))
    {
        int flag = timeout ? wxSPLASH_TIMEOUT : wxSPLASH_NO_TIMEOUT;
        result = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_SCREEN|flag,
            3000, NULL, -1, wxDefaultPosition, wxDefaultSize,
            wxBORDER_DOUBLE|wxSTAY_ON_TOP);
         if (timeout)
            wxYield();
    }

    return result;
}


/*
 * wxEVT_IDLE event handler
 */

void TestWindowApp::OnIdle( wxIdleEvent& event )
{
    event.Skip();

    if(IsMainLoopRunning())
    {
        if (m_firstIdle)
        {
            m_firstIdle = false;
            wxLogMessage("First Idle");
            theStore->mainFrame()->StartInitTimer();
        }
    }
    //if (!theStore->getReady())
        //event.RequestMore();

}


