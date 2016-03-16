#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "json/json.h"

#include "plottable.h"

using namespace agw;

JsonStore::JsonStore(const std::string& name, const Json::Value& root)
{
    name_ = name;
    setJson(root);
}

void JsonStore::setJson(const Json::Value& root)
{
    std::stringstream ss;

    ss << root;

    if (root.isMember("jype"))
    {
        this->jype_ = root["jype"].asString();
    }
    else {
        this->jype_ = "";
    }
    this->json_ = ss.str();
}

DataTableColumn::DataTableColumn(SeriesPtr p) : data_(p), colWidth_(-1), colDec_(-1), colFormat_(-1)
{
    this->label_ = p->getLabel();
}

DataTable::DataTable(std::vector<PlotPtr>& pdata, const Json::Value& graph)
{
    graph_ = graph;

    auto it = pdata.begin();
    auto fin = pdata.end();
    maxRows_ = 0;
    for( ; it != fin; it++)
    {
        PlotLayer* pl = it->get();
        DataLayer* dlay = dynamic_cast<DataLayer*>(pl);
        if (dlay)
        {
            if (dlay->xdata_ != nullptr)
            {
                SeriesPtr dp = dlay->xdata_;
                addColumn(dp);
            }
            if (dlay->ydata_ != nullptr)
            {
                SeriesPtr dp = dlay->ydata_;
                addColumn(dp);
            }
            if (dlay->errorbar_ != nullptr)
            {
                SeriesPtr dp = dlay->errorbar_;
                addColumn(dp);
            }
        }
    }
}

DataTable::~DataTable()
{
}

void DataTable::addColumn(SeriesPtr& sp)
{
    ColnPtr cp(new DataTableColumn(sp));
    series_.push_back(std::move(cp));
    Series* s = sp.get();
    if (s->size() > maxRows_)
        maxRows_ = s->size();
}


int DataTable::GetNumberRows()
{
    return maxRows_;
}

int  DataTable::GetNumberCols ()
{
    return series_.size();
}

bool DataTable::IsEmptyCell( int row, int col )
{
    if (row < 0 || row >= (int)maxRows_)
        return true;
    if (col < 0 || col >= (int)series_.size())
        return true;

    DataTableColumn* cp = series_[col].get();
    if (row >= (int) cp->get()->size())
        return true;
    return false;
}


wxString DataTable::GetColLabelValue (int col)
{
   if (col < 0 || col >= (int)series_.size())
        return "";
    DataTableColumn* cp = series_[col].get();
    return cp->label();

}

void DataTable::SetValue( int row, int col, const wxString& value )
{
}


wxString DataTable::GetValue (int row, int col)
{
    if (row < 0 || row >= (int)maxRows_)
        return wxEmptyString;
    if (col < 0 || col >= (int) series_.size())
        return wxEmptyString;

    DataTableColumn* cp = series_[col].get();
    Series* s = cp->get();
    if (row >= (int) s->size())
        return wxEmptyString;
    double value = (*s)[row];
    wxString result;
    result << value;
    return result;
}
