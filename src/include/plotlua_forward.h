#ifndef _H_PLOTLUA_FW
#define _H_PLOTLUA_FW

class PlotFrame;
 class PlotXY;

namespace agw {
	class PixelWorld;
	class PlotLua;
    class DataTable;
    class PlotLayer;


	typedef std::shared_ptr<DataTable>  DataTable_sptr;
	typedef std::shared_ptr<PlotLua>    PlotLua_sptr;
    typedef std::weak_ptr<PlotLua>      PlotLua_wptr;
	// this class generates the plot image
	typedef std::shared_ptr<PlotLayer> PlotLayer_sptr;
};

#endif
