    #ifndef _H_APPDATA
#define _H_APPDATA

#include "database.hpp"
#include "wx/stdpaths.h"

#include "json/json.h"
#include <list>
#include "fromlua.h"

class TestWindowApp;
class MainFrame;
class PlotFrame;
class SeriesFrame;
class PlotTable;

class DataView {
    PlotTable* pt;
    PlotFrame* pf;
    SeriesFrame* sf;
};

class CountryLayer;
typedef std::shared_ptr<CountryLayer>  CountryMapPtr;

class LocTable;
typedef std::shared_ptr<LocTable> LocTablePtr;


extern const wxString STD_PLOT_FILES;

// Singleton application data and management object, to be passed around by pointer or reference
    class AppData {
        // data about OS and user

        std::string       OSDescription_;
        wxOperatingSystemId  OSId_;

        int               vMajor_, vMinor_, vMicro_;
        bool              isLittleEndian_;
        bool              is64Bit_;
        std::string       userName_;
        std::string       userId_;
        std::string       host_;
        std::string       homeDir_;

        // file system paths

        wxString          appPath_;
        #if defined(__WXGTK__)
        wxString          prefix_;
        #endif

        wxString          userDataDir_;  // where to put database and reference files.
        wxString          startWorkDir_;  // where we started
        wxString          appDataDir_;   // read-only data
        Database          db_;          // original and main copy - can clone
        TestWindowApp*    theApp;       // The GUI boss
        MainFrame*        mFrame;       // GUI main window
        Json::Value       root_;        // shared JSON configuration data
        Json::Value        user_;        // JSON user configuration
        wxString          lastPlotDir_;        // remember plots location
        std::vector<DataView>   views_;
        std::string       mydbpath_;
        bool              ready_;
        CountryMapPtr     countryMap_;

        LuaEngine            engine_;

        void createDB(std::string& dbfilepath);
        void readConfig();
        void readUserConfig();
        void saveUserConfig();

        std::string getUserConfigPath();

        void createCountryTable();
        void setupUnixPaths();
        void setupWindowsPaths();

        static AppData* gAppData;

    public:
        AppData();
        ~AppData();
        static AppData& instance()
        {
            return *gAppData;
        }

        CountryMapPtr getCountryMap();

        const wxString& appDataDir() { return appDataDir_; }
        Database& getDB();
        void initPaths(TestWindowApp *app, MainFrame* mf);
        void openDB();
        void callOpenDB();
        void exit();
        void openPlotFile();
        bool getPlotFile(std::string& ipath);
        void readPlot(const std::string& path);
        void loadShapeFile();
        bool getReady() const { return ready_; }
        void registerView(SeriesFrame* sf);
        void registerView(PlotFrame* pf);

        void runLuaScript(const std::string& script);

        const wxString& lastPlotDir() const
        {
            return lastPlotDir_;
        }

        void  readOSInfo();

        std::string getDBPath() const { return mydbpath_; }

        void lastPlotDir(const wxString& lastDir);
        MainFrame* mainFrame() { return mFrame;}
// lua interface
    static void setup_require(lua_State *L);
    static int init_lib(lua_State* L);
    static int dbpath(lua_State* L);

    };
#define APP_LUA "app"

#endif // _H_APPDATA
