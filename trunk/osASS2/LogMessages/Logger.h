/*
 * Logger.h
 *
 *  Created on: Jul 6, 2009
 *      Author: dikla
 */

#ifndef LOGGER_H_
#define LOGGER_H_
//extern ostream cerr;
#include <string>
#include <string.h>
#include <sstream>
using namespace std;

#define LOG_DEBUG(m) \
{ \
    stringstream s; \
    s <<"[DEBUG, "<<(pthread_self()) \
      <<" , in function "<<__FUNCTION__<<"  in file : "<<__FILE__<<" : at line "<<__LINE__<<"] "<<m; \
    cerr << (s.str()); \
}

#endif /* LOGGER_H_ */

