#ifndef H_STATION_TABLE
#define H_STATION_TABLE

#include <wx/grid.h>
#include <wx/log.h>

#include "database.hpp"

typedef std::vector<std::unique_ptr<GissLocation>> LocList;

class LocTable : public wxGridTableBase {
    LocList table_;
    std::vector<std::string> colLabels_;

public:

    const LocList& data() const { return table_; }

    LocTable();
    virtual ~LocTable();

    static std::string displayColumn(GissLocation* loc, int col);


    virtual int 	GetNumberRows ()
    {
        return table_.size();
    }

    virtual int 	GetNumberCols ()
    {
        return 10;
    }
    virtual bool IsEmptyCell( int row, int col )
    {
        if ((row < 0) || (row >= (int) table_.size()))
            return false;
        if ((col < 0) || ( col >= 10))
            return false;
        return true;
    }

    virtual wxString GetColLabelValue (int col)
    {
        if ((col >= 0) && (col < (int)colLabels_.size()))
        {
            return colLabels_[col];
        }
        else
            return "";
    }
    virtual void SetValue( int row, int col, const wxString& value )
    {
        // not allowed
    }

    GissLocation* getRecord(int row)
    {
        return table_[row].get();
    }

    void addRecord(GissLocation* loc)
    {
        table_.push_back(std::move(std::unique_ptr<GissLocation>(loc)));
    }
    virtual wxString GetValue (int row, int col)
    {
        if (row < 0 || row >= (int)table_.size())
            return "";
        GissLocation* loc = table_[row].get();
        return displayColumn(loc,col);
    }
};

#endif