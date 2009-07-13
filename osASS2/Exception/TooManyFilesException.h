/*
 * TooManyFilesException.h
 *
 *  Created on: Jul 13, 2009
 *      Author: noam
 */

#ifndef TOOMANYFILESEXCEPTION_H_
#define TOOMANYFILESEXCEPTION_H_


#include <stdexcept>

class TooManyFilesException : std::exception{
public:
	TooManyFilesException(){};
	virtual ~TooManyFilesException()throw(){};

	virtual const char* what() const throw()
	{
		return "Too Many Open Files\n";
	}

};

#endif /* TOOMANYFILESEXCEPTION_H_ */
