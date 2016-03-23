#ifndef _H_SERIES_HELPER
#define _H_SERIES_HELPER

#include "agw.h"


bool readUInt(const Json::Value& json, const std::string& prop, uint& dest);
bool readInt(const Json::Value& json, const std::string& prop, int& dest);
bool readDouble(const Json::Value& json, const std::string& prop, double& dest);
bool readBool(const Json::Value& json, const std::string& prop, bool& dest);
bool readString(const Json::Value& json, const std::string& prop, std::string& dest);

agw::SeriesPtr seriesFromFile(const char* path);
agw::SeriesPtr readJSONSeries(const Json::Value& v);



#endif // _H_HELPER

