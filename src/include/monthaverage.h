/////////////////////////////////////////////////////////////////////////////
// Name:        monthaverage.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     02/11/2013 13:25:39
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _MONTHAVERAGE_H_
#define _MONTHAVERAGE_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/gbsizer.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_MONTHAVERAGE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_MONTHAVERAGE_TITLE _("Month Average Comparison")
#define SYMBOL_MONTHAVERAGE_IDNAME ID_MONTHAVERAGE
#define SYMBOL_MONTHAVERAGE_SIZE wxSize(400, 300)
#define SYMBOL_MONTHAVERAGE_POSITION wxDefaultPosition
////@end control identifiers



struct MonthAverageOptions {
   int baseLineStartYear;
   int baseLineEndYear;
   int compareStartYear;
   int compareEndYear;
};
/*!
 * MonthAverage class declaration
 */

class MonthAverage: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( MonthAverage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MonthAverage();
    MonthAverage( wxWindow* parent, wxWindowID id = SYMBOL_MONTHAVERAGE_IDNAME, const wxString& caption = SYMBOL_MONTHAVERAGE_TITLE, const wxPoint& pos = SYMBOL_MONTHAVERAGE_POSITION, const wxSize& size = SYMBOL_MONTHAVERAGE_SIZE, long style = SYMBOL_MONTHAVERAGE_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MONTHAVERAGE_IDNAME, const wxString& caption = SYMBOL_MONTHAVERAGE_TITLE, const wxPoint& pos = SYMBOL_MONTHAVERAGE_POSITION, const wxSize& size = SYMBOL_MONTHAVERAGE_SIZE, long style = SYMBOL_MONTHAVERAGE_STYLE );

    /// Destructor
    ~MonthAverage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MonthAverage event handler declarations

////@end MonthAverage event handler declarations

////@begin MonthAverage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MonthAverage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MonthAverage member variables
    wxTextCtrl* editFrom_;
    wxTextCtrl* editTo_;
    wxTextCtrl* editCompareFrom_;
    wxTextCtrl* editCompareTo_;
    /// Control identifiers
    enum {
        ID_MONTHAVERAGE = 10013,
        ID_TEXTCTRL3 = 10014,
        ID_TEXTCTRL4 = 10015,
        ID_TEXTCTRL5 = 10016,
        ID_TEXTCTRL6 = 10017
    };
////@end MonthAverage member variables

    void SetOptions(MonthAverageOptions& val);
    bool UpdateOptions(MonthAverageOptions& val);

};

#endif
    // _MONTHAVERAGE_H_
