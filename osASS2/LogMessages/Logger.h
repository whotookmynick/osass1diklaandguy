/*
 * Logger.h
 *
 *  Created on: Jul 6, 2009
 *      Author: dikla
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <pthread.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

#define LOG_DEBUG(m) \
{ \
    std::stringstream s; \
    s <<"[DEBUG, "<<(pthread_self()) \
      <<" , in function "<<__FUNCTION__<<"  in file : "<<__FILE__<<" : at line "<<__LINE__<<"] "<<m; \
    std::cerr << (s.str()); \
}

#endif /* LOGGER_H_ */

