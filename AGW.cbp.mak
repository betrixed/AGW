#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = `wx-config --cflags` -Winvalid-pch -include wx_pch.h -DWX_PRECOMP -Wall
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = `wx-config --libs`

INC_DEBUG = $(INC) -Isrc/include -Isrc/msw-include
CFLAGS_DEBUG = $(CFLAGS) -std=c++14 -g -D__WXGTK__
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)-lsqlite3 -llua -lboost_date_time -lshp -lspatialite
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/agw

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE =  -s $(LDFLAGS)
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/agw

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/data/lua_series.o $(OBJDIR_DEBUG)/src/gui/countrylistdlg.o $(OBJDIR_DEBUG)/src/gui/appdata.o $(OBJDIR_DEBUG)/src/data/wxCSV.o $(OBJDIR_DEBUG)/src/data/textlayer.o $(OBJDIR_DEBUG)/src/data/stationtable.o $(OBJDIR_DEBUG)/src/data/sqlitedb.o $(OBJDIR_DEBUG)/src/data/series.o $(OBJDIR_DEBUG)/src/data/plottable.o $(OBJDIR_DEBUG)/src/data/plotlua.o $(OBJDIR_DEBUG)/src/data/plotlayer.o $(OBJDIR_DEBUG)/src/gui/datalayerdlg.o $(OBJDIR_DEBUG)/src/data/lsqlite3.o $(OBJDIR_DEBUG)/src/data/linearscale.o $(OBJDIR_DEBUG)/src/data/legend.o $(OBJDIR_DEBUG)/src/data/jsoncpp.o $(OBJDIR_DEBUG)/src/data/geometry.o $(OBJDIR_DEBUG)/src/data/fromlua.o $(OBJDIR_DEBUG)/src/data/datalayer.o $(OBJDIR_DEBUG)/src/data/database.o $(OBJDIR_DEBUG)/src/data/autoaxis.o $(OBJDIR_DEBUG)/src/gui/plotframe.o $(OBJDIR_DEBUG)/src/gui/vegedlg.o $(OBJDIR_DEBUG)/src/gui/testwindowapp.o $(OBJDIR_DEBUG)/src/gui/stationsets.o $(OBJDIR_DEBUG)/src/gui/stationquery.o $(OBJDIR_DEBUG)/src/gui/seriesgrid.o $(OBJDIR_DEBUG)/src/gui/seriesframe.o $(OBJDIR_DEBUG)/src/gui/selectseries.o $(OBJDIR_DEBUG)/src/gui/plotxy.o $(OBJDIR_DEBUG)/src/gui/plotproperty.o $(OBJDIR_DEBUG)/src/gui/plotoptions.o $(OBJDIR_DEBUG)/src/data/app_lua.o $(OBJDIR_DEBUG)/src/gui/monthaverage.o $(OBJDIR_DEBUG)/src/gui/mapwindow.o $(OBJDIR_DEBUG)/src/gui/mapframe.o $(OBJDIR_DEBUG)/src/gui/mainframe.o $(OBJDIR_DEBUG)/src/gui/luaedit.o $(OBJDIR_DEBUG)/src/gui/loadshapedlg.o $(OBJDIR_DEBUG)/src/gui/linestylecombo.o $(OBJDIR_DEBUG)/src/gui/largeplot.o $(OBJDIR_DEBUG)/src/gui/helper.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/data/lua_series.o $(OBJDIR_RELEASE)/src/gui/countrylistdlg.o $(OBJDIR_RELEASE)/src/gui/appdata.o $(OBJDIR_RELEASE)/src/data/wxCSV.o $(OBJDIR_RELEASE)/src/data/textlayer.o $(OBJDIR_RELEASE)/src/data/stationtable.o $(OBJDIR_RELEASE)/src/data/sqlitedb.o $(OBJDIR_RELEASE)/src/data/series.o $(OBJDIR_RELEASE)/src/data/plottable.o $(OBJDIR_RELEASE)/src/data/plotlua.o $(OBJDIR_RELEASE)/src/data/plotlayer.o $(OBJDIR_RELEASE)/src/gui/datalayerdlg.o $(OBJDIR_RELEASE)/src/data/lsqlite3.o $(OBJDIR_RELEASE)/src/data/linearscale.o $(OBJDIR_RELEASE)/src/data/legend.o $(OBJDIR_RELEASE)/src/data/jsoncpp.o $(OBJDIR_RELEASE)/src/data/geometry.o $(OBJDIR_RELEASE)/src/data/fromlua.o $(OBJDIR_RELEASE)/src/data/datalayer.o $(OBJDIR_RELEASE)/src/data/database.o $(OBJDIR_RELEASE)/src/data/autoaxis.o $(OBJDIR_RELEASE)/src/gui/plotframe.o $(OBJDIR_RELEASE)/src/gui/vegedlg.o $(OBJDIR_RELEASE)/src/gui/testwindowapp.o $(OBJDIR_RELEASE)/src/gui/stationsets.o $(OBJDIR_RELEASE)/src/gui/stationquery.o $(OBJDIR_RELEASE)/src/gui/seriesgrid.o $(OBJDIR_RELEASE)/src/gui/seriesframe.o $(OBJDIR_RELEASE)/src/gui/selectseries.o $(OBJDIR_RELEASE)/src/gui/plotxy.o $(OBJDIR_RELEASE)/src/gui/plotproperty.o $(OBJDIR_RELEASE)/src/gui/plotoptions.o $(OBJDIR_RELEASE)/src/data/app_lua.o $(OBJDIR_RELEASE)/src/gui/monthaverage.o $(OBJDIR_RELEASE)/src/gui/mapwindow.o $(OBJDIR_RELEASE)/src/gui/mapframe.o $(OBJDIR_RELEASE)/src/gui/mainframe.o $(OBJDIR_RELEASE)/src/gui/luaedit.o $(OBJDIR_RELEASE)/src/gui/loadshapedlg.o $(OBJDIR_RELEASE)/src/gui/linestylecombo.o $(OBJDIR_RELEASE)/src/gui/largeplot.o $(OBJDIR_RELEASE)/src/gui/helper.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src/data || mkdir -p $(OBJDIR_DEBUG)/src/data
	test -d $(OBJDIR_DEBUG)/src/gui || mkdir -p $(OBJDIR_DEBUG)/src/gui

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/src/data/lua_series.o: src/data/lua_series.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/lua_series.cpp -o $(OBJDIR_DEBUG)/src/data/lua_series.o

$(OBJDIR_DEBUG)/src/gui/countrylistdlg.o: src/gui/countrylistdlg.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/countrylistdlg.cpp -o $(OBJDIR_DEBUG)/src/gui/countrylistdlg.o

$(OBJDIR_DEBUG)/src/gui/appdata.o: src/gui/appdata.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/appdata.cpp -o $(OBJDIR_DEBUG)/src/gui/appdata.o

$(OBJDIR_DEBUG)/src/data/wxCSV.o: src/data/wxCSV.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/wxCSV.cpp -o $(OBJDIR_DEBUG)/src/data/wxCSV.o

$(OBJDIR_DEBUG)/src/data/textlayer.o: src/data/textlayer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/textlayer.cpp -o $(OBJDIR_DEBUG)/src/data/textlayer.o

$(OBJDIR_DEBUG)/src/data/stationtable.o: src/data/stationtable.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/stationtable.cpp -o $(OBJDIR_DEBUG)/src/data/stationtable.o

$(OBJDIR_DEBUG)/src/data/sqlitedb.o: src/data/sqlitedb.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/sqlitedb.cpp -o $(OBJDIR_DEBUG)/src/data/sqlitedb.o

$(OBJDIR_DEBUG)/src/data/series.o: src/data/series.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/series.cpp -o $(OBJDIR_DEBUG)/src/data/series.o

$(OBJDIR_DEBUG)/src/data/plottable.o: src/data/plottable.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/plottable.cpp -o $(OBJDIR_DEBUG)/src/data/plottable.o

$(OBJDIR_DEBUG)/src/data/plotlua.o: src/data/plotlua.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/plotlua.cpp -o $(OBJDIR_DEBUG)/src/data/plotlua.o

$(OBJDIR_DEBUG)/src/data/plotlayer.o: src/data/plotlayer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/plotlayer.cpp -o $(OBJDIR_DEBUG)/src/data/plotlayer.o

$(OBJDIR_DEBUG)/src/gui/datalayerdlg.o: src/gui/datalayerdlg.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/datalayerdlg.cpp -o $(OBJDIR_DEBUG)/src/gui/datalayerdlg.o

$(OBJDIR_DEBUG)/src/data/lsqlite3.o: src/data/lsqlite3.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/lsqlite3.c -o $(OBJDIR_DEBUG)/src/data/lsqlite3.o

$(OBJDIR_DEBUG)/src/data/linearscale.o: src/data/linearscale.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/linearscale.cpp -o $(OBJDIR_DEBUG)/src/data/linearscale.o

$(OBJDIR_DEBUG)/src/data/legend.o: src/data/legend.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/legend.cpp -o $(OBJDIR_DEBUG)/src/data/legend.o

$(OBJDIR_DEBUG)/src/data/jsoncpp.o: src/data/jsoncpp.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/jsoncpp.cpp -o $(OBJDIR_DEBUG)/src/data/jsoncpp.o

$(OBJDIR_DEBUG)/src/data/geometry.o: src/data/geometry.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/geometry.cpp -o $(OBJDIR_DEBUG)/src/data/geometry.o

$(OBJDIR_DEBUG)/src/data/fromlua.o: src/data/fromlua.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/fromlua.cpp -o $(OBJDIR_DEBUG)/src/data/fromlua.o

$(OBJDIR_DEBUG)/src/data/datalayer.o: src/data/datalayer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/datalayer.cpp -o $(OBJDIR_DEBUG)/src/data/datalayer.o

$(OBJDIR_DEBUG)/src/data/database.o: src/data/database.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/database.cpp -o $(OBJDIR_DEBUG)/src/data/database.o

$(OBJDIR_DEBUG)/src/data/autoaxis.o: src/data/autoaxis.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/autoaxis.cpp -o $(OBJDIR_DEBUG)/src/data/autoaxis.o

$(OBJDIR_DEBUG)/src/gui/plotframe.o: src/gui/plotframe.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/plotframe.cpp -o $(OBJDIR_DEBUG)/src/gui/plotframe.o

$(OBJDIR_DEBUG)/src/gui/vegedlg.o: src/gui/vegedlg.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/vegedlg.cpp -o $(OBJDIR_DEBUG)/src/gui/vegedlg.o

$(OBJDIR_DEBUG)/src/gui/testwindowapp.o: src/gui/testwindowapp.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/testwindowapp.cpp -o $(OBJDIR_DEBUG)/src/gui/testwindowapp.o

$(OBJDIR_DEBUG)/src/gui/stationsets.o: src/gui/stationsets.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/stationsets.cpp -o $(OBJDIR_DEBUG)/src/gui/stationsets.o

$(OBJDIR_DEBUG)/src/gui/stationquery.o: src/gui/stationquery.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/stationquery.cpp -o $(OBJDIR_DEBUG)/src/gui/stationquery.o

$(OBJDIR_DEBUG)/src/gui/seriesgrid.o: src/gui/seriesgrid.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/seriesgrid.cpp -o $(OBJDIR_DEBUG)/src/gui/seriesgrid.o

$(OBJDIR_DEBUG)/src/gui/seriesframe.o: src/gui/seriesframe.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/seriesframe.cpp -o $(OBJDIR_DEBUG)/src/gui/seriesframe.o

$(OBJDIR_DEBUG)/src/gui/selectseries.o: src/gui/selectseries.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/selectseries.cpp -o $(OBJDIR_DEBUG)/src/gui/selectseries.o

$(OBJDIR_DEBUG)/src/gui/plotxy.o: src/gui/plotxy.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/plotxy.cpp -o $(OBJDIR_DEBUG)/src/gui/plotxy.o

$(OBJDIR_DEBUG)/src/gui/plotproperty.o: src/gui/plotproperty.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/plotproperty.cpp -o $(OBJDIR_DEBUG)/src/gui/plotproperty.o

$(OBJDIR_DEBUG)/src/gui/plotoptions.o: src/gui/plotoptions.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/plotoptions.cpp -o $(OBJDIR_DEBUG)/src/gui/plotoptions.o

$(OBJDIR_DEBUG)/src/data/app_lua.o: src/data/app_lua.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/data/app_lua.cpp -o $(OBJDIR_DEBUG)/src/data/app_lua.o

$(OBJDIR_DEBUG)/src/gui/monthaverage.o: src/gui/monthaverage.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/monthaverage.cpp -o $(OBJDIR_DEBUG)/src/gui/monthaverage.o

$(OBJDIR_DEBUG)/src/gui/mapwindow.o: src/gui/mapwindow.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/mapwindow.cpp -o $(OBJDIR_DEBUG)/src/gui/mapwindow.o

$(OBJDIR_DEBUG)/src/gui/mapframe.o: src/gui/mapframe.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/mapframe.cpp -o $(OBJDIR_DEBUG)/src/gui/mapframe.o

$(OBJDIR_DEBUG)/src/gui/mainframe.o: src/gui/mainframe.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/mainframe.cpp -o $(OBJDIR_DEBUG)/src/gui/mainframe.o

$(OBJDIR_DEBUG)/src/gui/luaedit.o: src/gui/luaedit.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/luaedit.cpp -o $(OBJDIR_DEBUG)/src/gui/luaedit.o

$(OBJDIR_DEBUG)/src/gui/loadshapedlg.o: src/gui/loadshapedlg.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/loadshapedlg.cpp -o $(OBJDIR_DEBUG)/src/gui/loadshapedlg.o

$(OBJDIR_DEBUG)/src/gui/linestylecombo.o: src/gui/linestylecombo.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/linestylecombo.cpp -o $(OBJDIR_DEBUG)/src/gui/linestylecombo.o

$(OBJDIR_DEBUG)/src/gui/largeplot.o: src/gui/largeplot.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/largeplot.cpp -o $(OBJDIR_DEBUG)/src/gui/largeplot.o

$(OBJDIR_DEBUG)/src/gui/helper.o: src/gui/helper.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/gui/helper.cpp -o $(OBJDIR_DEBUG)/src/gui/helper.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src/data
	rm -rf $(OBJDIR_DEBUG)/src/gui

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/src/data || mkdir -p $(OBJDIR_RELEASE)/src/data
	test -d $(OBJDIR_RELEASE)/src/gui || mkdir -p $(OBJDIR_RELEASE)/src/gui

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/src/data/lua_series.o: src/data/lua_series.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/lua_series.cpp -o $(OBJDIR_RELEASE)/src/data/lua_series.o

$(OBJDIR_RELEASE)/src/gui/countrylistdlg.o: src/gui/countrylistdlg.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/countrylistdlg.cpp -o $(OBJDIR_RELEASE)/src/gui/countrylistdlg.o

$(OBJDIR_RELEASE)/src/gui/appdata.o: src/gui/appdata.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/appdata.cpp -o $(OBJDIR_RELEASE)/src/gui/appdata.o

$(OBJDIR_RELEASE)/src/data/wxCSV.o: src/data/wxCSV.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/wxCSV.cpp -o $(OBJDIR_RELEASE)/src/data/wxCSV.o

$(OBJDIR_RELEASE)/src/data/textlayer.o: src/data/textlayer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/textlayer.cpp -o $(OBJDIR_RELEASE)/src/data/textlayer.o

$(OBJDIR_RELEASE)/src/data/stationtable.o: src/data/stationtable.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/stationtable.cpp -o $(OBJDIR_RELEASE)/src/data/stationtable.o

$(OBJDIR_RELEASE)/src/data/sqlitedb.o: src/data/sqlitedb.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/sqlitedb.cpp -o $(OBJDIR_RELEASE)/src/data/sqlitedb.o

$(OBJDIR_RELEASE)/src/data/series.o: src/data/series.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/series.cpp -o $(OBJDIR_RELEASE)/src/data/series.o

$(OBJDIR_RELEASE)/src/data/plottable.o: src/data/plottable.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/plottable.cpp -o $(OBJDIR_RELEASE)/src/data/plottable.o

$(OBJDIR_RELEASE)/src/data/plotlua.o: src/data/plotlua.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/plotlua.cpp -o $(OBJDIR_RELEASE)/src/data/plotlua.o

$(OBJDIR_RELEASE)/src/data/plotlayer.o: src/data/plotlayer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/plotlayer.cpp -o $(OBJDIR_RELEASE)/src/data/plotlayer.o

$(OBJDIR_RELEASE)/src/gui/datalayerdlg.o: src/gui/datalayerdlg.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/datalayerdlg.cpp -o $(OBJDIR_RELEASE)/src/gui/datalayerdlg.o

$(OBJDIR_RELEASE)/src/data/lsqlite3.o: src/data/lsqlite3.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/lsqlite3.c -o $(OBJDIR_RELEASE)/src/data/lsqlite3.o

$(OBJDIR_RELEASE)/src/data/linearscale.o: src/data/linearscale.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/linearscale.cpp -o $(OBJDIR_RELEASE)/src/data/linearscale.o

$(OBJDIR_RELEASE)/src/data/legend.o: src/data/legend.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/legend.cpp -o $(OBJDIR_RELEASE)/src/data/legend.o

$(OBJDIR_RELEASE)/src/data/jsoncpp.o: src/data/jsoncpp.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/jsoncpp.cpp -o $(OBJDIR_RELEASE)/src/data/jsoncpp.o

$(OBJDIR_RELEASE)/src/data/geometry.o: src/data/geometry.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/geometry.cpp -o $(OBJDIR_RELEASE)/src/data/geometry.o

$(OBJDIR_RELEASE)/src/data/fromlua.o: src/data/fromlua.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/fromlua.cpp -o $(OBJDIR_RELEASE)/src/data/fromlua.o

$(OBJDIR_RELEASE)/src/data/datalayer.o: src/data/datalayer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/datalayer.cpp -o $(OBJDIR_RELEASE)/src/data/datalayer.o

$(OBJDIR_RELEASE)/src/data/database.o: src/data/database.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/database.cpp -o $(OBJDIR_RELEASE)/src/data/database.o

$(OBJDIR_RELEASE)/src/data/autoaxis.o: src/data/autoaxis.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/autoaxis.cpp -o $(OBJDIR_RELEASE)/src/data/autoaxis.o

$(OBJDIR_RELEASE)/src/gui/plotframe.o: src/gui/plotframe.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/plotframe.cpp -o $(OBJDIR_RELEASE)/src/gui/plotframe.o

$(OBJDIR_RELEASE)/src/gui/vegedlg.o: src/gui/vegedlg.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/vegedlg.cpp -o $(OBJDIR_RELEASE)/src/gui/vegedlg.o

$(OBJDIR_RELEASE)/src/gui/testwindowapp.o: src/gui/testwindowapp.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/testwindowapp.cpp -o $(OBJDIR_RELEASE)/src/gui/testwindowapp.o

$(OBJDIR_RELEASE)/src/gui/stationsets.o: src/gui/stationsets.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/stationsets.cpp -o $(OBJDIR_RELEASE)/src/gui/stationsets.o

$(OBJDIR_RELEASE)/src/gui/stationquery.o: src/gui/stationquery.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/stationquery.cpp -o $(OBJDIR_RELEASE)/src/gui/stationquery.o

$(OBJDIR_RELEASE)/src/gui/seriesgrid.o: src/gui/seriesgrid.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/seriesgrid.cpp -o $(OBJDIR_RELEASE)/src/gui/seriesgrid.o

$(OBJDIR_RELEASE)/src/gui/seriesframe.o: src/gui/seriesframe.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/seriesframe.cpp -o $(OBJDIR_RELEASE)/src/gui/seriesframe.o

$(OBJDIR_RELEASE)/src/gui/selectseries.o: src/gui/selectseries.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/selectseries.cpp -o $(OBJDIR_RELEASE)/src/gui/selectseries.o

$(OBJDIR_RELEASE)/src/gui/plotxy.o: src/gui/plotxy.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/plotxy.cpp -o $(OBJDIR_RELEASE)/src/gui/plotxy.o

$(OBJDIR_RELEASE)/src/gui/plotproperty.o: src/gui/plotproperty.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/plotproperty.cpp -o $(OBJDIR_RELEASE)/src/gui/plotproperty.o

$(OBJDIR_RELEASE)/src/gui/plotoptions.o: src/gui/plotoptions.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/plotoptions.cpp -o $(OBJDIR_RELEASE)/src/gui/plotoptions.o

$(OBJDIR_RELEASE)/src/data/app_lua.o: src/data/app_lua.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/data/app_lua.cpp -o $(OBJDIR_RELEASE)/src/data/app_lua.o

$(OBJDIR_RELEASE)/src/gui/monthaverage.o: src/gui/monthaverage.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/monthaverage.cpp -o $(OBJDIR_RELEASE)/src/gui/monthaverage.o

$(OBJDIR_RELEASE)/src/gui/mapwindow.o: src/gui/mapwindow.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/mapwindow.cpp -o $(OBJDIR_RELEASE)/src/gui/mapwindow.o

$(OBJDIR_RELEASE)/src/gui/mapframe.o: src/gui/mapframe.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/mapframe.cpp -o $(OBJDIR_RELEASE)/src/gui/mapframe.o

$(OBJDIR_RELEASE)/src/gui/mainframe.o: src/gui/mainframe.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/mainframe.cpp -o $(OBJDIR_RELEASE)/src/gui/mainframe.o

$(OBJDIR_RELEASE)/src/gui/luaedit.o: src/gui/luaedit.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/luaedit.cpp -o $(OBJDIR_RELEASE)/src/gui/luaedit.o

$(OBJDIR_RELEASE)/src/gui/loadshapedlg.o: src/gui/loadshapedlg.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/loadshapedlg.cpp -o $(OBJDIR_RELEASE)/src/gui/loadshapedlg.o

$(OBJDIR_RELEASE)/src/gui/linestylecombo.o: src/gui/linestylecombo.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/linestylecombo.cpp -o $(OBJDIR_RELEASE)/src/gui/linestylecombo.o

$(OBJDIR_RELEASE)/src/gui/largeplot.o: src/gui/largeplot.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/largeplot.cpp -o $(OBJDIR_RELEASE)/src/gui/largeplot.o

$(OBJDIR_RELEASE)/src/gui/helper.o: src/gui/helper.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/gui/helper.cpp -o $(OBJDIR_RELEASE)/src/gui/helper.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/src/data
	rm -rf $(OBJDIR_RELEASE)/src/gui

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

