#ifndef _H_SERIES_HELPER
#define _H_SERIES_HELPER

#include "agw.h"


class Series;
typedef std::shared_ptr<Series> SeriesPtr;

bool readUInt(const Json::Value& json, const std::string& prop, uint& dest);
bool readInt(const Json::Value& json, const std::string& prop, int& dest);
bool readDouble(const Json::Value& json, const std::string& prop, double& dest);
bool readBool(const Json::Value& json, const std::string& prop, bool& dest);
bool readString(const Json::Value& json, const std::string& prop, std::string& dest);

SeriesPtr seriesFromFile(const char* path);
SeriesPtr readJSONSeries(const Json::Value& v);



#endif // _H_HELPER

