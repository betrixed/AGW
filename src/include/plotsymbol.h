#ifndef _H_PLOTSYMBOL_
#define _H_PLOTSYMBOL_

#ifndef _H_AGW_FIRST
#include "agw.h"
#endif

namespace agw {
	enum PlotShape {
		NONE,
		PLUS,
		CROSS,
		CIRCLE,
		TRIANGLE,
		SQUARE
	};

	class SymbolDraw {
	protected:
		wxDC& dc_;
	public:
		SymbolDraw(wxDC& dc) : dc_(dc)
		{
		}

		virtual void draw(int x, int y) { ignore(x); ignore(y); }

		static std::unique_ptr<SymbolDraw> MakeSymbolDraw(PlotShape ps, int radius, wxDC& dc);

	};

	class SymNone : public SymbolDraw {
	public:
		SymNone(wxDC& dc) : SymbolDraw(dc)
		{
		}
		/*
		SymNone(wxDC& dc, int radius) : SymbolDraw(dc)
		{
			ignore(radius);
		}
		*/
	};

	class SymCircle : public SymbolDraw {
	public:
		int radius_;

		SymCircle(wxDC& dc, int radius) : SymbolDraw(dc), radius_(radius)
		{
		}

		virtual void draw(int x, int y)
		{
			dc_.DrawCircle(x, y, radius_);
		}
	};


	class SymTriangle : public SymbolDraw {
	public:
		wxPoint triangle[3];

		SymTriangle(wxDC& dc, int radius) : SymbolDraw(dc)
		{
			int base = (int)((double)radius * 2 * 1.155);
			triangle[0] = wxPoint(-radius, radius);
			triangle[1] = wxPoint(base / 2 - radius, -radius);
			triangle[2] = wxPoint(base - radius, radius);
		}

		virtual void draw(int x, int y)
		{
			dc_.DrawPolygon(3, triangle, x, y);
		}
	};

	class SymSquare : public SymbolDraw {
	public:
		int half_;
		int full_;

		SymSquare(wxDC& dc, int radius) : SymbolDraw(dc), half_(radius), full_(radius * 2)
		{

		}

		virtual void draw(int x, int y)
		{
			dc_.DrawRectangle(x - half_, y - half_, full_, full_);
		}
	};

	class SymPlus : public SymbolDraw {
	public:
		int half_;

		SymPlus(wxDC& dc, int radius) : SymbolDraw(dc), half_(radius)
		{

		}

		virtual void draw(int x, int y)
		{
			dc_.DrawLine(x - half_ + 1, y, x + half_, y);
			dc_.DrawLine(x, y - half_ + 1, x, y + half_);
		}
	};

	class SymCross : public SymbolDraw {
	public:
		int half_;

		SymCross(wxDC& dc, int radius) : SymbolDraw(dc), half_(radius)
		{

		}

		virtual void draw(int x, int y)
		{
			dc_.DrawLine(x - half_, y - half_, x + half_ + 1, y + half_ + 1);
			dc_.DrawLine(x - half_, y + half_, x + half_ + 1, y - half_ - 1);
		}
	};

}; // namespace agw
#endif //_H_PLOTSYMBOL_
