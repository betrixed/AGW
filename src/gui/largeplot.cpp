/////////////////////////////////////////////////////////////////////////////
// Name:        largeplot.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     02/11/2013 00:04:06
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
#include "plotxy.h"
////@end includes

#include "largeplot.h"
#include "plotoptions.h"
#include "monthaverage.h"
#include <sstream>
#include <wx/filedlg.h>
#include "plotlua.h"
#include "plotxy.h"
////@begin XPM images
////@end XPM images

using namespace agw;

/*
 * LargePlot type definition
 */

IMPLEMENT_CLASS( LargePlot, wxFrame )


/*
 * LargePlot event table definition
 */

BEGIN_EVENT_TABLE( LargePlot, wxFrame )

////@begin LargePlot event table entries
    EVT_MENU( ID_OPTIONS, LargePlot::OnOptionsClick )
    EVT_MENU( ID_MONTH_AVERAGE, LargePlot::OnMonthAverageClick )
    EVT_MENU( ID_MENU_EXPORT, LargePlot::OnMenuExportClick )
    EVT_MENU( ID_EXPORT_IMAGE, LargePlot::OnExportImageClick )
    EVT_MENU( ID_RAINFALL_TOTAL, LargePlot::OnRainfallTotalClick )
////@end LargePlot event table entries

END_EVENT_TABLE()


/*
 * LargePlot constructors
 */

LargePlot::LargePlot()
{
    Init();
}

LargePlot::LargePlot( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * LargePlot creator
 */

bool LargePlot::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin LargePlot creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end LargePlot creation
    return true;
}


/*
 * LargePlot destructor
 */

LargePlot::~LargePlot()
{
////@begin LargePlot destruction
////@end LargePlot destruction
}


/*
 * Member initialisation
 */

void LargePlot::Init()
{
////@begin LargePlot member initialisation
    plot_ = NULL;
////@end LargePlot member initialisation
}


/*
 * Control creation for LargePlot
 */

void LargePlot::CreateControls()
{
////@begin LargePlot content construction
    LargePlot* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_OPTIONS, _("&Options ..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_MONTH_AVERAGE, _("Month Temperature Average ..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_MENU_EXPORT, _("&Export CSV"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_EXPORT_IMAGE, _("Export &Image"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_RAINFALL_TOTAL, _("Monthly Rainfall Total"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("Plot"));
    itemFrame1->SetMenuBar(menuBar);

    plot_ = new PlotXY( itemFrame1, PlotXY::ID_SCROLLEDWINDOW, wxDefaultPosition, wxSize(500, 300), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );

////@end LargePlot content construction
}


/*
 * Should we show tooltips?
 */

bool LargePlot::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap LargePlot::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin LargePlot bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end LargePlot bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon LargePlot::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin LargePlot icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end LargePlot icon retrieval
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPTIONS
 */

void LargePlot::OnOptionsClick( wxCommandEvent& event )
{
    event.Skip();
    PlotOptions* pod = new PlotOptions(this);
    pod->plotWindow_ = this->plot_;
    pod->plot_ = this->plot_->plotLua_;

    pod->SetFromPlot();
    pod->Show();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MONTH_AVERAGE
 */
static MonthAverageOptions  AvgOptions;

void LargePlot::OnMonthAverageClick( wxCommandEvent& event )
{

    event.Skip();

    MonthAverage mav(this);
    mav.SetOptions(AvgOptions);
    auto result = mav.ShowModal();


    if ((result == wxID_OK) && mav.UpdateOptions(AvgOptions))
    {
        // do this for each month of the year
        if (AvgOptions.baseLineStartYear >= AvgOptions.baseLineEndYear ||
                AvgOptions.compareStartYear >= AvgOptions.compareEndYear ||
                AvgOptions.baseLineEndYear >= AvgOptions.compareStartYear)
        {
            wxLogError("Year ranges are empty or overlap each other");
            return;
        }

        int YearNo;
        int MonthNo;

        auto pp = plot_->plotLua_;
		agw::DataLayer* p = dynamic_cast<agw::DataLayer*>(pp->layers_[0].get());

        Series *ds = p->xdata_.get();
        Series *ts = p->ydata_.get();

        std::vector<double> compare[12];
        std::vector<double> base[12];
        NormalStats baseStats[12];
        NormalStats compareStats[12];

        size_t ix;
        wxDateTime   theDate;
        // collect baseLine
        // assume ordered date observation sequence

        // init series
        wxString title;
        std::stringstream ss;

        title.Printf(wxT("Monthly Average comparison between %d to %d and %d to %d"),
            AvgOptions.baseLineStartYear,AvgOptions.baseLineEndYear,
            AvgOptions.compareStartYear, AvgOptions.compareEndYear);

        for(ix = 0; ix < ds->size(); ix++)
        {
               theDate.Set( (*ds)[ix] + 2400000.5);
               YearNo = theDate.GetYear();
               if (YearNo >= AvgOptions.baseLineStartYear && YearNo < AvgOptions.baseLineEndYear)
               {
                    MonthNo = (int) theDate.GetMonth(); // index starting with 0
                    base[MonthNo].push_back((*ts)[ix]);
               }
               else if (YearNo >= AvgOptions.compareStartYear && YearNo < AvgOptions.compareEndYear)
               {
                    MonthNo = (int) theDate.GetMonth(); // index starting with 0
                    compare[MonthNo].push_back((*ts)[ix]);
               }
        }
        // calculate NormalStats
        for(ix = 0; ix < 12; ix++)
        {
			baseStats[ix].calc(base[ix]);
			compareStats[ix].calc(compare[ix]);
        }
		// show plot
		std::vector<double> MonthId;
		std::vector<double> CompareData;
		std::vector<double> CompareDev;
		std::vector<double> BaseData;
		std::vector<double> BaseDev;

		for(ix = 0; ix < 12; ix++)
		{
			MonthId.push_back(ix);
			CompareData.push_back(compareStats[ix].mean_);
			CompareDev.push_back(compareStats[ix].stddev_);
			BaseData.push_back(baseStats[ix].mean_);
			BaseDev.push_back(baseStats[ix].stddev_);
		}

		SeriesPtr months = std::make_shared<DoubleSeries>(MonthId,DATE_MONTH_NUM,"Month");

        ss.str("");
        ss << "Average Monthly temperature from " << AvgOptions.baseLineStartYear << " to " << AvgOptions.baseLineEndYear;

		SeriesPtr avg1 = std::make_shared<DoubleSeries>(BaseData,DEFAULT,ss.str());
		SeriesPtr dev1 = std::make_shared<DoubleSeries>(BaseDev,DEFAULT,"std-dev");


		LargePlot* f = new LargePlot(this->GetParent());
        f->SetTitle(title);
		f->Show();
        auto xyplot = f->plot_;
        xyplot->SetVirtualSize(1000,500);

		agw::PlotLayer_sptr sp = std::make_shared<agw::DataLayer>();
		agw::DataLayer* layer = static_cast<agw::DataLayer*>(sp.get());

		layer->xdata_ = months;
		layer->ydata_ = avg1;
		layer->errorbar_ = dev1;
		layer->symbolBorder_.Set(0x66,0x88,0xDD);
		xyplot->AddData(sp);

        ss << "Average Monthly temperature from " << AvgOptions.compareStartYear << " to " << AvgOptions.compareEndYear;
        std::make_shared<DoubleSeries>(BaseData,DEFAULT,ss.str());
        SeriesPtr avg2 = std::make_shared<DoubleSeries>(CompareData,DEFAULT,ss.str());
        SeriesPtr dev2 = std::make_shared<DoubleSeries>(CompareDev,DEFAULT,"std-dev");

		sp = std::make_shared<agw::DataLayer>();
		layer = static_cast<agw::DataLayer*>(sp.get());

		layer->xdata_ = months;
		layer->ydata_ = avg2;
		layer->errorbar_ = dev2;
		layer->symbolBorder_.Set(0xDD,0x66,0x88);
		xyplot->AddData(sp);


        xyplot->Refresh();

    }
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_EXPORT
 */

void LargePlot::OnMenuExportClick( wxCommandEvent& event )
{
    event.Skip();
    // Do a save as, CSV, export the data points on the plot

    wxFileDialog   SaveAsDlg(this, "Export to CSV", wxEmptyString, wxEmptyString,
        "CSV files (*.csv)|*.csv",wxFD_SAVE, wxDefaultPosition, wxDefaultSize,"PlotExportCSV");

    if (SaveAsDlg.ShowModal() == wxID_OK)
    {
        wxString path = SaveAsDlg.GetPath();
        plot_->ExportToCSV(path);
    }



}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_IMAGE
 */

void LargePlot::OnExportImageClick( wxCommandEvent& event )
{
    event.Skip();
    wxFileDialog   SaveAsDlg(this, "Export to PNG", wxEmptyString, wxEmptyString,
        "PNG files (*.png)|*.png",wxFD_SAVE, wxDefaultPosition, wxDefaultSize,"PlotExportPNG");
    if (SaveAsDlg.ShowModal() == wxID_OK)
    {
        wxString path = SaveAsDlg.GetPath();
        plot_->ExportToPNG(path);
    }
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_RAINFALL_TOTAL
 */

void LargePlot::OnRainfallTotalClick( wxCommandEvent& event )
{

    event.Skip();

    MonthAverage mav(this);
    mav.SetOptions(AvgOptions);
    auto result = mav.ShowModal();


    if ((result = wxID_OK) && mav.UpdateOptions(AvgOptions))
    {
        // do this for each month of the year
        if (AvgOptions.baseLineStartYear >= AvgOptions.baseLineEndYear ||
                AvgOptions.compareStartYear >= AvgOptions.compareEndYear ||
                AvgOptions.baseLineEndYear >= AvgOptions.compareStartYear)
        {
            wxLogError("Year ranges are empty or overlap each other");
            return;
        }

        int YearNo;
        int MonthNo;

        auto pp = plot_->plotLua_;

		agw::DataLayer* p = dynamic_cast<agw::DataLayer*> (pp->layers_[0].get());

        Series *ds = p->xdata_.get();
        Series *ts = p->ydata_.get();

        std::vector<double> compare[12];
        std::vector<double> base[12];
        NormalStats baseStats[12];
        NormalStats compareStats[12];

        size_t ix;
        wxDateTime   theDate;
        // collect baseLine
        // assume ordered date observation sequence

        // init series
        wxString title;
        std::stringstream ss;

        title.Printf(wxT("Monthly Average Rainfall between %d to %d and %d to %d"),
            AvgOptions.baseLineStartYear,AvgOptions.baseLineEndYear,
            AvgOptions.compareStartYear, AvgOptions.compareEndYear);
        int oldYearNo = 0;
        int m = 0;
        double BaseOneYear[12]; // totals for one year
        double CompareOneYear[12];

        // for each year , must total the month.
        for(ix = 0; ix < ds->size(); ix++)
        {
               theDate.Set( (*ds)[ix] + 2400000.5);
               YearNo = theDate.GetYear();
               MonthNo = (int) theDate.GetMonth(); // index starting with 0
               if (YearNo != oldYearNo)
               {
                  if (oldYearNo == 0)
                  {
                    oldYearNo = YearNo;
                  }
                  else {
                       if (oldYearNo >= AvgOptions.baseLineStartYear && oldYearNo < AvgOptions.baseLineEndYear)
                       {
                            for( m = 0; m < 12; m++)
                                base[m].push_back(BaseOneYear[m]);
                       }
                       else if (oldYearNo >= AvgOptions.compareStartYear && oldYearNo < AvgOptions.compareEndYear)
                       {
                            for( m = 0; m < 12; m++)
                                compare[m].push_back(CompareOneYear[m]);
                       }
                       oldYearNo = YearNo;
                  }
                  for( int m = 0; m < 12; m++)
                  {
                     BaseOneYear[m] = 0.0;
                     CompareOneYear[m] = 0.0;
                  }
               }
               else {
                   // save in current year
                   auto rainfall = (*ts)[ix];
                   if (rainfall > 0.0)
                   {
                       if (YearNo >= AvgOptions.baseLineStartYear && YearNo < AvgOptions.baseLineEndYear)
                       {
                            BaseOneYear[MonthNo] += rainfall;
                       }
                       else if (YearNo >= AvgOptions.compareStartYear && YearNo < AvgOptions.compareEndYear)
                       {
                            CompareOneYear[MonthNo] += rainfall;
                       }
                    }
               }

        }
        // calculate NormalStats
        for(ix = 0; ix < 12; ix++)
        {
			baseStats[ix].calc(base[ix]);
			compareStats[ix].calc(compare[ix]);
        }
		// show plot
		std::vector<double> MonthId;
		std::vector<double> CompareData;
		std::vector<double> CompareDev;
		std::vector<double> BaseData;
		std::vector<double> BaseDev;

		for(ix = 0; ix < 12; ix++)
		{
			MonthId.push_back(ix);
			CompareData.push_back(compareStats[ix].mean_);
			CompareDev.push_back(compareStats[ix].stddev_);
			BaseData.push_back(baseStats[ix].mean_);
			BaseDev.push_back(baseStats[ix].stddev_);
		}

		SeriesPtr months = std::make_shared<DoubleSeries>(MonthId,DATE_MONTH_NUM,"Month");

		ss.str("");
        ss << "Average Monthly Rainfall from " << AvgOptions.baseLineStartYear << " to " << AvgOptions.baseLineEndYear;
		SeriesPtr avg1 = std::make_shared<DoubleSeries>(BaseData,DEFAULT, ss.str());

		SeriesPtr s1 = std::make_shared<DoubleSeries>(BaseDev,DEFAULT, "std-dev");

		LargePlot* f = new LargePlot(this->GetParent());
        f->SetTitle(title);
		f->Show();
        auto xyplot = f->plot_;
        xyplot->SetVirtualSize(1000,500);

        agw::PlotLayer_sptr sp = std::make_shared<agw::DataLayer>();

		agw::DataLayer* layer = static_cast<agw::DataLayer*>(sp.get());

		layer->xdata_ = months;
		layer->ydata_ = avg1;
		layer->errorbar_ = s1;
		layer->symbolBorder_.Set(0x66,0x88,0xDD);
		xyplot->AddData(sp);


        ss.str("");
        ss << "Average Monthly Rainfall from " << AvgOptions.compareStartYear << " to " << AvgOptions.compareEndYear;
        SeriesPtr avg2 = std::make_shared<DoubleSeries>(CompareData, DEFAULT, ss.str());
        SeriesPtr s2 = std::make_shared<DoubleSeries>(CompareDev, DEFAULT, "std-dev");

        sp = std::make_shared<DataLayer>();
        layer = static_cast<agw::DataLayer*>(sp.get());

		layer->xdata_ = months;
		layer->ydata_ = avg2;
		layer->errorbar_ = s2;
		layer->symbolBorder_.Set(0xDD,0x66,0x88);
		xyplot->AddData(sp);


        xyplot->Refresh();

    }
    event.Skip(false);

}

