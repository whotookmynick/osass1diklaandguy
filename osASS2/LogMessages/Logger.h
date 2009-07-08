/*
 * Logger.h
 *
 *  Created on: Jul 6, 2009
 *      Author: dikla
 */

#ifndef LOGGER_H_
#define LOGGER_H_


#define LOG_DEBUG(m) \
{ \
    std::stringstream s; \
    s <<"[DEBUG, "<<(pthread_self()) \
      <<" , in function "<<__FUNCTION__<<"  in file : "<<__FILE__<<" : at line "<<__LINE__<<"] "<<m; \
    std::cerr << (s.str()); \
}

#endif /* LOGGER_H_ */

