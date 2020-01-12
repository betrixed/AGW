/////////////////////////////////////////////////////////////////////////////
// Name:        luaedit.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Sat 20 Feb 2016 22:20:46 AEDT
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

#include "luaedit.h"
#include <wx/filename.h>
#include <wx/stc/stc.h>

////@begin XPM images
////@end XPM images

enum
{
    MARGIN_LINE_NUMBERS
};

/*
 * LuaEdit type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LuaEdit, wxPanel )


/*
 * LuaEdit event table definition
 */

BEGIN_EVENT_TABLE( LuaEdit, wxPanel )

////@begin LuaEdit event table entries
////@end LuaEdit event table entries

END_EVENT_TABLE()


/*
 * LuaEdit constructors
 */

LuaEdit::LuaEdit()
{
    Init();
}

LuaEdit::LuaEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * LuaEdit creator
 */

bool LuaEdit::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin LuaEdit creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
////@end LuaEdit creation
    return true;
}


/*
 * LuaEdit destructor
 */

LuaEdit::~LuaEdit()
{
////@begin LuaEdit destruction
////@end LuaEdit destruction
}


/*
 * Member initialisation
 */

void LuaEdit::Init()
{
////@begin LuaEdit member initialisation
////@end LuaEdit member initialisation
}


/*
 * Control creation for LuaEdit
 */

void LuaEdit::CreateControls()
{
////@begin LuaEdit content construction
////@end LuaEdit content construction


    luaEdit_ = new wxStyledTextCtrl(this,wxID_ANY);


    luaEdit_->SetMarginWidth (MARGIN_LINE_NUMBERS, 50);
    luaEdit_->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (75, 75, 75) );
    luaEdit_->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColour (220, 220, 220));
    luaEdit_->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

    luaEdit_->SetWrapMode (wxSTC_WRAP_WORD);

    luaEdit_->SetLexer(wxSTC_LEX_LUA);

    luaEdit_->StyleSetForeground (wxSTC_LUA_DEFAULT,     wxColour(0,0,0));
    luaEdit_->StyleSetForeground (wxSTC_LUA_COMMENT,     wxColour(150,220,0));
    luaEdit_->StyleSetForeground (wxSTC_LUA_COMMENTLINE,           wxColour(255,0,0));
    luaEdit_->StyleSetForeground (wxSTC_LUA_COMMENTDOC,              wxColour(0,150,0));
    luaEdit_->StyleSetForeground (wxSTC_LUA_NUMBER,       wxColour(0,150,0));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD,        wxColour(180,80,220));
    luaEdit_->StyleSetForeground (wxSTC_LUA_STRING, wxColour(0,0,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_CHARACTER,          wxColour(0,0,0));
    luaEdit_->StyleSetForeground (wxSTC_LUA_LITERALSTRING,          wxColour(150,150,250));
    luaEdit_->StyleSetForeground (wxSTC_LUA_PREPROCESSOR,          wxColour(150,250,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_OPERATOR,          wxColour(120,150,220));
    luaEdit_->StyleSetForeground (wxSTC_LUA_COMMENTLINE,          wxColour(150,50,250));
    luaEdit_->StyleSetForeground (wxSTC_LUA_IDENTIFIER,          wxColour(50,30,25));
    luaEdit_->StyleSetForeground (wxSTC_LUA_STRINGEOL,          wxColour(50,150,200));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD2,          wxColour(200,230,180));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD3,          wxColour(150,150,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD4,          wxColour(150,150,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD5,          wxColour(150,150,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD6,          wxColour(150,150,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD7,          wxColour(150,150,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_WORD8,          wxColour(150,150,150));
    luaEdit_->StyleSetForeground (wxSTC_LUA_LABEL,          wxColour(100,120,220));



    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(luaEdit_, 1, wxEXPAND);
    SetSizer(sizer);

    luaEdit_->Show();
}


/*
 * Should we show tooltips?
 */

bool LuaEdit::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap LuaEdit::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin LuaEdit bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end LuaEdit bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon LuaEdit::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin LuaEdit icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end LuaEdit icon retrieval
}

void LuaEdit::SetFilePath(const wxString& path)
{
    savePath_ = path;
    wxFileName fn;

    fn.Assign(path);
    tabName_ = fn.GetName();

}
