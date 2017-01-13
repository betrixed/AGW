#ifndef _H_PLOTPTR
#define _H_PLOTPTR

#include <memory>
#include <string>
#include <vector>
#include "series.hpp"

#include <wx/log.h>
#include <wx/geometry.h>
#include <wx/dc.h>

#include "plotlua_forward.h"

namespace agw {

	enum PlotPlace {
		LEFT_BORDER,
		TOP_BORDER,
		RIGHT_BORDER,
		BOTTOM_BORDER,
		TLEFT_CORNER,
		TRIGHT_CORNER,
		BRIGHT_CORNER,
		BLEFT_CORNER,
		MIDDLE,
		FIXED,
	};

	enum TextRotate {
		R_0,
		CW_90,
		ACW_90,
		R_180
	};

	int degreesRotation(TextRotate r);

	enum TextAlign {
		LEFT,
		CENTRE,
		RIGHT
	};



	class PlotLayer {
	public:
		PlotLayer() : visible_(true) {}
		virtual ~PlotLayer() {}

		virtual void SaveJSON(Json::Value& json) = 0;
		virtual void ReadJSON(const Json::Value& json) = 0;
		virtual void renderDC(wxDC& dc, PixelWorld& px) = 0;

		void Show(bool visible) { visible_ = visible; }
		bool isVisible() const { return visible_; }

		const std::string& get_jype()
		{
			return jype_;
		}

		void setPlot(PlotLua_sptr pp) { thePlot_ = pp; }
		PlotLua_sptr getPlot() { return thePlot_; }
	protected:
		PlotLua_sptr thePlot_; // a non-shared pointer or a weak pointer?
		std::string jype_;
		bool  visible_;

	};


}; // namespace agw

#endif //_H_PLOTPTR


