#ifndef _H_AGW_FIRST
#define _H_AGW_FIRST

template <typename T>
void ignore(T &&)
{ }

#ifdef __WXMSW__
    typedef unsigned int  uint;
    #include "json/json-forwards.h"
    
#endif

#include <string>
#include <memory>
#include "msgexception.h"

#endif // _H_AGW_FIRST