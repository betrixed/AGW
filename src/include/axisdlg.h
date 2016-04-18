/////////////////////////////////////////////////////////////////////////////
// Name:        axisdlg.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Mon 18 Apr 2016 17:44:42 AEST
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _AXISDLG_H_
#define _AXISDLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/clrpicker.h"
#include "wx/gbsizer.h"
#include "wx/spinctrl.h"
#include "wx/datectrl.h"
#include "wx/dateevt.h"
////@end includes

#include "appdata.h"
#include "linearscale.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxNotebook;
class wxColourPickerCtrl;
class wxSpinCtrl;
class wxDatePickerCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_AXISDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_AXISDLG_TITLE _("Axis Dialog")
#define SYMBOL_AXISDLG_IDNAME ID_AXISDLG
#define SYMBOL_AXISDLG_SIZE wxSize(500, 400)
#define SYMBOL_AXISDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * AxisDlg class declaration
 */

class PlotFrame;

class AxisDlg: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( AxisDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AxisDlg();
    AxisDlg( wxWindow* parent, wxWindowID id = SYMBOL_AXISDLG_IDNAME, const wxString& caption = SYMBOL_AXISDLG_TITLE, const wxPoint& pos = SYMBOL_AXISDLG_POSITION, const wxSize& size = SYMBOL_AXISDLG_SIZE, long style = SYMBOL_AXISDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_AXISDLG_IDNAME, const wxString& caption = SYMBOL_AXISDLG_TITLE, const wxPoint& pos = SYMBOL_AXISDLG_POSITION, const wxSize& size = SYMBOL_AXISDLG_SIZE, long style = SYMBOL_AXISDLG_STYLE );

    /// Destructor
    ~AxisDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AxisDlg event handler declarations

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_AXIS_ID
    void OnAxisIdSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_REVERT
    void OnRevertClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
    void OnApplyClick( wxCommandEvent& event );

////@end AxisDlg event handler declarations

////@begin AxisDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AxisDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AxisDlg member variables
    wxListBox* mAxisId;
    wxButton* mBtnRestore;
    wxButton* mBtnApply;
    wxButton* mBtnCancel;
    wxButton* mButtonOK;
    wxNotebook* mUnitsBook;
    wxPanel* mPageRender;
    wxColourPickerCtrl* mColorBack;
    wxColourPickerCtrl* mColorAxis;
    wxColourPickerCtrl* mColorRefLine;
    wxColourPickerCtrl* mColorGrid;
    wxCheckBox* mXReverse;
    wxCheckBox* mXAutolimits;
    wxCheckBox* mXAutoAxis;
    wxCheckBox* mXTextRotate;
    wxCheckBox* mXInside;
    wxCheckBox* mXLabelInside;
    wxCheckBox* mXBoxMajor;
    wxCheckBox* mXBoxSmall;
    wxCheckBox* mXBoxRef;
    wxPanel* mPageJSON;
    wxTextCtrl* mJSON;
    wxPanel* mPageNumber;
    wxTextCtrl* mXDivMajor;
    wxSpinCtrl* mXDivSmall;
    wxTextCtrl* mXRefLine;
    wxTextCtrl* mXMin;
    wxTextCtrl* mXMax;
    wxListBox* mXListUnits;
    wxPanel* mPageDate;
    wxDatePickerCtrl* mDateFrom;
    wxDatePickerCtrl* mDateTo;
    /// Control identifiers
    enum {
        ID_AXISDLG = 10000,
        ID_AXIS_ID = 10001,
        ID_UNITS_NOTE = 10002,
        ID_PAGE_RENDER = 10003,
        ID_COLOR_BACK = 10023,
        ID_COLOR_AXIS = 10024,
        ID_COLOR_REFLINE = 10025,
        ID_COLOUR_GRID = 10038,
        ID_XBOX_REVERSE = 10034,
        ID_XBOX_AUTOLIMITS = 10031,
        ID_XBOX_AUTOAXIS = 10027,
        ID_XBOX_ROTATE = 10054,
        ID_XBOX_INSIDE = 10056,
        ID_LABEL_INSIDE = 10058,
        ID_XBOX_MAJOR = 10010,
        ID_XBOX_MINOR = 10011,
        ID_XBOX_REFLINE = 10014,
        ID_PAGE_JSON = 10004,
        ID_JSON = 10005,
        ID_PAGE_NUMBER = 10006,
        ID_XDIV_MAJOR = 10008,
        ID_XSPIN_MINOR = 10009,
        ID_XEDIT_REFLINE = 10013,
        ID_XMIN = 10032,
        ID_XMAX = 10033,
        ID_XLIST_UNITS = 10012,
        ID_PAGE_DATE = 10007,
        ID_DATE_FROM = 10012,
        ID_DATE_TO = 10015
    };
////@end AxisDlg member variables

    PlotFrame*  plotFrame;

    agw::LinearScale    origXAxis;
    agw::LinearScale    origYAxis;

    agw::LinearScale    tempXAxis;
    agw::LinearScale    tempYAxis;

    int  selectedAxis;
    bool allowAxisSelect;

    void fromAxis(const agw::LinearScale& axis);
    void toAxis(agw::LinearScale& xaxis);

    void setPlotFrame(PlotFrame* pf);
    void setFromJson(const Json::Value& json);

};


#endif
    // _AXISDLG_H_
