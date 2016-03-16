#include "wxCSV.hpp"

#include <wx/stream.h>

wxCSV::wxCSV(wxTextInputStream* intext, wxChar sep) : input_(intext), sep_(sep)
{
    
}
wxCSV::~wxCSV()
{

}

bool wxCSV::next(wxArrayString &row)
{
    auto chars = input_->ReadLine();
    if (chars.size() == 0)
        return false;
    row = wxSplit(chars,sep_);
    return true;
}
