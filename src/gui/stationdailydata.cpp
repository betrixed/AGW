/////////////////////////////////////////////////////////////////////////////
// Name:        stationdailydata.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Tue 02 Jun 2020 13:27:30 AEST
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

#include "stationdailydata.h"

////@begin XPM images
////@end XPM images

#include "wxCSV.hpp"
#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/arrstr.h>
#include <unordered_map>
#include <limits>
/*
 * StationDailyData type definition
 */

IMPLEMENT_CLASS( StationDailyData, wxFrame )


/*
 * StationDailyData event table definition
 */

BEGIN_EVENT_TABLE( StationDailyData, wxFrame )

////@begin StationDailyData event table entries
////@end StationDailyData event table entries

END_EVENT_TABLE()

#include "appdata.h"
/*
 * StationDailyData constructors
 */

StationDailyData::StationDailyData()
{
    Init();
}

StationDailyData::StationDailyData( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * StationDailyData creator
 */

bool StationDailyData::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin StationDailyData creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end StationDailyData creation
    return true;
}


/*
 * StationDailyData destructor
 */

StationDailyData::~StationDailyData()
{
////@begin StationDailyData destruction
////@end StationDailyData destruction
}


/*
 * Member initialisation
 */

void StationDailyData::Init()
{
////@begin StationDailyData member initialisation
    btnSpinShow = NULL;
    txtDailyData = NULL;
////@end StationDailyData member initialisation
}


/*
 * Control creation for StationDailyData
 */

void StationDailyData::CreateControls()
{
////@begin StationDailyData content construction
    StationDailyData* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer1);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer2, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText3 = new wxStaticText( itemFrame1, wxID_STATIC, _("Year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl4 = new wxTextCtrl( itemFrame1, ID_SHOW_YEAR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemTextCtrl4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemFrame1, wxID_STATIC, _("Month"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice6Strings;
    itemChoice6Strings.Add(_("1"));
    itemChoice6Strings.Add(_("2"));
    itemChoice6Strings.Add(_("3"));
    itemChoice6Strings.Add(_("4"));
    itemChoice6Strings.Add(_("5"));
    itemChoice6Strings.Add(_("6"));
    itemChoice6Strings.Add(_("7"));
    itemChoice6Strings.Add(_("8"));
    itemChoice6Strings.Add(_("9"));
    itemChoice6Strings.Add(_("10"));
    itemChoice6Strings.Add(_("11"));
    itemChoice6Strings.Add(_("12"));
    wxChoice* itemChoice6 = new wxChoice( itemFrame1, ID_SHOW_MONTH, wxDefaultPosition, wxDefaultSize, itemChoice6Strings, 0 );
    itemChoice6->SetStringSelection(_("1"));
    itemBoxSizer2->Add(itemChoice6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemFrame1, ID_SHOW_NOW, _("Show"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnSpinShow = new wxSpinButton( itemFrame1, ID_SPINSHOW, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL );
    btnSpinShow->SetRange(0, 100);
    btnSpinShow->SetValue(0);
    itemBoxSizer2->Add(btnSpinShow, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtDailyData = new wxTextCtrl( itemFrame1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL|wxSUNKEN_BORDER );
    itemBoxSizer1->Add(txtDailyData, 1, wxGROW|wxALL, 5);

////@end StationDailyData content construction
}


/*
 * Should we show tooltips?
 */

bool StationDailyData::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap StationDailyData::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin StationDailyData bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end StationDailyData bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon StationDailyData::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin StationDailyData icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end StationDailyData icon retrieval
}




static std::vector<std::string> csv_header =
    {
        "STATION", "DATE", "LATITUDE", "LONGITUDE",
        "ELEVATION", "NAME",
        "PRCP", "PRCP_ATTRIBUTES",
        "TMAX", "TMAX_ATTRIBUTES",
        "TMIN", "TMIN_ATTRIBUTES",
        "DATX", "DATX_ATTRIBUTES",
        "MDTX", "MIDX_ATTRIBUTES"
    };


wxString
StationDailyData::FullPath(const wxString& stationid) {
    AppData& app = AppData::instance();
    std::string path;
    if(!app.userValue("ghcn-daily", path)) {
        wxLogError("Configure directly path for ghcn-daily");
        return wxEmptyString;
    }
    return wxString::Format("%s/%s.csv", path.c_str(), stationid);
}

// return unquoted character if required
static wxString unquote(const wxString& value) {
    wxString result;
    wxString::const_iterator it = value.begin();;
    wxString::const_iterator eit = value.end();
    const wchar_t dquote = '\"';
    wchar_t  char_end = 0x00;

    if (it != eit)
    {
        if (*it == dquote ) {
            it++;
            char_end = dquote;
        }
        while((it != eit) && (*it != char_end)) {
            result << *it;
            it++;
        }
    }
    return result;
}

//Feed some extra statistics into the GissYear, GissTemp sections in a
//single pass through a daily station text file
void StationDailyData::SetStationId(const wxString& name, const wxString& filepath)
{
    stationId = name;

    wxFileInputStream input_stream(filepath);
    double fileLength = input_stream.GetLength();
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", filepath.utf8_str());
        return;
    }
    wxTextInputStream   text(input_stream, ',');
    wxCSV rdr(&text, ',');
    wxArrayString row;
    int tmax_column = -1;
    int date_column = -1;
    int tmin_column  = -1;
    int prcp_column = -1;
    std::unordered_map<std::string, int> col_names;

    if(rdr.next(row))
    {
        // check headers
        for(int ix = 0; ix < (int)row.GetCount(); ix++) {
            auto check = row[ix];
            std::string key(check.utf8_str());
            col_names.insert({key, ix});
        }
    }
    tmax_column = col_names["TMAX"];
    tmin_column = col_names["TMIN"];
    prcp_column = col_names["PRCP"];
    date_column = col_names["DATE"];

    wxString tmax_str;
    wxString tmin_str;
    wxString prcp_str;
    wxString date_str;
    wxString day_str;
    wxString month_str;
    wxString year_str;

    wxRegEx  dexp;
    dexp.Compile("(\\d\\d\\d\\d)\\-(\\d\\d)\\-(\\d\\d)",wxRE_ADVANCED);
    long yearval;
    long monthval;
    long dateindex;
    long bucketindex = 0;
    int32_t byearval;
    int32_t bmonthval;

    wxLogMessage("Columns %d %d %d %d",date_column,prcp_column, tmax_column,tmin_column );
    wxString line_str;
    SqliteDB& sdb = ap_->getDB();

    double tmaxsum,tminsum,tmaxsqsum,tminsqsum, tavgsum, tavgsqsum;
    double tmax_d, tmin_d, tavg_d;
    double mmax, mmin;
    long ndays = 0;
    long yearout, monthout;
    wxString output;
    GissYear gissyear;
    GissYear mmaxyear;
    GissYear tmaxyear;

    MonthTemp gisstemp;
    MonthTemp mmaxtemp;
    MonthTemp tmaxtemp;

    Station4 gissloc;
    std::string stationid(name.utf8_str());
    wxString firstdate;

    gissloc.loadByCode(sdb,stationid);
    while (rdr.next(row)) {
        tmax_str = row[tmax_column];


        if (tmax_str.size() > 0) {
            date_str = row[date_column];
            if (dexp.Matches(date_str) && dexp.GetMatchCount() == 4) {
                year_str = dexp.GetMatch(date_str,1);
                month_str = dexp.GetMatch(date_str,2);
                day_str = dexp.GetMatch(date_str,3);
                year_str.ToCLong(&yearval,10);
                month_str.ToCLong(&monthval,10);

                //yearval = std::strtol(year_str.utf8_str(),nullptr,10);
                //monthval = std::strtol(month_str.utf8_str(),nullptr,10);
                dateindex = yearval*12 + (monthval-1);
            }
            else {
                continue;
            }
            if (firstdate.IsEmpty()) {
                firstdate = date_str;
            }

            tmin_str = row[tmin_column];
            prcp_str = row[prcp_column];
            //line_str = year_str << "/" << month_str << "/" << day_str;
            //line_str << " " << tmin_str << " " << tmax_str << " " << prcp_str << "\n";
            txtDailyData->AppendText(line_str);
            if (bucketindex != dateindex) {
                // output averaged values
                yearout = bucketindex / 12;
                monthout = bucketindex - yearout*12 + 1;

                if (ndays > 0) {
                    output.Printf("%ld-%ld %3.1lf %3.1lf avg %3.1lf absminmax %3.1lf  %3.1lf  %ld ~% ",
                       yearout, monthout, tminsum/ndays,tmaxsum/ndays,
                       tavgsum/ndays, mmin, mmax, ndays);

                    // compare average to gisstemp record if found
                    if (byearval != gissyear.year) {
                        gissyear.loadByStation(sdb, gissloc.id_, byearval, TAVG);
                    }

                    if (gisstemp.loadByMonth(sdb, gissyear.dataid, bmonthval)) {
                        output  << wxString::Format("%3.1lf \n", gisstemp.value);
                        // ensure mmax year and month records exist
                        if (mmaxyear.year != gissyear.year) {
                            if (!mmaxyear.loadByStation(sdb, gissloc.id_, byearval, MMAX)){
                                mmaxyear.dataid = 0;
                                mmaxyear.sid = gissloc.id_;
                                mmaxyear.year = byearval;
                                mmaxyear.measure = MMAX;
                                mmaxyear.valuesct = ndays;
                                mmaxyear.create(sdb);
                            }
                        }
                        mmaxtemp.dataid = mmaxyear.dataid;
                        mmaxtemp.monthid = bmonthval;
                        mmaxtemp.value = mmax;
                        mmaxtemp.dmflag = 0;
                        mmaxtemp.dsflag = 0;
                        mmaxtemp.qcflag = 0;
                        mmaxtemp.save(sdb);

                        if (tmaxyear.year != gissyear.year) {
                            if (!tmaxyear.loadByStation(sdb, gissloc.id_, byearval, TMAX)){
                                tmaxyear.dataid = 0;
                                tmaxyear.sid = gissloc.id_;
                                tmaxyear.year = byearval;
                                tmaxyear.measure = TMAX;
                                tmaxyear.valuesct = ndays;
                                tmaxyear.create(sdb);
                            }
                        }
                        tmaxtemp.dataid = tmaxyear.dataid;
                        tmaxtemp.monthid = bmonthval;
                        tmaxtemp.value = tmaxsum/ndays;
                        tmaxtemp.dmflag = 0;
                        tmaxtemp.dsflag = 0;
                        tmaxtemp.qcflag = 0;
                        tmaxtemp.save(sdb);
                    }
                    else {
                        output << "XXXX\n";
                    }

                }
                else {
                    output = "****\n";
                }
                txtDailyData->AppendText(output);
                // save mmax

                // reset count
                bucketindex = dateindex;
                byearval = yearval;
                bmonthval = monthval;
                tavgsum = tavgsqsum = tmaxsum = tminsum = tmaxsqsum = tminsqsum = 0.0;
                mmin = 99.9;
                mmax = -99.9;
                ndays = 0;
            }
            tmax_str.ToDouble(&tmax_d);
            tmax_d /= 10.0;
            if (ndays == 0 || tmax_d > mmax) {
                mmax = tmax_d;
            }
            tmin_str.ToDouble(&tmin_d);
            tmin_d /= 10.0;
            if (ndays == 0 || tmin_d < mmin) {
                mmin = tmin_d;
            }
            tavg_d = (tmax_d + tmin_d)/2.0;
            tmaxsum += tmax_d;
            tmaxsqsum += tmax_d * tmax_d;
            tminsum += tmin_d;
            tminsqsum += tmin_d * tmin_d;
            tavgsum += tavg_d;
            tavgsqsum += tavg_d*tavg_d;
            ndays += 1;
        }
    }
    if ( !gissloc.startDate_.IsValid() && !firstdate.IsEmpty()) {
        gissloc.startDate_.ParseDate(firstdate);
        gissloc.endDate_.ParseDate(date_str);
        gissloc.save(sdb);
    }

}
