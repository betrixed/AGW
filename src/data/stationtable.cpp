
#include "stationtable.h"

LocTable::LocTable()
{
    colLabels_.push_back("Name");
    colLabels_.push_back("Pop.");
    colLabels_.push_back("Class");
    colLabels_.push_back("Lights");
    colLabels_.push_back("Vege.");
    colLabels_.push_back("Lat.");
    colLabels_.push_back("Long.");
    colLabels_.push_back("Elev1.");
    colLabels_.push_back("Elev2.");
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

std::string LocTable::displayColumn(GissLocation* loc, int col)
{
    switch(col)
    {

   case 0:
        return loc->name_;
   case 1:
        return std::to_string(loc->popSize_);
   case 2:
        return std::to_string(loc->popClass_);
   case 3:
        return std::to_string(loc->popLights_);
   case 4:
        return loc->vegetation_;
   case 5:
        return std::to_string(loc->lat_);
   case 6:
        return std::to_string(loc->long_);
   case 7:
        return std::to_string(loc->elevation1_);
   case 8:
        return std::to_string(loc->elevation2_);
   case 9:
        return std::to_string(loc->id);
   default:
        return "";
    }
}
