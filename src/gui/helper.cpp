#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstopcd
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/spinctrl.h>
#include <wx/combo.h>
#include <wx/listbox.h>

#include "helper.h"
#include "json/json.h"
#include "series.hpp"
#include "plotxy.h"
#include "series_helper.h"
#include <fstream>
#include <exception>

using namespace agw;

 void setEditCtrl(wxWindow* host, int ID, const wxString& value)
{
    wxTextCtrl* text = dynamic_cast<wxTextCtrl*>(host->FindWindow(ID));
    if (text != nullptr)
    {
        text->SetValue(value);
    }
}

void ensureExtension(wxString& path, const wxString& ext)
{
    wxFileName fn;

    fn.Assign(path);

    if (fn.GetExt() != ext)
    {
        fn.SetExt(ext);
        path = fn.GetFullPath();
    }
}

void initComboBox(wxComboBox* box, wxArrayString& data)
{
  size_t i;
  for (i = 0; i < data.size(); i++)
    {
      // setting PK Fields
      wxString& str = data[i];
      box->Append(str);
   }
   box->Enable(true);
   box->SetSelection(wxNOT_FOUND);

}
void initListBox(wxListBox* box, wxArrayString& data)
{
  size_t i;
  for (i = 0; i < data.size(); i++)
    {
      // setting PK Fields
      wxString& str = data[i];
      box->Append(str);
   }
   box->Enable(true);
   box->SetSelection(wxNOT_FOUND);

}

 bool getEditCtrl(wxWindow* host, int ID, wxString& value)
{
    wxTextCtrl* text = dynamic_cast<wxTextCtrl*>(host->FindWindow(ID));
    if (text != nullptr)
    {
        value = text->GetValue();
        return true;
    }
    return false;
}

 bool getCheckBox(wxWindow* host, int ID, bool& value)
{
    wxCheckBox* c = dynamic_cast<wxCheckBox*>(host->FindWindow(ID));
    if (c != nullptr)
    {
        value = c->IsChecked();
        return true;
    }
    return false;

}
 bool getListIndex(wxWindow* host, int ID, int& value)
{
    wxListBox* c = dynamic_cast<wxListBox*>(host->FindWindow(ID));
    if (c != nullptr)
    {
        value = c->GetSelection();
        return true;
    }
    return false;
}

 bool getComboIndex(wxWindow* host, int ID, int& value)
{
    wxComboBox* c = dynamic_cast<wxComboBox*>(host->FindWindow(ID));
    if (c != nullptr)
    {
        value = c->GetSelection();
        return true;
    }
    return false;

}
 bool getSpinCtrl(wxWindow* host, int ID, int& value)
{
    wxSpinCtrl* c = dynamic_cast<wxSpinCtrl*>(host->FindWindow(ID));
    if (c != nullptr)
    {
        value = c->GetValue();
        return true;
    }
    return false;

}

 bool getEditCtrl(wxWindow* host, int ID, std::string& value)
{
   wxString s;
   if (getEditCtrl(host,ID, s))
   {
        value = s.ToStdString();
        return true;
   }
   return false;
}

std::string colorAsString(const wxColor& c)
{
    return c.GetAsString().ToStdString();
}


bool readUInt(const Json::Value& json, const std::string& prop, uint& dest)
{
    if(json.isMember(prop))
    {
        dest = json[prop].asUInt();
        return true;
    }
    return false;
}

bool readPoint(const Json::Value& json, const std::string& prop, wxPoint& pt)
{
    if (json.isMember(prop))
    {
        const Json::Value& tup = json[prop];
        wxASSERT_MSG(tup.size()==2,"Json read not array size of 2");
        pt.x = tup[0].asInt();
        pt.y = tup[1].asInt();
        return true;
    }
    return false;
}

bool readInt(const Json::Value& json, const std::string& prop, int& dest)
{
    if(json.isMember(prop))
    {
        dest = json[prop].asInt();
        return true;
    }
    return false;
}


bool readDouble(const Json::Value& json, const std::string& prop, double& dest)
{
    if(json.isMember(prop))
    {
        dest = json[prop].asDouble();
        return true;
    }
    return false;
}

bool readBool(const Json::Value& json, const std::string& prop, bool& dest)
{
    if(json.isMember(prop))
    {
        dest = json[prop].asBool();
        return true;
    }
    return false;
}

bool readString(const Json::Value& json, const std::string& prop, std::string& dest)
{
    if(json.isMember(prop))
    {
        dest = json[prop].asString();
        return true;
    }
    return false;
}

bool readColor(const Json::Value& json, const std::string& prop, wxColor& color)
{
    if(json.isMember(prop))
    {
        wxString tempStr = json[prop].asString();
        color.Set(tempStr);
        return true;
    }
    return false;
}

SeriesPtr seriesFromFile(const char* path)
{
    SeriesPtr result;
    if (path == nullptr)
        throw MsgException("nil path");
    std::ifstream jstream(path);
    if (jstream.is_open())
    {
        Json::Reader  jsonrdr;
        Json::Value   jser;
        if (!jsonrdr.parse(jstream,jser))
        {
            throw MsgException("Json parse error");
        }
        result = readJSONSeries(jser);
    }
    return result;
}

SeriesPtr readJSONSeries(const Json::Value& s)
{
    SeriesPtr result;
    if (s.isMember("jype"))
    {
        const std::string& hype = s["jype"].asString();
        if (hype=="float")
        {
            result = std::make_shared<FloatSeries>();
        }
        else if (hype=="double")
        {
            result = std::make_shared<DoubleSeries>();
        }
        else if (hype=="line")
        {
            result = std::make_shared<LineSeries>();
        }
        if (result.get())
        {
            result->ReadJSON(s);
        }
    }
    return result;
}



int strToPlotShape(const std::string& s)
{
    if (s == "cross")
    {
        return PlotShape::CROSS;
    }
    else if (s == "circle")
    {
        return PlotShape::CIRCLE;
    }
    else if (s == "plus")
    {
        return PlotShape::PLUS;
    }
    else if (s == "square")
    {
        return PlotShape::SQUARE;
    }
    else if (s == "triangle")
    {
        return PlotShape::TRIANGLE;
    }
    else {
        return PlotShape::NONE;
    }
}

std::string shapeToString(int shapeType)
{
    switch(shapeType)
    {
        case PlotShape::NONE:
            return "none";
        case PlotShape::CROSS:
            return "cross";
        case PlotShape::CIRCLE:
            return "circle";
        case PlotShape::TRIANGLE:
            return "triangle";
        case PlotShape::SQUARE:
            return "square";
        case PlotShape::PLUS:
        default:
            return "plus";
    }
}

bool readPenStyle(const Json::Value& v, const std::string& s, wxPenStyle& style)
{
    int result;
    if (!readInt(v, s, result))
    {
        return false;
    }
    style = (wxPenStyle) result;
    return true;
}

static std::string penstyle_name[12] =
    {
        "solid",
        "transparent",
        "dot",
        "long_dash",
        "short_dash",
        "dot_dash",
        "b_diag_hatch",
        "cross_diag_hatch",
        "f_diag_hatch",
        "cross_hatch",
        "horiz_hatch",
        "vert_hatch"
    };

static wxPenStyle penstyle_lookup[12] =
    {
        wxPENSTYLE_SOLID,
        wxPENSTYLE_TRANSPARENT,
        wxPENSTYLE_DOT,
        wxPENSTYLE_LONG_DASH,
        wxPENSTYLE_SHORT_DASH,
        wxPENSTYLE_DOT_DASH,
        wxPENSTYLE_BDIAGONAL_HATCH,
        wxPENSTYLE_CROSSDIAG_HATCH,
        wxPENSTYLE_FDIAGONAL_HATCH,
        wxPENSTYLE_CROSS_HATCH,
        wxPENSTYLE_HORIZONTAL_HATCH,
        wxPENSTYLE_VERTICAL_HATCH
    };

// presume ignorance on combobox pen style selection index and style values
 int penStyleToIndex(wxPenStyle s)
 {
    for(int i = 0; i < 12; i++ )
    {
        if (penstyle_lookup[i] == s)
            return i;
    }
    return 0;
 }

 wxPenStyle indexToPenStyle(int ix)
 {

     if (ix < 0 || ix >= 12)
        return penstyle_lookup[0];
    else
        return penstyle_lookup[ix];
 }

int
penNameToIndex(const std::string& s)
{
   for(int i = 0; i < 12; i++ )
    {
        if (penstyle_name[i] == s)
            return i;
    }
    return 0;

}

std::string indexToPenName(int ix)
{
     if (ix < 0 || ix >= 12)
        return penstyle_name[0];
    else
        return penstyle_name[ix];
}


 bool wxStringValue(wxTextCtrl* txt, wxString& value)
{
   value = txt->GetValue();
   value.Trim();
   return (value.size() > 0);
}
 bool textValue(wxTextCtrl* txt, std::string& value)
{
    wxString temp;
    if (wxStringValue(txt,temp))
    {
        value = temp.ToStdString();
        return true;
    }
    return false;
}

 bool doubleValue(wxTextCtrl* txt, double& value)
{
    wxString test;
    if (!wxStringValue(txt,test))
        return false;
    return test.ToDouble(&value);
}

 bool longValue(wxTextCtrl* txt, long& value)
{
    wxString test;
    if (!wxStringValue(txt,test))
        return false;
    return test.ToLong(&value);
}

 std::string getChoice(wxChoice* ch)
{
    int sel = ch->GetSelection();
    if (sel == wxNOT_FOUND)
        sel = 0;
    return ch->GetString(sel).ToStdString();
}

 void setChoice(wxChoice* ch,const wxString& value)
{
    int sel = ch->FindString(value);
    if (sel != wxNOT_FOUND)
    {
        ch->SetSelection(sel);
    }
    else
    {
        wxLogMessage("Operation choice value %s not found", value.c_str());
    }
}
