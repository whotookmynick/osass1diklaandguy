/*
 * mainFile.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include <iostream>
#include "../LibraryFunctionsUI/UI.h"

using namespace std;

int main(int argc,char** argv)
{
	cout<<"not implemented yet"<<endl;
}


// how to check if a file exists
//#include <sys/stat.h>
//
//bool FileExists(string strFilename) {
//  struct stat stFileInfo;
//  bool blnReturn;
//  int intStat;
//
//  // Attempt to get the file attributes
//  intStat = stat(strFilename.c_str(),&stFileInfo);
//  if(intStat == 0) {
//    // We were able to get the file attributes
//    // so the file obviously exists.
//    blnReturn = true;
//  } else {
//    // We were not able to get the file attributes.
//    // This may mean that we don't have permission to
//    // access the folder which contains this file. If you
//    // need to do that level of checking, lookup the
//    // return values of stat which will give you
//    // more details on why stat failed.
//    blnReturn = false;
//  }
//
//  return(blnReturn);
//}