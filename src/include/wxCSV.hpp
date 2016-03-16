#ifndef _H_WXCSV
#define _H_WXCSV

/** Read from a text stream, output array of strings, according to separator and quote logic */
#include <wx/txtstrm.h>

class wxCSV {
public:
    wxCSV(wxTextInputStream* intext, wxChar sep);
    virtual ~wxCSV();
    bool next(wxArrayString &);
private:
    wxTextInputStream *input_;
    wxChar sep_;
};

#endif