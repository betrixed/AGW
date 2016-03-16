#ifndef _H_SPATIALITE_GEOMETRY
#define _H_SPATIALITE_GEOMETRY

#ifndef _H_AGW_FIRST
#include "agw.h"
#endif


#include <vector>
#include <cassert>



namespace agw {

enum {
    POINT = 1,
    LINESTR = 2,
    POLY = 3,
    MPOINT = 4,
    MLINESTR = 5,
    MPOLY = 6,
    COLLECTION = 7
};

class GPoint {
public:
    GPoint(double x = 0.0, double y = 0.0) : X_(x), Y_(y)
    {
    }

    bool operator!=(const GPoint& c)
    {
        return (c.X_ != this->X_) || (c.Y_ != this->Y_);
    }
    double  X_;
    double  Y_;
};

typedef std::vector<GPoint> LineData;

class GRing {
public:

    GRing(const double* p, size_t ct);

    int wn_PnPoly(const GPoint& P);

    bool containsPoint(const GPoint& P);

    LineData data;
};

class  GPoly {
public:
    std::vector<std::unique_ptr<GRing>> data;

    GPoly(size_t len = 0)
    {
        if (len > 0)
        {
            data.reserve(len);
        }
    }
    void size(size_t len)
    {
        data.resize( len);
    }

    size_t size() const
    {
        return data.size();
    }

    bool containsPoint(const GPoint& p)
    {
        auto rit = data.begin();
        auto rfin = data.end();

        for(;rit != rfin; rit++)
        {
            agw::GRing& g = *(rit->get());

            if (g.containsPoint(p))
                return true;
        }
        return false;
    }
};

class GRect {
public:
    double minX_;
    double minY_;
    double maxX_;
    double maxY_;

    bool containsPoint(const GPoint& P)
    {
        if (P.X_ < minX_ || P.X_ > maxX_)
            return false;
        if (P.Y_ < minY_ || P.Y_ > maxY_)
            return false;
        return true;
    }
};

typedef std::unique_ptr<GPoly> GPolyPtr;

class  GMultiPoly  {
public:
    GRect       MBR;
    std::vector<GPolyPtr> data;

    GMultiPoly(size_t size_hint = 0)
    {
        data.reserve(size_hint);
    }

    bool containsPoint(const GPoint& P)
    {
        if (!MBR.containsPoint(P))
            return false;
        auto pit = data.begin();
        auto pfin = data.end();

        for(; pit != pfin; pit++)
        {
            agw::GPoly& poly= *(pit->get());
            if (poly.containsPoint(P))
            {
                return true;
            }
        }
        return false;
    }

};



// Read off a blob stream

class  GeomReader {
    const uint8_t* p;
    const uint8_t* end_;

public:
    GeomReader(const void* ptr, uint bytes);

    void readGType(int32_t& srid, GRect& bound, int32_t& wkb_class);
    void readMultiPoly(GMultiPoly& mp);
    void readPoint(GPoint& pt);
};

}; //  namespace agw


#endif //_H_SPATIALITE_GEOMETRY
