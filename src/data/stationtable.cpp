
#include "stationtable.h"

LocTable::LocTable()
{
    colLabels_.push_back("Name");
    colLabels_.push_back("Lat.");
    colLabels_.push_back("Long.");
    colLabels_.push_back("Elev.");
    colLabels_.push_back("id");
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

std::string LocTable::displayColumn(Station4* loc, int col)
{
    switch(col)
    {

   case 0:
        return loc->name_;
   case 1:
        return std::to_string(loc->lat_);
   case 2:
        return std::to_string(loc->long_);
   case 3:
        return std::to_string(loc->elev_);
   case 4:
        return loc->stationid;
   default:
        return "";
    }
}
