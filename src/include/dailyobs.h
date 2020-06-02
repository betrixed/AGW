#ifndef DAILYOBS_H
#define DAILYOBS_H

#include "database.hpp"

class DailyObs
{
    public:


        DailyObs();
        virtual ~DailyObs();
        static bool createTable(SqliteDB &sdb);

            DBRowId id;


    protected:



    private:

    public:
    DBRowId sid;
    wxDateTime obsdate;
    double  tmax;
    double  tmin;
    double  pptn;
};

#endif // DAILYOBS_H
