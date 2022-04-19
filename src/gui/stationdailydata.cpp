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
    EVT_BUTTON( ID_SHOW_NOW, StationDailyData::OnShowNowClick )
    EVT_BUTTON( ID_CALCULATE, StationDailyData::OnCalculateClick )
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
    txShowYear = NULL;
    txShowMonth = NULL;
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

    txShowYear = new wxTextCtrl( itemFrame1, ID_SHOW_YEAR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(txShowYear, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemFrame1, wxID_STATIC, _("Month"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString txShowMonthStrings;
    txShowMonthStrings.Add(_("1"));
    txShowMonthStrings.Add(_("2"));
    txShowMonthStrings.Add(_("3"));
    txShowMonthStrings.Add(_("4"));
    txShowMonthStrings.Add(_("5"));
    txShowMonthStrings.Add(_("6"));
    txShowMonthStrings.Add(_("7"));
    txShowMonthStrings.Add(_("8"));
    txShowMonthStrings.Add(_("9"));
    txShowMonthStrings.Add(_("10"));
    txShowMonthStrings.Add(_("11"));
    txShowMonthStrings.Add(_("12"));
    txShowMonth = new wxChoice( itemFrame1, ID_SHOW_MONTH, wxDefaultPosition, wxDefaultSize, txShowMonthStrings, 0 );
    txShowMonth->SetStringSelection(_("1"));
    itemBoxSizer2->Add(txShowMonth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemFrame1, ID_SHOW_NOW, _("Show"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton1 = new wxButton( itemFrame1, ID_CALCULATE, _("Calculate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton1->SetName(wxT("btnCalculate"));
    itemBoxSizer2->Add(itemButton1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

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
    return wxString::Format("%s/%s.dly", path.c_str(), stationid);
}

// return unquoted character if required
/* static wxString unquote(const wxString& value) {
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
*/

class MonthStat {
public:
    int32_t byearval;
    int32_t bmonthval;
    double tmax_d, tmin_d, tavg_d;
    double mmax, mmin;
    long ndays;
    long bucketindex;
    double tmaxsum, tminsum, tavgsum;

    // station id
    sqlite3_int64   sid;


    sqlite3_int64 tmax_year_id;
    sqlite3_int64 tmin_year_id;
    sqlite3_int64 mmax_year_id;
    sqlite3_int64 mmin_year_id;
    int32_t       rec_year;

    MonthStat() : ndays(0), bucketindex(0), rec_year(0)
    {
    }

    void set_sid(sqlite3_int64 id) {
        this->sid = id;
    }

    bool dirty() {
        return (ndays > 0);
    }

    void setmonth(int32_t year, int32_t month) {
        byearval = year;
        bmonthval = month;

        bucketindex = year*12 + (month-1);
        ndays = 0;
        tmaxsum = 0.0;
        tminsum = 0.0;
        tavgsum = 0.0;

        if (byearval != rec_year) {
            rec_year = 0;
        }

    }


    long index() {
        return bucketindex;
    }


    sqlite3_int64 get_year(SqliteDB& sdb, int code) {
        GissYear year;

        if (!year.loadByStation(sdb, sid, byearval, code)){
            year.dataid = 0;
            year.sid = sid;
            year.year = byearval;
            year.measure = code;
            year.valuesct = 0;
            year.create(sdb);
        }
        return year.dataid;
    }

    void text_stat(wxString& data) {
        if (ndays > 0) {
            data.Printf("%4d-%02d:  %4.1f %4.1f %4.1f\n", byearval, bmonthval, tminsum/ndays, tmaxsum/ndays, tavgsum/ndays);
        }
        else {
            data.Printf("*** no data ***");
        }
    }
    void save_stat(SqliteDB& sdb, sqlite3_int64 yid, double value)
    {
        MonthTemp temp;

        temp.dataid = yid;
        temp.monthid = bmonthval;
        temp.value = value;
        temp.dmflag = 0;
        temp.dsflag = 0;
        temp.qcflag = 0;
        temp.save(sdb);
    }

    void save(SqliteDB& sdb) {
        if (ndays == 0) {
            return;
        }
        tmax_year_id = get_year(sdb, TMAX);
        tmin_year_id = get_year(sdb, TMIN);
        mmax_year_id = get_year(sdb, MMAX);
        mmin_year_id = get_year(sdb, MMIN);
        rec_year = byearval;

        save_stat(sdb, tmin_year_id, tminsum / ndays);
        save_stat(sdb, tmax_year_id, tmaxsum / ndays);
        save_stat(sdb, mmin_year_id, mmin);
        save_stat(sdb, mmax_year_id, mmax);

        ndays = 0;
    }

    void addTemp(double tmin_d, double tmax_d)
    {
        if (ndays == 0 || tmax_d > mmax) {
            mmax = tmax_d;
        }
        if (ndays == 0 || tmin_d < mmin) {
            mmin = tmin_d;
        }
        tavg_d = (tmax_d + tmin_d)/2.0;
        tmaxsum += tmax_d;

        tminsum += tmin_d;

        tavgsum += tavg_d;

        ndays += 1;
    }
    void add(const wxString& smin, const wxString& smax)
    {
        if (smin.size()==0 || smax.size()==0) {
            return; // need both
        }
        smax.ToDouble(&tmax_d);
        tmax_d /= 10.0;
        smin.ToDouble(&tmin_d);
        tmin_d /= 10.0;

        addTemp(tmin_d, tmax_d);

    }

};



bool skipNChar(wxFileInputStream& rdr, int ct) {
    for(int i = 0; i < ct; i++) {
        rdr.GetC();
        if (rdr.Eof()) {
            return false;
        }
    }
    return true;
}

bool readText(wxFileInputStream& rdr, wxString& data, int len)
{
   data.Truncate(0);

   for(int i = 0; i < len; i++) {
        wxUniChar val(rdr.GetC());
        if (rdr.Eof()) {
            return false;
        }
        data.Append(val);
    }
    return true;

}
bool readYear(wxFileInputStream& rdr, long& year) {
    wxString data;

    for(int i = 0; i < 4; i++) {
        wxUniChar val(rdr.GetC());
        if (rdr.Eof()) {
            return false;
        }
        data.Append(val);
    }
    data.ToCLong(&year);
    return true;
}

bool readMonth(wxFileInputStream& rdr, long& month) {
    wxString data;

    for(int i = 0; i < 2; i++) {
        wxUniChar val(rdr.GetC());
        if (rdr.Eof()) {
            return false;
        }
        data.Append(val);
    }
    data.ToCLong(&month);
    return true;
}

bool readValue(wxFileInputStream& rdr, long& value) {
    wxString data;

    for(int i = 0; i < 5; i++) {
        wxUniChar val(rdr.GetC());
        if (rdr.Eof()) {
            return false;
        }
        data.Append(val);
    }
    data.ToCLong(&value);
    return true;
}

void StationDailyData::Calculate()
{
    wxString filepath = StationDailyData::FullPath(stationId);
    const wxString StrTMAX = "TMAX";
    const wxString StrTMIN = "TMIN";
    const int fixed_len = 270; // include LF

    long year, month;
    wxString measure;
    long     tmax[32];
    long     tmin[32];

    tmax[0] = 0;
    tmin[0] = 0;

    int firstDay, lastDay;
    int firstMonth, firstYear;
    int lastMonth, lastYear;

    MonthStat   mstat;

    firstMonth = 0;
    firstYear = 0;

    wxFileInputStream rdr(filepath);
    //double fileLength = input_stream.GetLength();
    if (!rdr.IsOk())
    {
        wxLogError("Cannot open file '%s'.", filepath.utf8_str());
        return;
    }

    // at beginning of fixed format text file.
    // read the station id 3+8 = 11
    // year4 and month2  4+2
    // read the measurement type - TMAX, TMIN, PRCP - 4
    // read the value# (5) - flags# (3) for each day of the month
    // 270 characters each line.

    std::string stationid(stationId.utf8_str());

    SqliteDB& sdb = ap_->getDB();

    Station4 gissloc;
    gissloc.loadByCode(sdb,stationid);
    mstat.set_sid(gissloc.id_);

    while (!rdr.Eof()) {
        if(skipNChar(rdr,11)) {

            readYear(rdr,year); // 15
            readMonth(rdr,month); // 17
            readText(rdr,measure,4); // 21

            if (measure.IsSameAs(StrTMAX)) {
                tmax[0] = year*12+month-1;

                for(int day = 1; day <= 31; day++)
                {
                    readValue(rdr, tmax[day]);
                    skipNChar(rdr,3);
                }
                skipNChar(rdr,1);
            }
            else if  (measure.IsSameAs(StrTMIN)) {
                tmin[0] = year*12+month-1;

                for(int day = 1; day <= 31; day++) //31*8 = 248 + 21 = 269
                {
                    readValue(rdr, tmin[day]);
                    skipNChar(rdr,3);
                }
                skipNChar(rdr,1);
            }
            else {
                skipNChar(rdr,fixed_len - 21);
            }
            if (tmax[0] && tmin[0] && (tmax[0] == tmin[0])) {
                mstat.setmonth(year,month);

                tmax[0] = 0;
                tmin[0] = 0;
                firstDay = 0;
                lastDay = 0;

                for(int day = 1; day <= 31; day++)
                {

                    if (tmax[day] != -9999 && tmin[day] != -9999 )
                    {
                        if (!firstDay) {
                            firstDay = day;
                        }
                        lastDay = day;
                        mstat.addTemp(tmin[day]/10.0, tmax[day]/10.0);
                    }
                }
                if (firstDay && lastDay)
                {
                    if (!firstYear) {
                        firstYear = year;
                        firstMonth = month;
                    }
                    lastYear = year;
                    lastMonth = month;
                }
                mstat.save(sdb);

            }

        }
    }

    if ( firstYear ) {

        gissloc.startDate_ = wxDateTime(firstDay, wxDateTime::Month(firstMonth-1), firstYear);
        gissloc.endDate_ = wxDateTime(lastDay, wxDateTime::Month(lastMonth-1), lastYear);
        gissloc.save(sdb);
        wxString msg;

        msg << "From " << gissloc.startDate_.FormatDate() << " to " <<  gissloc.endDate_.FormatDate() << "\n";
        txtDailyData->AppendText(msg);
    }
}
//Feed some extra statistics into the GissYear, GissTemp sections in a
//single pass through a daily station text file
void StationDailyData::SetStationId(const wxString& name, const wxString& filepath)
{
    stationId = name;
    wxString title;
    title << name << " TMIN TMAX MMIN MMAX from daily data";
    SetTitle(title);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SHOW_NOW
 */

void StationDailyData::OnShowNowClick( wxCommandEvent& event )
{
    // read from database, not dly

}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CALCULATE
 */

void StationDailyData::OnCalculateClick( wxCommandEvent& event )
{
    event.Skip();
    Calculate();
}

