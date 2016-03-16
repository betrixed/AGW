/////////////////////////////////////////////////////////////////////////////
// Name:        plotxy.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     02/11/2013 00:09:10
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _PLOTXY_H_
#define _PLOTXY_H_


/*!
 * Includes
 */
#include <wx/scrolwin.h>
#include <wx/log.h>
#include <wx/dc.h>
#include <memory>
#include <vector>

#include "json/json.h"
////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class PlotXY;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_PLOTXY_STYLE wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL
#define SYMBOL_PLOTXY_IDNAME ID_SCROLLEDWINDOW
#define SYMBOL_PLOTXY_SIZE wxSize(500, 300)
#define SYMBOL_PLOTXY_POSITION wxDefaultPosition
////@end control identifiers

#include "series.hpp"

#include "linearscale.h"

/*!
 * PlotXY class declaration
 */


#include "plotlayer.h"
#include "plottable.h"

#include "plotsymbol.h"



class wxDragImage;

enum DragMode {
    TEST_DRAG_NONE,
    TEST_DRAG_START,
    TEST_DRAG_DRAGGING
};

namespace agw {
	class DataTable;
	class PlotLua;
	typedef std::shared_ptr<PlotLua> PlotLuaPtr;
};

class PlotXY: public wxScrolledCanvas
{
    DECLARE_DYNAMIC_CLASS( PlotXY )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    PlotXY();
    PlotXY(wxWindow* parent, wxWindowID id = ID_SCROLLEDWINDOW, const wxPoint& pos = wxDefaultPosition, const wxSize& size = SYMBOL_PLOTXY_SIZE, long style = wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_SCROLLEDWINDOW, const wxPoint& pos = wxDefaultPosition, const wxSize& size = SYMBOL_PLOTXY_SIZE, long style = wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL);

    /// Destructor
    ~PlotXY();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin PlotXY event handler declarations

    /// wxEVT_SIZE event handler for ID_SCROLLEDWINDOW
    void OnSize( wxSizeEvent& event );

    /// All mouse events event handler for ID_SCROLLEDWINDOW
    void OnMouse( wxMouseEvent& event );

////@end PlotXY event handler declarations

////@begin PlotXY member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PlotXY member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PlotXY member variables
    /// Control identifiers
    enum {
        ID_SCROLLEDWINDOW = 10001
    };
////@end PlotXY member variables

    virtual void OnDraw(wxDC& dc);
    void ExportToCSV(const wxString& filepath);
    void ExportToPNG(const wxString& filepath);

    void ReadJSON(const Json::Value& jplot);

    void AddData(agw::PlotPtr& pp);
    void SaveToFile(const wxString& path);
    void setAxisMargins(const wxRect& m);
    void calcScales();
    agw::LegendLayer* getLegend();
    void addLegend();
    void removeLegend();
    agw::PlotPlace nearestPlace(const wxPoint& pos);
    wxPoint getPlotPlace(agw::PlotPlace pp);

    agw::PlotLuaPtr   plotLua_;

    /*
    PixelWorld  world_;
    LabelWorld  text_;

	std::vector<PlotPtr> layers_;

	DataTable*             m_table;
    */

	DragMode        m_dragMode;
    wxPoint         m_dragStartPos;
    agw::BoxLayer*       m_dragLayer;
	agw::BoxLayer*       m_currentlyHighlighted;
	agw::BoxLayer* findLayer(const wxPoint& pos);

    wxDragImage*    m_dragImage;
    bool                   m_fullScreen;

};




#endif
    // _PLOTXY_H_
