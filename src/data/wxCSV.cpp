#include "wxCSV.hpp"

#include <wx/stream.h>

wxCSV::wxCSV(wxTextInputStream* intext, wxChar sep)
: input_(intext), sep_(sep)
{

}
wxCSV::~wxCSV()
{

}

bool wxCSV::next(wxArrayString &row)
{
    const wchar_t DQUOTE = '\"';
    row.Clear();
    wxString chars = input_->ReadLine();
    if (chars.size() == 0)
        return false;
    wxString::const_iterator it = chars.begin();
    wxString::const_iterator end_it = chars.end();
    wxString part;

    part = "";
    wxChar stop = sep_;
    while(it != end_it) {
        if (*it==stop) {
            if (stop == sep_) {
                row.Add(part);
                part = "";
            }
            if (*it==DQUOTE) {
                stop = sep_;
            }
            it++;
            continue;
        }
        if ((*it==DQUOTE)&&(part.size()==0))
        {
            stop = DQUOTE;
            it++;
            continue;
        }
        part << *it;
        it++;
    }
    return row.size() > 0;
}
