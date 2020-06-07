#ifndef _H_HELPER
#define _H_HELPER

#ifndef _H_AGW_FIRST
#include "agw.h"
#endif

#include <wx/string.h>
#include <string>
#include <wx/window.h>
#include <wx/log.h>
#include <wx/filename.h>

#include <memory>

// set or return dialog control values using ID
void setEditCtrl(wxWindow* host, int ID, const wxString& value);

bool getEditCtrl(wxWindow* host, int ID, wxString& value);

bool getCheckBox(wxWindow* host, int ID, bool& value);
bool getListIndex(wxWindow* host, int ID, int& value);

bool getComboIndex(wxWindow* host, int ID, int& value);
bool getSpinCtrl(wxWindow* host, int ID, int& value);
bool getEditCtrl(wxWindow* host, int ID, std::string& value);

void ensureExtension(wxString& path, const wxString& ext);

class wxComboBox;
class wxListBox;
class wxTextCtrl;

extern void initComboBox(wxComboBox* box, wxArrayString& data);
extern void initListBox(wxListBox* box, wxArrayString& data);
int GetLongValue(wxTextCtrl* ctl);

extern std::string colorAsString(const wxColor& c);

template<class T> T* getWindowByID(wxWindow* host, int ID)
{
    T* result = dynamic_cast<T*> (host->FindWindow(ID));
    if (!result)
    {
        wxLogMessage("Failed to get control type %s for ID %d", typeid(T).name(),  ID);
        return nullptr;
    }
    return result;
}

template<class T> T* getParentByID(wxWindow* host, int ID)
{
    while(ID != host->GetId())
    {
        host = host->GetParent();
        if (host == nullptr)
            return nullptr;
    }
    return dynamic_cast<T*>(host);
}

namespace Json {
    class Value;
};

namespace agw {
    class Series;
    typedef std::shared_ptr<Series> SeriesPtr;
}

std::string shapeToString(int shapeType);
int strToPlotShape(const std::string& s);

//  extern bool readPenStyle(const Json::Value& json, const std::string& prop, wxPenStyle& style);
int penStyleToIndex(wxPenStyle s);
wxPenStyle indexToPenStyle(int ix);
int penNameToIndex(const std::string& name);
std::string indexToPenName(int ix);

void mjdn_to_year_month(double mjdn, int& year, wxDateTime::Month& month);
double to_mjdn(int year, wxDateTime::Month month);
void to_year_month(const std::string& text, int& year, wxDateTime::Month& month);


bool readPoint(const Json::Value& json, const std::string& prop, wxPoint& pt);
bool readUInt(const Json::Value& json, const std::string& prop, uint& dest);
bool readInt(const Json::Value& json, const std::string& prop, int& dest);
bool readDouble(const Json::Value& json, const std::string& prop, double& dest);
bool readBool(const Json::Value& json, const std::string& prop, bool& dest);
bool readString(const Json::Value& json, const std::string& prop, std::string& dest);
bool readColor(const Json::Value& json, const std::string& prop, wxColor& color);

agw::SeriesPtr readJSONSeries(const Json::Value& v);

class wxChoice;
void setChoice(wxChoice* ch,const wxString& value);
std::string getChoice(wxChoice* ch);
bool longValue(wxTextCtrl* txt, long& value);
bool doubleValue(wxTextCtrl* txt, double& value);
bool textValue(wxTextCtrl* txt, std::string& value);
bool wxStringValue(wxTextCtrl* txt, wxString& value);
void setDouble(wxTextCtrl* txt, double val);

#endif // _H_HELPER
