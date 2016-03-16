

#include "geometry.h"
#include <wx/log.h>

using namespace agw;

GeomReader::GeomReader(const void* ptr, uint bytes) : p((uint8_t*)ptr), end_((uint8_t*)ptr + bytes)
{
}

void
GeomReader::readGType(int32_t& srid, GRect& bound, int32_t& wkb_class)
{   // 0 - 42, 43 bytes
    assert(*p == 0 && p + 43 < end_);
    p++;

    bool isLittleEndian = (*p == 0x01);
    assert(isLittleEndian);
    p++;

    srid = *((int32_t*)p);
    p += sizeof(int32_t);

    double* rp = (double*) p;

    bound.minX_ = *rp++;
    bound.minY_ = *rp++;
    bound.maxX_ = *rp++;
    bound.maxY_ = *rp++;
    p = (unsigned char*) rp;

    assert(*p == 0x7C);
    p++;

    wkb_class = *((int32_t*)p);
    p += sizeof(int32_t);
}

void GeomReader::readMultiPoly(GMultiPoly& mp)
{
    int32_t polyCt = *((int32_t*)p);
    p += sizeof(int32_t);

    mp.data.clear();
    mp.data.reserve(polyCt);


    for(int32_t i = 0; i < polyCt; i++)
    {
        // read the collection entity header
        assert(*p == 0x69);
        p++;
        int32_t coll_class = *((int32_t*)p);
        assert(coll_class == POLY);
        p += sizeof(int32_t);


        // read the entity
        int32_t ringCt = *((int32_t*)p);
        p += sizeof(int32_t);

        GPolyPtr poly(new GPoly(ringCt));


        for(int32_t r = 0; r < ringCt; r++)
        {
            int32_t ptsCt = *((int32_t*)p);
            p += sizeof(int32_t);

            const double* dp = (const double*)p;
            const double* endp = (dp + ptsCt*2);

            std::ptrdiff_t diff = end_ - (uint8_t*)endp;
            if (diff < 0)
            {
                assert(diff > 0);
            }
            std::unique_ptr<GRing> spoly(new GRing(dp, ptsCt));
            poly->data.push_back(std::move(spoly));
            p = (uint8_t*)endp;
        }
        mp.data.push_back(std::move(poly));
    }

}
void GeomReader::readPoint(GPoint& pt)
{
    const double* rp = (const double*) p;
    pt.X_ = *rp++;
    pt.Y_ = *rp++;
    p = (uint8_t*)rp;
}

inline int isLeft( const GPoint& P0, const GPoint&  P1, const GPoint&  P2 )
{
    const double diff = (P1.X_ - P0.X_) * (P2.Y_ - P0.Y_) - (P2.X_ -  P0.X_) * (P1.Y_ - P0.Y_);

    return (diff > 0.0) ? 1
         : (diff < 0.0) ? -1
         : 0;

}



int GRing::wn_PnPoly(const GPoint& P)
{
    // loop through all edges of the polygon
    const GPoint* V = &data.front();
    uint  n = data.size()-1;
    int wn = 0;
    for (uint i=0; i<n; i++) {   // edge from V[i] to  V[i+1]
        if (V[i].Y_ <= P.Y_) {          // start y <= P.y
            if (V[i+1].Y_  > P.Y_)      // an upward crossing
                 if (isLeft( V[i], V[i+1], P) > 0)  // P left of  edge
                     ++wn;            // have  a valid up intersect
        }
        else {                        // start y > P.y (no test needed)
            if (V[i+1].Y_  <= P.Y_)     // a downward crossing
                 if (isLeft( V[i], V[i+1], P) < 0)  // P right of  edge
                     --wn;            // have  a valid down intersect
        }
    }
    return wn;
}

// expect closed ring with data[0] == data[ct-1]
GRing::GRing(const double* p, size_t ct)
{
    data.reserve(ct);
    for(size_t i = 0; i < ct; i++, p += 2)
    {
        data.push_back(GPoint(*p, *(p+1)));
    }
}

bool GRing::containsPoint(const GPoint& P)
{
    return (wn_PnPoly(P) != 0);
}



