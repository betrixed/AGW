#ifndef _H_AGW_FIRST
#define _H_AGW_FIRST

#define wxOVERRIDE override

/** README - linux build dependencies
    libspatialite
        - requires geos, proj, libfreexl
    shapelib
    jsoncpp

    Need to link in module library for wxStyledTextCtrl
**/
template <typename T>
void ignore(T &&)
{ }

#ifdef __WXMSW__
    typedef unsigned int  uint;
    #include "json/json-forwards.h"

#endif

#include <string>
#include <memory>
#include "msgexception.h"


// forward declarations
namespace agw {
    class Series;
    typedef std::shared_ptr<Series> SeriesPtr;
}


#endif // _H_AGW_FIRST
