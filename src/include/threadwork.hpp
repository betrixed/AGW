#ifndef _H_THREADWORK_
#define _H_THREADWORK_

#include <wx/thread.h>
#include <wx/timer.h>

#include "globaltemp.h"

class MainFrame;

enum {
    WORKER_EVENT = wxID_HIGHEST+1,
};

class WorkThread : public wxThread
 {
 public:
    WorkThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& filePath)
        : wxThread(), m_pHandler(handler), dbPath_(dbPath), filePath_(filePath)
        {
        }

    virtual ~WorkThread();
    const std::string& dbBath() const { return dbPath_; }
    const std::string& filePath() const { return filePath_; }

    void UpdateProgress(int value);

 protected:
    // send a number from 0 to 100: -1 means cancel.  100 means complete.


    MainFrame *m_pHandler;
    std::string dbPath_;
    std::string filePath_;
 };


class IdleTimer : public wxTimer
{
protected:
    MainFrame* notify_;
public:
    IdleTimer(MainFrame* owner);
    virtual ~IdleTimer();
    virtual void Notify();
};


class TempTrendThread : public WorkThread {
public:
    // importPath is location of country shapefile


    TempTrendThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& filePath)
            : WorkThread(handler, dbPath, filePath)
        {
        }

     void doGlobalTempTrend();

     virtual ExitCode Entry() wxOVERRIDE;

     agw::GTAPtr gta;
 };

class NewDBThread : public WorkThread {
public:
// importPath is location of country shapefile
NewDBThread(MainFrame *handler,
        const std::string& dbPath,
        const std::string& filePath)
        : WorkThread(handler, dbPath, filePath)
    {
    }

    virtual ExitCode Entry() wxOVERRIDE;
};

 class ImportGissThread : public WorkThread {
 public:
    ImportGissThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& filePath)
            : WorkThread(handler, dbPath, filePath)
        {
        }

     virtual ExitCode Entry() wxOVERRIDE;
 };

 class ImportStationThread : public WorkThread {
 public:
    ImportStationThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& filePath)
        : WorkThread(handler, dbPath, filePath)
        {
        }

    virtual ExitCode Entry() wxOVERRIDE;
 };

#endif // _H_THREADWORK_

