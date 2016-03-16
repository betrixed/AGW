/////////////////////////////////////////////////////////////////////////////
// Name:        plotproperty.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Sat 23 Jan 2016 22:21:53 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _PLOTPROPERTY_H_
#define _PLOTPROPERTY_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/propdlg.h"
#include "wx/gbsizer.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "wx/fontpicker.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
class wxColourPickerCtrl;
class wxFontPickerCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_PLOTPROPERTY_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PLOTPROPERTY_TITLE _("PlotProperty")
#define SYMBOL_PLOTPROPERTY_IDNAME ID_PLOTPROPERTY
#define SYMBOL_PLOTPROPERTY_SIZE wxSize(400, 300)
#define SYMBOL_PLOTPROPERTY_POSITION wxDefaultPosition
////@end control identifiers


#include "json/json.h"

namespace agw {
    class LinearScale;
};

/*!
 * PlotProperty class declaration
 */

class PlotProperty: public wxPropertySheetDialog
{
    DECLARE_DYNAMIC_CLASS( PlotProperty )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    PlotProperty();
    PlotProperty( wxWindow* parent, wxWindowID id = SYMBOL_PLOTPROPERTY_IDNAME, const wxString& caption = SYMBOL_PLOTPROPERTY_TITLE, const wxPoint& pos = SYMBOL_PLOTPROPERTY_POSITION, const wxSize& size = SYMBOL_PLOTPROPERTY_SIZE, long style = SYMBOL_PLOTPROPERTY_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PLOTPROPERTY_IDNAME, const wxString& caption = SYMBOL_PLOTPROPERTY_TITLE, const wxPoint& pos = SYMBOL_PLOTPROPERTY_POSITION, const wxSize& size = SYMBOL_PLOTPROPERTY_SIZE, long style = SYMBOL_PLOTPROPERTY_STYLE );

    /// Destructor
    ~PlotProperty();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin PlotProperty event handler declarations

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_XBOX_AUTOLIMITS
    void OnXboxAutolimitsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_YBOX_AUTOLIMITS
    void OnYboxAutolimitsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LABEL_LIST
    void OnLabelListSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE_LABEL
    void OnDeleteLabelClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_LABEL
    void OnAddLabelClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_ATTACH_LIST
    void OnAttachListSelected( wxCommandEvent& event );

    /// wxEVT_KILL_FOCUS event handler for ID_LABEL_CONTENT
    void OnKillFocus( wxFocusEvent& event );

////@end PlotProperty event handler declarations

////@begin PlotProperty member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PlotProperty member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PlotProperty member variables
    wxCheckBox* mLFrame;
    wxCheckBox* mTFrame;
    wxCheckBox* mRFrame;
    wxCheckBox* mBFrame;
    wxCheckBox* mBoxFixed;
    wxTextCtrl* mXSize;
    wxTextCtrl* mYSize;
    wxTextCtrl* mXMin;
    wxTextCtrl* mXMax;
    wxCheckBox* mXReverse;
    wxCheckBox* mXAutolimits;
    wxTextCtrl* mXDivMajor;
    wxCheckBox* mXBoxMajor;
    wxSpinCtrl* mXDivSmall;
    wxCheckBox* mXBoxSmall;
    wxTextCtrl* mXRefLine;
    wxCheckBox* mXBoxRef;
    wxCheckBox* mXAutoAxis;
    wxCheckBox* mXTextRotate;
    wxCheckBox* mXInside;
    wxCheckBox* mXLabelInside;
    wxListBox* mXListUnits;
    wxTextCtrl* mYMin;
    wxTextCtrl* mYMax;
    wxCheckBox* mYReverse;
    wxCheckBox* mYAutolimits;
    wxTextCtrl* mYDivMajor;
    wxCheckBox* mYBoxMajor;
    wxSpinCtrl* mYDivSmall;
    wxCheckBox* mYBoxSmall;
    wxTextCtrl* mYRefLine;
    wxCheckBox* mYBoxRef;
    wxCheckBox* mYAutoAxis;
    wxCheckBox* mYTextRotate;
    wxCheckBox* mYInside;
    wxCheckBox* mYLabelInside;
    wxListBox* mYListUnits;
    wxColourPickerCtrl* mColorBack;
    wxColourPickerCtrl* mColorAxis;
    wxColourPickerCtrl* mColorRefLine;
    wxColourPickerCtrl* mColorGrid;
    wxListBox* mLabelList;
    wxButton* mDeleteLabel;
    wxButton* mAddLabel;
    wxListBox* mRotateList;
    wxListBox* mAttachToList;
    wxTextCtrl* mXLabel;
    wxTextCtrl* mYLabel;
    wxFontPickerCtrl* mFont;
    wxTextCtrl* mLabelText;
    /// Control identifiers
    enum {
        ID_PLOTPROPERTY = 10000,
        ID_PP_MARGIN = 10001,
        ID_EDIT_LMARGIN = 10004,
        ID_BOX_LFRAME = 10039,
        ID_EDIT_TMARGIN = 10005,
        ID_BOX_TFRAME = 10040,
        ID_EDIT_RMARGIN = 10007,
        ID_BOX_RFRAME = 10041,
        ID_EDIT_BMARGIN = 10006,
        ID_BOX_BFRAME = 10042,
        ID_BOX_FIXED = 10028,
        ID_XSIZE = 10029,
        ID_YSIZE = 10030,
        ID_PP_XSCALE = 10002,
        ID_XMIN = 10032,
        ID_XMAX = 10033,
        ID_XBOX_REVERSE = 10034,
        ID_XBOX_AUTOLIMITS = 10031,
        ID_XDIV_MAJOR = 10008,
        ID_XBOX_MAJOR = 10010,
        ID_XSPIN_MINOR = 10009,
        ID_XBOX_MINOR = 10011,
        ID_XEDIT_REFLINE = 10013,
        ID_XBOX_REFLINE = 10014,
        ID_XBOX_AUTOAXIS = 10027,
        ID_XBOX_ROTATE = 10054,
        ID_XBOX_INSIDE = 10056,
        ID_LABEL_INSIDE = 10058,
        ID_XLIST_UNITS = 10012,
        ID_PP_YSCALE = 10003,
        ID_YMIN = 10035,
        ID_YMAX = 10036,
        ID_YBOX_REVERSE = 10037,
        ID_YBOX_AUTOLIMITS = 10043,
        ID_YDIV_MAJOR = 10015,
        ID_YBOX_MAJOR = 10016,
        ID_YDIV_SMALL = 10017,
        ID_YBOX_SMALL = 10018,
        ID_YREF_LINE = 10019,
        ID_YBOX_REFLINE = 10020,
        ID_YBOX_AUTOTICK = 10026,
        ID_YBOX_ROTATE = 10055,
        ID_YBOX_INSDIE = 10057,
        ID_YLABEL_INSIDE = 10059,
        ID_YLIST_UNITS = 10021,
        ID_PLOT_COLOR = 10022,
        ID_COLOR_BACK = 10023,
        ID_COLOR_AXIS = 10024,
        ID_COLOR_REFLINE = 10025,
        ID_COLOUR_GRID = 10038,
        ID_PANEL = 10044,
        ID_LABEL_LIST = 10045,
        ID_DELETE_LABEL = 10046,
        ID_ADD_LABEL = 10047,
        ID_ROTATE_LIST = 10049,
        ID_ATTACH_LIST = 10050,
        ID_XLABEL_OFFSET = 10051,
        ID_YLABEL_OFFSET = 10052,
        ID_FONTCTRL = 10053,
        ID_LABEL_CONTENT = 10048
    };
////@end PlotProperty member variables

    int   currentTextSelect;
    bool  blockUpdate;
    void saveTextSelect(bool textOnly = false);
    void setLabelObject(Json::Value& json, bool textOnly = false);
    void changeSelection();


    void ReadXAxis(const agw::LinearScale& xaxis);
    void ReadYAxis(const agw::LinearScale& yaxis);

    void SaveXAxis(agw::LinearScale& xaxis);
    void SaveYAxis(agw::LinearScale& xaxis);

    void ReadJSON(const Json::Value& json);
    void SaveJSON( Json::Value& json);


    Json::Value labelStore;

};

#endif
    // _PLOTPROPERTY_H_
