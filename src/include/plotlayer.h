#ifndef _H_PLOTLAYER
#define _H_PLOTLAYER

#ifndef _H_AGW_FIRST
#include "agw.h"
#endif

#include "plotptr.h"
#include "plotsymbol.h"
//#include "lua.hpp"
// first 4 are middle of borders
// second 4 are corners

// Something that can be moved around, as an image. Therefore it is drawn as an image.
namespace agw {


	class BoxLayer : public PlotLayer {
	protected:
		wxRect      bounds_; // calculated bounds
		wxPoint     offset_; //
		PlotPlace   place_; // relation to plot corner

	public:
		BoxLayer() : PlotLayer(), bounds_(0, 0, 0, 0), offset_(0, 0), place_(MIDDLE)
		{
		}
		void  setOffset(const wxPoint& p) { offset_ = p; }
		void  setPlace(PlotPlace pp) { place_ = pp; }
		const wxPoint& getOffset() const { return offset_; }
		void  offset(const wxPoint& p) { bounds_.Offset(p); offset_ += p; }
		wxPoint getCenter() { return wxPoint(bounds_.x + bounds_.width / 2, bounds_.y + bounds_.height / 2); }
		wxPoint getPosition() { return wxPoint(bounds_.x, bounds_.y); }
		wxPoint getPlaceAdjust(const wxSize& dim);
		wxSize  getSize() const { return bounds_.GetSize(); }
		bool    contains(const wxPoint& p) { return bounds_.Contains(p); }

		virtual void SaveJSON(Json::Value& json);
		virtual void ReadJSON(const Json::Value& json);

		virtual wxBitmap* makeImage() = 0;

		virtual ~BoxLayer() {}

		void dump()
		{
			std::stringstream ss;

			ss << "bounds: " << bounds_.x << ", " << bounds_.y << ", " << bounds_.width << ", " << bounds_.height << std::endl;
			ss << "place: " << place_ << std::endl;
			ss << "offset: " << offset_.x << ", " << offset_.y << std::endl;

			wxLogMessage("Box %s", ss.str().c_str());
		}
	};



	enum LabelKind {
		NORMAL,
		XLABEL,
		YLABEL,
		TITLE
	};

	class TextLayer : public BoxLayer {
	protected:
		LabelKind kind_;
		std::vector<std::string> lines_;
		TextAlign  align_;
		TextRotate rotate_;
		std::string fontFace_;
		int fontPoints_;

		const int border = 5;
	public:
		TextLayer(PlotPlace pp, const wxPoint& offset);

		virtual void setLines(const std::vector<std::string>& lines);
		virtual const std::vector<std::string>& getLines();

		virtual void SaveJSON(Json::Value& json);
		virtual void ReadJSON(const Json::Value& json);


		virtual void renderDC(wxDC& dc, PixelWorld& px);

		virtual wxBitmap* makeImage();
		void drawInBox(wxDC& dc, const wxRect& box);
		LabelKind kind() const { return kind_; }
		void kind(LabelKind k) { kind_ = k; }
		void value(const std::string& s);
		void rotate(TextRotate r) { rotate_ = r; }
	};


	class LegendLayer : public BoxLayer {
		const int symwidth = 36;
		const int border = 10;
	public:
		LegendLayer(PlotPlace pp, const wxPoint& offset);
		virtual ~LegendLayer();

		virtual void SaveJSON(Json::Value& json);
		virtual void ReadJSON(const Json::Value& json);

		virtual void renderDC(wxDC& dc, PixelWorld& px);
		virtual wxBitmap* makeImage();
		void drawInBox(wxDC& dc, const wxRect& box);
	};




	class DataLayer : public PlotLayer {
	public:
		DataLayer();
		virtual ~DataLayer();

		bool isReady()
		{
			return (xdata_ != nullptr) && (ydata_ != nullptr);
		}

		void calcStats();

		virtual void randomStyle();
		virtual void renderDC(wxDC& dc, PixelWorld& px);

		virtual void SaveJSON(Json::Value& json);
		virtual void ReadJSON(const Json::Value& json);
	public:
		NormalStats xstats_;
		SeriesPtr xdata_;

		NormalStats ystats_;
		SeriesPtr    ydata_;

		SeriesPtr errorbar_;
        double    errorMax_;
		// render properties
		int         symbolShape_;
		int         symbolSize_;
		wxColour    symbolBorder_;
		wxColour    symbolFill_;

		int         lineStyle_;  // index into a penstyle
		int         lineWidth_;
		wxColour    lineColor_;
		int         xNudge_; // a pixel nudge or two for distinguishing error bars.
		std::string label_;
		std::string legendText_;
		// lua
		static int getXData(lua_State* L);
		static int getYData(lua_State* L);
		static int setLegend(lua_State* L);
		static int setDisplay(lua_State* L);

		static PlotLayer_sptr* checkLayer(lua_State *L, int index);
		static PlotLayer_sptr* toLayer(lua_State* L, int index);
		static PlotLayer_sptr* pushLayer(lua_State* L);

		static const char* DL_LUA;
	};

	class LineFit : public DataLayer {
	public:
		LineFit()
		{
			jype_ = "line_fit";

			xdata_ = std::make_shared<DoubleSeries>();
			xdata_->setSize(2);
			xdata_->setLabel("fit.x");
			ydata_ = std::make_shared<DoubleSeries>();
			ydata_->setSize(2);
			ydata_->setLabel("fit.y");
			lineColor_.Set(0xFF, 0, 0);
		}
		// regression calculates from series but does not hold a reference to them.
		void regression(const Series& x, const Series& y)
		{
			RCorrelate rc;

			rc.calc(x, y);

			slope_ = rc.rval_ * rc.ys.stddev_ / rc.xs.stddev_;
			yintercept_ = rc.ys.mean_ - slope_ * rc.xs.mean_;
			rval_ = rc.rval_;

			xdata_->set(0, rc.xs.minval_);
			xdata_->set(1, rc.xs.maxval_);
			ydata_->set(0, slope_ * rc.xs.minval_ + yintercept_);
			ydata_->set(1, slope_ * rc.xs.maxval_ + yintercept_);
		}
		virtual void renderDC(wxDC& dc, PixelWorld& px);

		double slope_;
		double yintercept_;
		double rval_;

		std::string  resultAsText();

		virtual void SaveJSON(Json::Value& json);
		virtual void ReadJSON(const Json::Value& json);

		static const char* LF_LUA;
		static PlotLayer_sptr* checkLineFit(lua_State *L, int index);
		static PlotLayer_sptr* pushLineFit(lua_State* L);
		static int getRVal(lua_State* L);
		static int getSlope(lua_State* L);
		static int getYOffset(lua_State* L);
	};

	class LabelWorld {
	public:
		PlotLayer_sptr              legend_; // only one of these?
		std::vector<PlotLayer_sptr> labels_;
		PlotXY*  thePlot;
		void SaveJSON(Json::Value& json);
		void ReadJSON(const Json::Value& json);

		void xlabel(const std::string& s);
		void ylabel(const std::string& s);
		void title(const std::string& s);
		void addLegend();
		void removeLegend();
		void setLegend(PlotLayer_sptr pp);

		PlotLayer_sptr getLabelKind(LabelKind k);

		LegendLayer* getLegend();

	};

    PlotLayer_sptr regressRange(const Series& srcX, const Series& srcY, double xMin, double xMax);


}; // namespace agw
#endif
