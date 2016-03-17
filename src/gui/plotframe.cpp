/////////////////////////////////////////////////////////////////////////////
// Name:        plotframe.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Sat 23 Jan 2016 10:25:31 AEDT
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

#include "plotframe.h"

#include <wx/filedlg.h>
////@begin XPM images
////@end XPM images
#include <wx/filename.h>

#include "plotxy.h"
#include "appdata.h"
#include "mainframe.h"
#include "plotproperty.h"
#include "json/json.h"
#include "seriesframe.h"
#include "seriesgrid.h"
#include "datalayerdlg.h"
#include "plottable.h"
#include "plotlua.h"
#include "helper.h"

using namespace agw;
/*
 * PlotFrame type definition
 */

IMPLEMENT_CLASS( PlotFrame, wxFrame )


/*
 * PlotFrame event table definition
 */

BEGIN_EVENT_TABLE( PlotFrame, wxFrame )

////@begin PlotFrame event table entries
    EVT_MENU( ID_OPEN_MENU, PlotFrame::OnOpenMenuClick )
    EVT_MENU( ID_SAVE_MENU, PlotFrame::OnSaveMenuClick )
    EVT_MENU( ID_CLOSE, PlotFrame::OnCloseClick )
    EVT_MENU( ID_EXPORT_PNG, PlotFrame::OnExportPngClick )
    EVT_MENU( ID_PLOT_SETTINGS, PlotFrame::OnPlotSettingsClick )
    EVT_MENU( ID_LAYER_MENU, PlotFrame::OnLayerMenuClick )
    EVT_MENU( ID_LEGEND, PlotFrame::OnLegendClick )
    EVT_MENU( ID_DATA_VIEW, PlotFrame::OnDataViewClick )
////@end PlotFrame event table entries

END_EVENT_TABLE()


/*
 * PlotFrame constructors
 */

PlotFrame::PlotFrame()
{
    Init();
}

PlotFrame::PlotFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * PlotFrame creator
 */

bool PlotFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin PlotFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end PlotFrame creation


    MainFrame* mf = static_cast<MainFrame*>(parent);
    theApp = mf->ap_;
    return true;
}


/*
 * PlotFrame destructor
 */

PlotFrame::~PlotFrame()
{
////@begin PlotFrame destruction
////@end PlotFrame destruction
}


/*
 * Member initialisation
 */

void PlotFrame::Init()
{
////@begin PlotFrame member initialisation
    mFileMenu = NULL;
    mMenuView = NULL;
////@end PlotFrame member initialisation

    thePlot = nullptr;
}


/*
 * Control creation for PlotFrame
 */

void PlotFrame::CreateControls()
{
////@begin PlotFrame content construction
    PlotFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    mFileMenu = new wxMenu;
    mFileMenu->Append(ID_OPEN_MENU, _("&Open"), wxEmptyString, wxITEM_NORMAL);
    mFileMenu->Append(ID_SAVE_MENU, _("&Save"), wxEmptyString, wxITEM_NORMAL);
    mFileMenu->AppendSeparator();
    mFileMenu->Append(ID_CLOSE, _("&Close"), wxEmptyString, wxITEM_NORMAL);
    mFileMenu->Append(ID_EXPORT_PNG, _("Export as .PNG"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(mFileMenu, _("&File"));
    mMenuView = new wxMenu;
    mMenuView->Append(ID_PLOT_SETTINGS, _("Frame Settings"), wxEmptyString, wxITEM_NORMAL);
    mMenuView->Append(ID_LAYER_MENU, _("Layer Settings"), wxEmptyString, wxITEM_NORMAL);
    mMenuView->Append(ID_LEGEND, _("Legend"), wxEmptyString, wxITEM_CHECK);
    mMenuView->Append(ID_DATA_VIEW, _("Series Table"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(mMenuView, _("View"));
    itemFrame1->SetMenuBar(menuBar);

////@end PlotFrame content construction
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPEN_MENU
 */

void PlotFrame::OnOpenMenuClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPEN_MENU in PlotFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPEN_MENU in PlotFrame.
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_SAVE_MENU
 */

void PlotFrame::OnSaveMenuClick( wxCommandEvent& event )
{
    event.Skip(false);
    if (!thePlot)
        return;

    wxFileDialog savedlg(this, "Save as", theApp->lastPlotDir(), wxEmptyString,
            STD_PLOT_FILES, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (savedlg.ShowModal() == wxID_CANCEL)
        return;

    wxString path = savedlg.GetPath();

    ensureExtension(path,"plot");

    theApp->lastPlotDir(path);

    thePlot->SaveToFile(path);

    wxLogMessage("Saved file to %s", path.c_str());
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_CLOSE
 */

void PlotFrame::OnCloseClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_CLOSE in PlotFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_CLOSE in PlotFrame.
}


/*
 * Should we show tooltips?
 */

bool PlotFrame::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap PlotFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin PlotFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end PlotFrame bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon PlotFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin PlotFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end PlotFrame icon retrieval
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_ALLOW_RESIZE
 */

void PlotFrame::OnPlotSettingsClick( wxCommandEvent& event )
{
    event.Skip(false);

    Json::Value   prop(Json::objectValue);

    agw::PlotLuaPtr plua = thePlot->plotLua_;

    plua->world_.SaveJSON(prop);
    plua->text_.SaveJSON(prop);

    PlotProperty* pp = new PlotProperty(this);
    pp->ReadJSON(prop);

    if (pp->ShowModal()  == wxID_OK)
    {
        pp->saveTextSelect();
        Json::Value newprop(Json::objectValue);

        pp->SaveJSON(newprop);
        plua->world_.ReadJSON(newprop);
        plua->text_.ReadJSON(pp->labelStore);
        AdjustFixed();

        thePlot->Refresh();
    }
}

void PlotFrame::AdjustFixed(bool toContent)
{
    PixelWorld& w = thePlot->plotLua_->world_;
    this->SetSizeHints(-1, -1, -1, -1); // unlock SetSize
    if (w.fixed_)
    {
        int width = w.left_ + w.right_ + w.xfixed_;
        int height = w.top_ + w.yfixed_ + w.bottom_;


        wxSize theSize(width,height);
        thePlot->SetVirtualSize(theSize);
        thePlot->SetMaxSize(wxSize(theSize));
        // resize frame
        this->SetClientSize(thePlot->GetSize());
        this->Refresh();
        this->Update();
        theSize = this->GetSize(); // get new frame size

        this->SetSizeHints(theSize.x, theSize.y, theSize.x, theSize.y); // lock it
    }
    else if (toContent){
        int width = w.left_ + w.right_ + w.xspan_;
        int height = w.top_ + w.yspan_ + w.bottom_;

        wxSize theSize(width,height);
        thePlot->SetSize(theSize);
        this->SetClientSize(thePlot->GetSize());
        this->Refresh();
        this->Update();
        thePlot->SetVirtualSize(-1,-1);
        this->SetSizeHints(-1, -1, -1, -1); // unlock SetSize
    }
    else {
        thePlot->SetVirtualSize(-1,-1);
        thePlot->SetMaxSize(wxSize(-1,-1));
        }
    this->Refresh();
}
void  PlotFrame::setPlot(PlotXY* p)
{
    thePlot = p;
    AdjustFixed(true);
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_PNG
 */

void PlotFrame::OnExportPngClick( wxCommandEvent& event )
{
    event.Skip(false);
    wxFileDialog   SaveAsDlg(this, "Export to PNG", wxEmptyString, wxEmptyString,
        "PNG files (*.png)|*.png",wxFD_SAVE, wxDefaultPosition, wxDefaultSize,"PlotExportPNG");
    if (SaveAsDlg.ShowModal() == wxID_OK)
    {
        wxString path = SaveAsDlg.GetPath();

        ensureExtension(path,"png");

        thePlot->ExportToPNG(path);
    }
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_LAYER_MENU
 */

void PlotFrame::OnLayerMenuClick( wxCommandEvent& event )
{

    event.Skip(false);
    DataLayerDlg dlg(this);

    dlg.fromLayers(thePlot->plotLua_->layers_);

    if (dlg.ShowModal() == wxID_OK)
    {
        dlg.toLayers();
        thePlot->Refresh();
    }
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_LEGEND
 */

void PlotFrame::OnLegendClick( wxCommandEvent& event )
{

    event.Skip(false);
    if (event.IsChecked())
    {
        thePlot->addLegend();
    }
    else
        thePlot->removeLegend();
    thePlot->Refresh();

}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_DATA_VIEW
 */

void PlotFrame::OnDataViewClick( wxCommandEvent& event )
{
    event.Skip(false);

    // make a plottable
    Json::Value jgraph(Json::objectValue);
    PlotLuaPtr pp = thePlot->plotLua_;

    pp->world_.SaveJSON(jgraph);

    DataTablePtr tp = std::make_shared<DataTable>(pp->layers_, jgraph);

    // make a frame and grid to show
    SeriesFrame* sf = new SeriesFrame(theApp->mainFrame());

    sf->Show();
    pp->m_table = tp;

    //thePlot->m_table = pt;

    auto grid = sf->mGrid;

    grid->BeginBatch();
    grid->SetTable(tp.get());
    grid->AutoSize();
    grid->SetColLabelSize(50);
    grid->ForceRefresh();
    grid->EndBatch();

}

