/////////////////////////////////////////////////////////////////////////////
// Name:        luaedit.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sat 20 Feb 2016 22:20:46 AEDT
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _LUAEDIT_H_
#define _LUAEDIT_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class LuaEdit;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_LUAEDIT_STYLE wxSUNKEN_BORDER|wxTAB_TRAVERSAL
#define SYMBOL_LUAEDIT_IDNAME ID_NEW_LUA_EDIT
#define SYMBOL_LUAEDIT_SIZE wxDefaultSize
#define SYMBOL_LUAEDIT_POSITION wxDefaultPosition
////@end control identifiers


#include <wx/stc/stc.h>


/*!
 * LuaEdit class declaration
 */

class LuaEdit: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( LuaEdit )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LuaEdit();
    LuaEdit(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSUNKEN_BORDER|wxTAB_TRAVERSAL);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSUNKEN_BORDER|wxTAB_TRAVERSAL);

    /// Destructor
    ~LuaEdit();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin LuaEdit event handler declarations

////@end LuaEdit event handler declarations

////@begin LuaEdit member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end LuaEdit member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin LuaEdit member variables
    /// Control identifiers
    enum {
        ID_NEW_LUA_EDIT = 10057
    };
////@end LuaEdit member variables


    void SetFilePath(const wxString& path);
    
    wxStyledTextCtrl* luaEdit_;
    wxString    savePath_;
    wxString    tabName_;
    
};

#endif
    // _LUAEDIT_H_
