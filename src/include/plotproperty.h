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
#include "wx/clrpicker.h"
#include "wx/fontpicker.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
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
