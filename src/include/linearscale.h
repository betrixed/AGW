#ifndef _H_LINEAR_SCALE
#define _H_LINEAR_SCALE

#include <wx/colour.h>

#include "plotptr.h"



class PlotProperty;


namespace agw {
	enum ScaleFlow {
		POSITIVE = 1,
		NEGATIVE = -1
	};

	enum ScaleCoord {
		YCOORD = 0,
		XCOORD = 1
	};

	class PixelWorld;
	class PlotLayer;

	class LinearScale {

		friend class ::PlotProperty;
		friend class DataLayer;
		friend class PixelWorld;

		double scale_;
		double offset_;
		double axisTicks_;

		double refValue_;

		ScaleCoord coord_;
		ScaleFlow direct_;

		double dataMax_;
		double dataMin_;

		std::string label_;
		SeriesUnit units_;   // indicate kind of units
		uint   fixedSize_;   // user fixed pixel span
		uint   axisDiv_;     // number of small ticks

		bool   autoLimits_; // calculate axis data limits from data
		bool   autoTicks_; // calculate major ticks and minor ticks automatically from data limits

		bool   showMajorGrid_;  // display grid for ticks
		bool   showMinorTicks_; // display grid for ticks

		bool   doFixedSize_; // pixel span not changed by window resize
		bool   refLine_;     // display special line for value

		bool   insideTick_;
		bool   insideLabel_;
		TextRotate  tickRotate_;

	public:
		LinearScale(ScaleCoord co, ScaleFlow flow);

		void calcScale(int32_t pixelSpan)
		{
			if ((units_ == DATE_JULIAN_MOD) || (units_ == DATE_YEAR_MONTH))
			{
				// if span more than 2 years...
				wxDateTime startTime(dataMin_ + 2400000.5);
				wxDateTime endTime(dataMax_ + 2400000.5);

				auto startYear = startTime.GetYear();
				auto endYear = endTime.GetYear();
				scale_ = direct_ * pixelSpan / (dataMax_ - dataMin_);
				offset_ = (direct_ > 0) ? dataMin_ : dataMax_;

				if (this->autoTicks_)
					AutoAxis(pixelSpan, startYear, endYear, axisTicks_, axisDiv_);
			}
			else {
				scale_ = direct_ * pixelSpan / (dataMax_ - dataMin_);
				offset_ = (direct_ > 0) ? dataMin_ : dataMax_;
				if (this->autoTicks_)
					AutoAxis(pixelSpan, dataMin_, dataMax_, axisTicks_, axisDiv_);
			}
		}
		void units(SeriesUnit u) {
			units_ = u;
		}
		void setLabel(const std::string& s) { label_ = s; }
		const std::string& label() { return label_; }
		void widenLimits(double fraction)
		{
			double fdiff = (dataMax_ - dataMin_)* fraction;
			dataMax_ += fdiff;
			dataMin_ -= fdiff;
		}

		void setDataLimits(double& min, double& max)
		{
			dataMax_ = max;
			dataMin_ = min;
		}
		void setScale(ScaleFlow val, ScaleCoord co)
		{
			direct_ = val;
			coord_ = co;
		}
		void setGridDisplay(bool grid, bool ticks)
		{
			showMajorGrid_ = grid;
			showMinorTicks_ = ticks;
		}

		bool showGrid() const { return showMajorGrid_; }
		bool showTicks() const { return showMinorTicks_; }

		void renderDC(wxDC& dc, PixelWorld& px);
		void drawLeftYAxis(wxDC& dc, PixelWorld& px);
		void drawBottomXAxis(wxDC& dc, PixelWorld& px);
		void drawBottomDateAxis(wxDC& dc, PixelWorld& px);
		static void AutoAxis(long pspan, double a1, double a2, double &tics, uint &labels);

		void SaveJSON(Json::Value& json);
		void ReadJSON(const Json::Value& json);

	};

	enum FlexFlag {

		PLOT = 1,
		MARGIN = 2,
		FONT = 4,
		FLEX_ALL = 7
	};

	class PixelWorld {
	public:
		// margin insets
		int left_;
		int right_;
		int top_;
		int bottom_;
		// resulting spans of graph area.
		int xspan_;
		int yspan_;

		int xfixed_;   //
		int yfixed_;  //
		bool fixed_;  //  explicit spans, set virtual size

		LinearScale xScale_;
		LinearScale yScale_;

		wxColor refColor_;
		wxColor axisColor_;
		wxColor backColor_;
		wxColor gridColor_;

		// draw a frame line or not
		bool leftFrame_;
		bool topFrame_;
		bool rightFrame_;
		bool bottomFrame_;

		// space used up in drawing axis ticks and text, during render
		int leftAxisBound_;
		int topAxisBound_;
		int rightAxisBound_;
		int bottomAxisBound_;

		PixelWorld();

		void setMargins(int left, int top, int right, int bottom)
		{
			left_ = left;
			top_ = top;
			right_ = right;
			bottom_ = bottom;
		}
		int xdim() const
		{
			return xspan_ + left_ + right_;
		}

		int ydim() const
		{
			return yspan_ + top_ + bottom_;
		}

		bool calc(int xspan, int yspan)
		{

			auto save_xspan = xspan_;
			auto save_yspan = yspan_;

			xspan_ = xspan - left_ - right_;
			yspan_ = yspan - top_ - bottom_;
			return (save_xspan != xspan_) || (save_yspan != yspan_);

		}
		void getYScale(double& scale, double& offset)
		{
			scale = yScale_.scale_;
			offset = yScale_.offset_;
		}
		void getXScale(double& scale, double& offset)
		{
			scale = xScale_.scale_;
			offset = xScale_.offset_;
		}

		void SaveJSON(Json::Value& json);
		void ReadJSON(const Json::Value& json);

		void calcScales(std::vector<PlotPtr>& layers);
		void autoXDataLimits(std::vector<PlotPtr>& layers, double& xMin, double &xMax);
		void autoYDataLimits(std::vector<PlotPtr>& layers, double& yMin, double &yMax);
	};

}; // namespace agw;
#endif //_H_LINEAR_SCALE

