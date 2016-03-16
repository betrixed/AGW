#ifndef _H_MODULE_SHAPELIB
#define _H_MODULE_SHAPELIB


#include <shapefil.h>

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>

namespace shp {
// Model for shapemap
enum MapType {
    POINT   = 0,
    POLYGON_AREA = 1,
};

class ShapeFile;
class MapPolygon;

class MapPart {
public:
    MapType mapType_;
    int     objectId_;
};

class  MapPolygon {
    typedef std::vector<double>::iterator DataIt;

    void set(std::vector<double>& data, double* base, size_t ct)
    {
        size_t ix = 0;
        for (DataIt it = data.begin();
            (ix < ct) && (it != data.end());
            it++, ix++)
        {
            *it = *base++;
        }
    }
public:

    std::vector<double>    xval;
    std::vector<double>    yval;
    int         partId_;

    MapPolygon(int id) : partId_(id)
    {
    }
    void setLength(size_t len)
    {
        xval.resize( len);
        yval.resize( len);
    }

    size_t length() const
    {
        return xval.size();
    }
    void setX(double* base, size_t ct)
    {
        set(xval, base, ct);
    }
    void setY(double* base, size_t ct)
    {
        set(yval, base, ct);
    }
};

// made of multiple parts
class  ShapeArea : public MapPart {

public:
    typedef std::unique_ptr<MapPolygon> SAPtr;
    std::vector<SAPtr> part_;

    ShapeArea(int id)
    {
        objectId_ = id;
        mapType_ = MapType::POLYGON_AREA;
    }
};


enum FieldType {
  STR,
  INT,
  DBL,
  LOG,
  BAD
};

class DBField {
public:
    int             index_;
    FieldType       ftype_;
    std::string      name_;
    int             width_;
    int             decimals_;

    DBField(int i) : index_(i), ftype_(FieldType::STR), width_(0), decimals_(0)
    {
        index_ = i;
    }

    static std::string fieldType(FieldType ft)
    {
        switch(ft)
        {
            case FieldType::STR:
                return "TEXT";
            case FieldType::INT:
                return "INTEGER";
            case FieldType::DBL:
                return "REAL";
            case FieldType::LOG:
                return "INTEGER";
            default:
                return "TEXT";
        }
    }

    std::string sqlField ()
    {
        std::stringstream ss;

        outputSql(ss);

        return ss.str();
    }

    std::ostream& outputSql(std::ostream& os)
    {
        os << name_ << " " << fieldType(ftype_);
        if (width_ > 0) {
            os << " (" << width_;
            if (decimals_ > 0)
                os << ", " << decimals_;
            os << ")";
        }
        return os;
    }
    std::ostream& output(std::ostream& os)
    {
        os << index_ << ": " << name_ << ", " << fieldType(ftype_) << ", (" << width_ << ", " << decimals_ << ")";
        return os;
    }
};

class  ShapeDBF {
private:
    DBFHandle db_;
    std::string filepath_;
    int nRecords_;
    int nFields_;

    typedef std::unique_ptr<DBField> FieldPtr;
    std::vector<FieldPtr> field_;

public:
    ShapeDBF(const std::string& path) : db_(nullptr), nRecords_(0), nFields_(0)
    {
        open(path);
    }
    void close()
    {
        if (db_ != nullptr)
        {
            DBFClose(db_);
            db_ = nullptr;
        }
    }

    size_t nFields() const { return nFields_; }
    size_t nRecords() const { return nRecords_; }

    DBField* fieldInfo(size_t i)
    {
        if (i < field_.size())
        {
            return field_[i].get();
        }
        return nullptr;
    }
    void readTuple( std::vector<std::string>& data, int recordNo)
    {
        const size_t ct = (int) field_.size();
        if (data.size() != ct)
            data.resize(ct);
        const size_t i = recordNo;

        for(size_t f = 0; f < ct; f++)
        {
            std::string& value = data[f];
            if ( DBFIsAttributeNULL(db_,i,f)==0)
            {
                const DBField& info = *(field_[f]);
                switch(info.ftype_)
                {
                    case FieldType::STR:
                        value = DBFReadStringAttribute(db_,i,f);
                        break;
                    case FieldType::INT:
                        value = DBFReadIntegerAttribute(db_,i,f);
                        break;
                    case FieldType::LOG:
                        value = DBFReadLogicalAttribute(db_,i,f);
                        break;
                    case FieldType::DBL:
                        value = DBFReadDoubleAttribute(db_,i,f);
                    default:
                        break;
                }
            }
            else {
                value.clear();
            }
        }
    }

    bool readFieldInfo()
    {
        if (db_ != nullptr)
        {
            nFields_ = 	DBFGetFieldCount( db_ );
            nRecords_ = DBFGetRecordCount( db_ );
            std::cout << "Number of records = " << nRecords_ << std::endl;
            std::cout << "Number of fields = " << nFields_ << std::endl;
            std::unique_ptr<char> buf(new char[150]);

            for( int i = 0; i < nFields_; i++)
            {
                FieldPtr fp(new DBField(i));

                DBField& info = *fp;
                auto ftype = DBFGetFieldInfo( db_, i, buf.get(), &info.width_, &info.decimals_);

                info.name_ = buf.get();
                info.ftype_ = (FieldType) ftype;
                info.output(std::cout);

                field_.push_back(std::move(fp));
            }
        }
        return (field_.size() > 0);
    }

    void dump(std::ostream& os)
    {
        std::vector<std::string> data;
        for (int i = 0; i < nRecords_;i++)
        {
            readTuple(data, i);

            for(size_t j = 0 ; j < data.size(); j++)
            {
                if (j > 0)
                    os << ", ";
                os << j << ": ", data[j];
            }
            os << std::endl;
        }
    }
    bool open(const std::string&  path)
    {
        close();
        filepath_ = path;
        db_ = DBFOpen(filepath_.c_str(), "r+b");
        if (db_ == nullptr)
            return false;
        readFieldInfo();
        return true;
    }
};


class ShapeObject;



class ShapeObject {
// not virtual
private:
    SHPObject* sob_;
public:
    ShapeObject(SHPObject* obj)
    {
        sob_ = obj;
    }
    SHPObject* handle()
    {
        return sob_;
    }
    void close()
    {
        if (sob_ != nullptr)
        {
            SHPDestroyObject(sob_);
            sob_ = nullptr;
        }
    }
    ~ShapeObject()
    {
        close();
    }

};


class ShapeFile {
private:
    typedef std::unique_ptr<ShapeObject> ShapePtr;
    SHPHandle      sh_;
    std::string    filepath_;
    std::vector<ShapePtr>  cache_;

    void reset_cache()
    {
        for( size_t i = 0; i < cache_.size(); i++)
        {
            cache_[i].reset();
        }
    }

public:
    ShapeFile()
    {

    }

    ShapeFile(const std::string& path)
    {
        open(path);
    }
    SHPHandle handle()
    {
        return sh_;
    }
    ~ShapeFile()
    {
        close();
    }
    bool isOpen() const
    {
        return (sh_ != nullptr);
    }

    size_t length()
    {
        return cache_.size();
    }

    ShapeObject* get(size_t i)
    {
        if (i >= cache_.size())
            return nullptr;
        ShapeObject* result = cache_[i].get();
        if (result != nullptr)
            return result;

        SHPObject* sob = SHPReadObject(sh_, (int) i);
        cache_[i] = ShapePtr(new ShapeObject(sob));
        return cache_[i].get();
    }
    void close()
    {
        if (sh_ != nullptr)
        {
            reset_cache();
            SHPClose(sh_);
            sh_ = nullptr;
        }
    }
    bool open(const std::string& path)
    {
        close();
        filepath_ = path;
        sh_ = SHPOpen(filepath_.c_str(), "r+b");
        if (sh_ != nullptr)
        {
            cache_.resize(sh_->nRecords);
            return true;
        }
        return false;
    }
};

class Map {
    typedef std::unique_ptr<MapPart> MapPartPtr;

    std::vector<MapPartPtr>  part_;
public:
    void makeParts(ShapeFile* sf)
    {
        auto ct = sf->length();
        for (size_t i = 0; i < ct; i++)
        {
            ShapeObject* sob = sf->get(i);
            SHPObject* h = sob->handle();
            double* x = h->padfX;
            double* y = h->padfY;
            auto vLimit = h->nVertices;
            ShapeArea* area = new ShapeArea((int)i);
            part_.emplace_back(MapPartPtr(area));


            for (int p = 0; p < h->nParts; p++)
            {
                int pix = h->panPartStart[p];
                auto endIndex =  (p < h->nParts - 1) ? h->panPartStart[p+1] : vLimit;

                ShapeArea::SAPtr pp(new MapPolygon(p));
                size_t plen = endIndex - pix;
                MapPolygon& sp = *pp;
                sp.setLength(plen);
                sp.setX(x + pix, plen);
                sp.setY(y + pix, plen);
                area->part_.push_back(std::move(pp));
            }
        }
    }

};
}; // namespace shp
#endif
