/////////////////////////////////////////////////////////////////////////////
// Name:        datalayerdlg.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Wed 27 Jan 2016 11:43:55 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _DATALAYERDLG_H_
#define _DATALAYERDLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
////@end includes


#include "json/json.h"
#include <memory>
/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
class wxColourPickerCtrl;
class LineStyleCombo;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_DATALAYERDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_DATALAYERDLG_TITLE _("DataLayerDlg")
#define SYMBOL_DATALAYERDLG_IDNAME ID_DATALAYERDLG
#define SYMBOL_DATALAYERDLG_SIZE wxSize(500, 300)
#define SYMBOL_DATALAYERDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * DataLayerDlg class declaration
 */

namespace agw {
 class DataLayer;
 class PlotLayer;

 typedef std::shared_ptr<PlotLayer> PlotPtr;

};




class DataLayerDlg: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( DataLayerDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    DataLayerDlg();
    DataLayerDlg( wxWindow* parent, wxWindowID id = SYMBOL_DATALAYERDLG_IDNAME, const wxString& caption = SYMBOL_DATALAYERDLG_TITLE, const wxPoint& pos = SYMBOL_DATALAYERDLG_POSITION, const wxSize& size = SYMBOL_DATALAYERDLG_SIZE, long style = SYMBOL_DATALAYERDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_DATALAYERDLG_IDNAME, const wxString& caption = SYMBOL_DATALAYERDLG_TITLE, const wxPoint& pos = SYMBOL_DATALAYERDLG_POSITION, const wxSize& size = SYMBOL_DATALAYERDLG_SIZE, long style = SYMBOL_DATALAYERDLG_STYLE );

    /// Destructor
    ~DataLayerDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin DataLayerDlg event handler declarations

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
    void OnListboxSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX1
    void OnListbox1Selected( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL
    void OnSpinctrlUpdated( wxSpinEvent& event );

    /// wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURCTRL
    void OnColourctrlColourPickerChanged( wxColourPickerEvent& event );

    /// wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURPICKERCTRL
    void OnColourpickerctrlColourPickerChanged( wxColourPickerEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1
    void OnSpinctrl1Updated( wxSpinEvent& event );

    /// wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURPICKERCTRL1
    void OnColourpickerctrl1ColourPickerChanged( wxColourPickerEvent& event );

    /// wxEVT_PAINT event handler for ID_DISPLAY
    void OnPaintResult( wxPaintEvent& event );

////@end DataLayerDlg event handler declarations

////@begin DataLayerDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end DataLayerDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin DataLayerDlg member variables
    wxListBox* mLayer;
    wxListBox* mSymbol;
    wxSpinCtrl* mSymbolSize;
    wxColourPickerCtrl* mSymbolBorder;
    wxColourPickerCtrl* mSymbolFill;
    LineStyleCombo* mLineStyle;
    wxSpinCtrl* mLineThick;
    wxColourPickerCtrl* mLineColor;
    wxTextCtrl* mLabel;
    wxPanel* mDisplay;
    /// Control identifiers
    enum {
        ID_DATALAYERDLG = 10000,
        ID_LISTBOX = 10001,
        ID_NOTEBOOK = 10002,
        ID_SYM_PANEL = 10003,
        ID_LISTBOX1 = 10005,
        ID_SPINCTRL = 10006,
        ID_COLOURCTRL = 10007,
        ID_COLOURPICKERCTRL = 10008,
        ID_LINE_PANEL = 10004,
        ID_SPINCTRL1 = 10011,
        ID_COLOURPICKERCTRL1 = 10010,
        ID_LABEL = 10014,
        ID_DISPLAY = 10009
    };
////@end DataLayerDlg member variables

    std::vector<agw::DataLayer*> layers_;

    //wxArrayString   m_arrItems; // pen style names
    Json::Value     store;
    int     layerIX_;

    void fromLayers(std::vector<agw::PlotPtr>& layers);
    void toJSON();
    void toLayers();

};

#endif
    // _DATALAYERDLG_H_
