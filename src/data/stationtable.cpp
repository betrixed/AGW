
#include "stationtable.h"

LocTable::LocTable()
{
    colLabels_ = {"Name","Lat.", "Long.", "Elev.", "id", "First", "Latest"};
 /*
    colLabels_.push_back("Name");
    colLabels_.push_back("Lat.");
    colLabels_.push_back("Long.");
    colLabels_.push_back("Elev.");
    colLabels_.push_back("id");
    colLabels_.push_back()
    */
}

LocTable::~LocTable()
{
    wxGrid* myView = GetView();
    if (myView != nullptr)
    {
        myView->SetTable(nullptr);

        wxLogMessage("Remove Station Table from Grid");
    }
}

static const wxString MONTH_DATE_FMT = "%Y-%m";
wxString LocTable::displayColumn(Station4* loc, int col)
{
    wxString result;
    switch(col)
    {

   case 0:
        result << loc->name_;
        break;
   case 1:
        result << loc->lat_;
        break;
   case 2:
        result << loc->long_;
        break;
   case 3:
        result << loc->elev_;
        break;
   case 4:
        result << loc->stationid;
        break;
   case 5:
        if (loc->startDate_.IsValid())
            result = loc->startDate_.Format(MONTH_DATE_FMT);
        break;

   case 6:
        if (loc->endDate_.IsValid())
            result = loc->endDate_.Format(MONTH_DATE_FMT);
        break;
   default:
        break;
    }
    return result;
}
