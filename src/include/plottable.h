#ifndef H_PLOT_TABLE
#define H_PLOT_TABLE

#ifndef _H_AGW_FIRST
#include "agw.h"
#endif

#include "wx/grid.h"
#include "plotxy.h"
#include "plotptr.h"
#include "series.hpp"

#include <sstream>

// A model for display / edit of multiple layers / columns of plotdata from PlotXY

// A PlotXY has a number of DataLayers, each one of which has 3 potential series xdata, ydata and errorbars (y errors only for now)

// PlotTable references the PlotXY, and unpacks the Series, so we get an array of Series, but each one gets labelled as [Plot].[Layer][.x|.y|.ye|.xe]
// in a global namespace. To keep the entities from being too attached to a particular window, or view, use this global model hierarchy
// PlotXY graphics window is a vector of PlotLayer_sptr, associated with a "PixelWorld", which maybe can be a JSON template. (frame, axes, labels, legend templates.

// another shared object

// PlotLayer_sptr,  SeriesPtr,

namespace agw {
	class JsonStore {
	protected:
		std::string name_;
		std::string jype_; // whats in the json
		std::string json_;
	public:
		JsonStore() = default;
		~JsonStore() = default;

		JsonStore(const std::string& name, const Json::Value& root);
		void setJson(const Json::Value& root);

		const JsonStore& operator=(const Json::Value& root)
		{
			setJson(root);
			return *this;
		}
	};

	class DataTableColumn {
	protected:
		std::string label_;
		SeriesPtr   data_;
		int         colWidth_;
		int         colDec_;
		int         colFormat_;
	public:
		DataTableColumn(SeriesPtr p);
		Series* get() { return data_.get(); }
		const std::string& label() const { return label_; }
	};

	typedef std::unique_ptr<DataTableColumn> ColnPtr;

	// Like the plot XY , a
	class DataTable : public wxGridTableBase {
		std::string           filepath_;
		std::string           nickname_;
		JsonStore             graph_;
		std::vector<ColnPtr>  series_;
		uint                  maxRows_;
	public:

		DataTable(std::vector<PlotLayer_sptr>& data, const Json::Value& graph);

		virtual ~DataTable();

		virtual int GetNumberRows();

		virtual int  GetNumberCols();

		virtual bool IsEmptyCell(int row, int col);

		virtual wxString GetColLabelValue(int col);

		virtual void SetValue(int row, int col, const wxString& value);

		virtual wxString GetValue(int row, int col);

		void addColumn(SeriesPtr& sp);
	};

}; // namespace agw
#endif

