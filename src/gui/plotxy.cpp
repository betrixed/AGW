/////////////////////////////////////////////////////////////////////////////
// Name:        plotxy.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     02/11/2013 00:09:10
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "plotxy.h"
#include "plotptr.h"
#include "json/json.h"
#include "helper.h"
////@begin XPM images
////@end XPM images

#include <ostream>
#include <fstream>
#include <sstream>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/dragimag.h>

#include "plotlua.h"

#ifdef __WXGTK__
   // typedef wxGenericDragImage  wxDragImage
#endif // __WXGTK__

class MyDragImage: public wxDragBase
{
public:
    MyDragImage(PlotXY* canvas): m_canvas(canvas) {}

    MyDragImage(PlotXY* canvas, const wxBitmap& image, const wxCursor& cursor = wxNullCursor):
        wxDragImage(image, cursor), m_canvas(canvas)
    {
    }

    MyDragImage(PlotXY* canvas, const wxIcon& image, const wxCursor& cursor = wxNullCursor):
        wxDragImage(image, cursor), m_canvas(canvas)
    {
    }

    MyDragImage(PlotXY* canvas, const wxString& str, const wxCursor& cursor = wxNullCursor):
        wxDragImage(str, cursor), m_canvas(canvas)
    {
    }


protected:
    PlotXY*   m_canvas;
};
/*
 * PlotXY type definition
 */

IMPLEMENT_DYNAMIC_CLASS( PlotXY, wxScrolledCanvas )


/*
 * PlotXY event table definition
 */

BEGIN_EVENT_TABLE( PlotXY, wxScrolledCanvas )

////@begin PlotXY event table entries
    EVT_SIZE( PlotXY::OnSize )
    EVT_MOUSE_EVENTS( PlotXY::OnMouse )
////@end PlotXY event table entries

END_EVENT_TABLE()

using namespace agw;
/*
 * PlotXY constructors
 */

PlotXY::PlotXY()
{
    Init();
}

PlotXY::PlotXY(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * PlotXY creator
 */

bool PlotXY::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin PlotXY creation
    wxScrolledCanvas::Create(parent, id, pos, size, style);
    CreateControls();
////@end PlotXY creation
    return true;
}


/*
 * PlotXY destructor
 */

PlotXY::~PlotXY()
{
////@begin PlotXY destruction
////@end PlotXY destruction

}


/*
 * Member initialisation
 */

void PlotXY::Init()
{

    //world_.yScale_.setDisplay(true,true);

    m_dragImage = nullptr;
    m_dragMode = DragMode::TEST_DRAG_NONE;

    m_fullScreen = false;

    m_dragLayer = nullptr;
    m_currentlyHighlighted = nullptr;

    //text_.thePlot = this;
}


/*
 * Control creation for PlotXY
 */

void PlotXY::CreateControls()
{
////@begin PlotXY content construction
    //PlotXY* itemScrolledCanvas1 = this;

    this->SetScrollbars(1, 1, 8, 8);
////@end PlotXY content construction
}


/*
 * Should we show tooltips?
 */

bool PlotXY::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap PlotXY::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin PlotXY bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end PlotXY bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon PlotXY::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin PlotXY icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end PlotXY icon retrieval
}

void PlotXY::OnDraw(wxDC& dc)
{
    agw::PlotLua* pl = plotLua_.get();

    std::vector<agw::PlotLayer_sptr>& layers = pl->layers_;

    if (layers.size() == 0)
        return;

    int xv, yv;

    GetVirtualSize(&xv, &yv);

    PixelWorld& pw = pl->world_;

    pw.calc(xv,yv);
    pw.calcScales(layers);


    wxBrush backBrush(pw.backColor_);
    dc.SetBackground(backBrush);
    dc.DrawRectangle(0,0,xv,yv);


    wxPen axePen(pw.axisColor_);
    auto savePen = dc.GetPen();
    dc.SetPen(axePen);

    auto xright = pw.left_ + pw.xspan_;
    auto xbottom = pw.top_ + pw.yspan_;
    if (pw.leftFrame_)
        dc.DrawLine(pw.left_, pw.top_,pw.left_, xbottom);
    if (pw.topFrame_)
        dc.DrawLine(pw.left_, pw.top_, xright, pw.top_);
    if (pw.rightFrame_)
        dc.DrawLine(xright, pw.top_,xright, xbottom);
    if (pw.bottomFrame_)
        dc.DrawLine(xright, xbottom, pw.left_, xbottom);

    pw.yScale_.renderDC(dc,pw);

	// bottom axis,
	pw.xScale_.renderDC(dc,pw);

	auto it = layers.begin();
	auto fin = layers.end();

    for( ; it != fin; it++)
    {
		PlotLayer* p = it->get();

        if (p->isVisible())
            p->renderDC(dc, pw);

    }

    dc.SetPen(savePen);

    std::vector<PlotLayer_sptr>& text = pl->text_.labels_;
    it = text.begin();
    fin = text.end();

    for( ; it != fin; it++)
    {
        PlotLayer* p = it->get();
        p->setPlot(plotLua_);

        if (p->isVisible())
            p->renderDC(dc, pw);
    }

    PlotLayer* p = pl->text_.legend_.get();
    if (p && p->isVisible())
    {
        p->setPlot(plotLua_);
        p->renderDC(dc, pw);

    }

}

void PlotXY::calcScales()
{
    PlotLua* pl = plotLua_.get();
    pl->world_.calcScales(pl->layers_);
}

void PlotXY::AddData(PlotLayer_sptr& pp)
{
    plotLua_->addLayer(pp);
}

void PlotXY::ExportToPNG(const wxString& filepath)
{
// make the wxBitMap for drawing into
    int x,y;
    this->GetVirtualSize(&x, &y);

    wxBitmap bits(x,y,-1);

    wxMemoryDC temp_dc;

    temp_dc.SelectObject(bits);
    auto oldFont = temp_dc.GetFont();

    temp_dc.SetFont(this->GetFont());


    temp_dc.Clear();
    this->OnDraw(temp_dc);

    temp_dc.SetFont(oldFont);

    bits.SaveFile(filepath,  wxBITMAP_TYPE_PNG);
}


void PlotXY::ExportToCSV(const wxString& filepath)
{
    plotLua_->saveCSV(filepath.ToStdString());
}


/*
 * wxEVT_SIZE event handler for ID_SCROLLEDWINDOW
 */

void PlotXY::OnSize( wxSizeEvent& event )
{
    event.Skip();
    plotLua_->isDrawReady();
    this->Refresh();
}

void PlotXY::SaveToFile(const wxString& path)
{
    plotLua_->saveToFile(path.ToStdString());
}

std::unique_ptr<SymbolDraw>
 SymbolDraw::MakeSymbolDraw(PlotShape ps, int radius, wxDC& dc)
{
    switch(ps)
    {
    case PLUS:
        return std::unique_ptr<SymbolDraw>(new SymPlus(dc, radius));
    case CROSS:
        return std::unique_ptr<SymbolDraw>(new SymCross(dc, radius));
    case CIRCLE:
        return std::unique_ptr<SymbolDraw>(new SymCircle(dc, radius));
    case TRIANGLE:
        return std::unique_ptr<SymbolDraw>(new SymTriangle(dc,radius));
    case SQUARE:
        return std::unique_ptr<SymbolDraw>(new SymSquare(dc,radius));
    default:
        return std::unique_ptr<SymbolDraw>(new SymNone(dc));
    }
}

LegendLayer* PlotXY::getLegend()
{
    return plotLua_->text_.getLegend();
}

void PlotXY::addLegend()
{
    plotLua_->addLegend();
}

void PlotXY::removeLegend()
{
    plotLua_->removeLegend();
}


BoxLayer* PlotXY::findLayer(const wxPoint& pos)
{
    return plotLua_->findLayer(pos);
}
/*
 * All mouse events event handler for ID_SCROLLEDWINDOW
 */

void PlotXY::OnMouse( wxMouseEvent& event )
{
   // event.Skip();
    if (event.LeftDown())
    {
        BoxLayer* box = findLayer(event.GetPosition());
        if (box)
        {
            // We tentatively start dragging, but wait for
            // mouse movement before dragging properly.

            m_dragMode = TEST_DRAG_START;
            m_dragStartPos = event.GetPosition();
            m_dragLayer = box;
        }
    }
    else if (event.Dragging() && (m_dragMode != TEST_DRAG_NONE))
    {
        if (m_dragMode == TEST_DRAG_START)
        {
            // We will start dragging if we've moved beyond a couple of pixels

            int tolerance = 2;
            int dx = abs(event.GetPosition().x - m_dragStartPos.x);
            int dy = abs(event.GetPosition().y - m_dragStartPos.y);
            if (dx <= tolerance && dy <= tolerance)
                return;

            // Start the drag.
            m_dragMode = TEST_DRAG_DRAGGING;

            if (m_dragImage)
                delete m_dragImage;

            // Erase the dragged shape from the canvas
            m_dragLayer->Show(false);

            // redraw immediately
            Refresh(true);
            Update();

            wxBitmap* drag = m_dragLayer->makeImage();
            m_dragImage = (wxDragBase*) new MyDragImage(this, *drag, wxCursor(wxCURSOR_HAND));
            /*switch (m_draggedShape->GetDragMethod())
            {
                case SHAPE_DRAG_BITMAP:
                {
                    m_dragImage = new MyDragImage(this, m_draggedShape->GetBitmap(), wxCursor(wxCURSOR_HAND));
                    break;
                }
                case SHAPE_DRAG_TEXT:
                {
                    m_dragImage = new MyDragImage(this, wxString(wxT("Dragging some test text")), wxCursor(wxCURSOR_HAND));
                    break;
                }
                case SHAPE_DRAG_ICON:
                {
                    m_dragImage = new MyDragImage(this, wxICON(dragicon), wxCursor(wxCURSOR_HAND));
                    break;
                }
            }*/


            // The offset between the top-left of the shape image and the current shape position
            wxPoint beginDragHotSpot = m_dragStartPos - m_dragLayer->getPosition();

            // Now we do this inside the implementation: always assume
            // coordinates relative to the capture window (client coordinates)

            //if (fullScreen)
            //    beginDragHotSpot -= ClientToScreen(wxPoint(0, 0));

            if (!m_dragImage->BeginDrag(beginDragHotSpot, this, this->m_fullScreen))
            {
                wxDELETE(m_dragImage);
                m_dragMode = TEST_DRAG_NONE;

            } else
            {
                m_dragImage->Move(event.GetPosition());
                m_dragImage->Show();
            }
        }
        else if (m_dragMode == TEST_DRAG_DRAGGING)
        {
            // We're currently dragging. See if we're over another shape.
            BoxLayer* box = findLayer(event.GetPosition());

            bool mustUnhighlightOld = false;
            bool mustHighlightNew = false;

            if (m_currentlyHighlighted)
            {
                if ((box == nullptr) || (m_currentlyHighlighted != box))
                    mustUnhighlightOld = true;
            }

            if (box && (box != m_currentlyHighlighted) && box->isVisible())
                mustHighlightNew = true;

            if (mustUnhighlightOld || mustHighlightNew)
                m_dragImage->Hide();

            // Now with the drag image switched off, we can change the window contents.
            if (mustUnhighlightOld)
                m_currentlyHighlighted = nullptr;

            if (mustHighlightNew)
                m_currentlyHighlighted = box;

            if (mustUnhighlightOld || mustHighlightNew)
            {
                Refresh(mustUnhighlightOld);
                Update();
            }

            // Move and show the image again
            m_dragImage->Move(event.GetPosition());

            if (mustUnhighlightOld || mustHighlightNew)
                 m_dragImage->Show();
        }
    }
    else if (event.LeftUp() && (m_dragMode != TEST_DRAG_NONE))
    {
        // Finish dragging

        m_dragMode = TEST_DRAG_NONE;

        if (!m_dragLayer || !m_dragImage)
            return;
        wxPoint offset = event.GetPosition() - m_dragStartPos;

        m_dragLayer->offset(offset);

        PlotPlace pp = nearestPlace(m_dragLayer->getCenter());
        m_dragLayer->setPlace(pp);

        wxSize dim = m_dragLayer->getSize();
        wxPoint boffset = this->getPlotPlace(pp) + m_dragLayer->getPlaceAdjust(dim);

        m_dragLayer->setOffset(m_dragLayer->getPosition() - boffset);

        m_dragImage->Hide();
        m_dragImage->EndDrag();

        wxDELETE(m_dragImage);

        m_dragLayer->Show(true);

        m_currentlyHighlighted = nullptr;

        m_dragLayer = nullptr;

        Refresh(true);
    }
}

wxPoint PlotXY::getPlotPlace(PlotPlace pp)
{
    return plotLua_->getPlotPlace(pp);
}

void PlotXY::ReadJSON(const Json::Value& jplot)
{
    plotLua_->readJson(jplot);
}

PlotPlace PlotXY::nearestPlace(const wxPoint& pos)
{
    PlotPlace nearest = PlotPlace(0);
    double    min_distance;
    for(int pp = PlotPlace(0); pp < PlotPlace::FIXED; pp++)
    {
        wxPoint anchor = getPlotPlace((PlotPlace)pp);
        wxPoint offset = anchor - pos;

        double distance = sqrt(offset.x*offset.x + offset.y*offset.y);

        if (pp == PlotPlace(0))
        {
            min_distance = distance;
        }
        else if (distance < min_distance)
        {
            min_distance = distance;
            nearest = PlotPlace(pp);
        }
    }
    return nearest;
}
