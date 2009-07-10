/*
 * Exception.cpp
 *
 *  Created on: Jul 10, 2009
 *      Author: dikla
 */
#include <stdexcept>

class EmptyListException : public std::exception {

public:
	EmptyListException(const std::string& listName)
        : std::exception(), _listName(listName){}

	virtual ~EmptyListException() throw ()
	{}

	virtual const char* what() const throw() {
			return ("the list" + _listName + "is empty").c_str();
	}

private:
        const std::string _listName;
};
