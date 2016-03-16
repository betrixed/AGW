CXX=$(shell wx-config --cxx) -std=gnu++14
CC=$(shell wx-config --cc)

PREFIX=$(shell wx-config --prefix)
RESXX=$(shell wx-config --rescomp)
CPPFLAGS=$(shell wx-config --cppflags) -I../src/include -I../src/msw-include -I$(PREFIX)/include
CFLAGS=$(shell wx-config --cflags) -I../src/include -I../src/msw-include -I$(PREFIX)/include

BOOST=mgw53-mt-1_60.a
LIBFIX=$(PREFIX)/lib

LDFLAGS=$(shell wx-config --libs) -lsqlite3 -lspatialite -lshp -llua  $(LIBFIX)/libwxscintilla-3.1.a  $(LIBFIX)/libboost_date_time-$(BOOST)

GUI_SRC=$(wildcard ../src/gui/*.cpp)
GUI_OBJ=$(patsubst ../src/gui/%.cpp, %.o, $(GUI_SRC))

LC_SRC=$(wildcard ../src/data/*.c)
LC_OBJ=$(patsubst ../src/data/%.c, %.o, $(LC_SRC))

DATA_SRC=$(wildcard ../src/data/*.cpp)
DATA_OBJ=$(patsubst ../src/data/%.cpp, %.o, $(DATA_SRC))

VPATH = ../src/gui:../src/data

agw.exe:agw.o $(GUI_OBJ) $(DATA_OBJ) $(LC_OBJ)
	$(CXX) -o agw agw.o $(GUI_OBJ) $(DATA_OBJ) $(LC_OBJ) $(LDFLAGS)

agw.o:../AGW-0.2.rc
	$(RESXX) $< -o $@ 

-include $(GUI_OBJ:.o=.d)

-include $(DATA_OBJ:.o=.d)

-include $(LC_OBJ:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
	$(CC) -MM $(CFLAGS) -c $< > $*.d


%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $<
	$(CXX) -MM $(CPPFLAGS)  -c $< > $*.d

.PHONY : clean install list

list:
	echo $(LC_SRC)
	echo $(LC_OBJ)

clean:
	-rm *.o *.d
	-rm -f agw.exe

install:
	cp agw.exe $(PREFIX)/bin/