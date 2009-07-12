/*
 * UI.h
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#ifndef UI_H_
#define UI_H_
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <pthread.h>
#include <sstream>
#include <unistd.h>
#include "../SystemCalls/SystemCalls.h"

const int READ_ONLY = 1;
const int READ_AND_WRITE = 2;

using namespace std;

/*
All commands should return the output specified in the command if successful or a specification of the error. If no output is specified, return the output 1 when the command succeeded, or 0 if failed. Error description should be output to stderr. Support the following commands:
*/
class OSUI
{

private:

	pthread_t ui_thread;
	SystemCalls *_systemCallsCaller;
	string _pwd;
	vector<int>* _fdTable;
	int _pid;
	vector<OSUI*>* _processTable;

	/*------------------------ HELP FUNCTIONS ---------------------*/
	string getRealPWD();
	void goDownDir();
	void init();
	bool processExists(int pid);

	/* END HELP FUNCTIONS */
public:

        OSUI(SystemCalls* systemCallsCaller);
        OSUI(SystemCalls* systemCallsCaller,vector<int>* fdTable,int pid,string pwd,vector<OSUI*>* processTable);
        virtual ~OSUI();

        void run();

        /*
         * makes a directory with the given name.
         */
        int mkdir (string dir_name);

        /*
         * creates an empty file. Returns a file descriptor <fd).
         */
        int create (string file_name,string flags);
        /*
         * creates a hard link to file-to-link-to. Making a hard link to directories is not allowed.
         */
        int hdlink(string file_to_link_to,string new_file_name);
        /*
         * -creates a soft link to dest-to-link-to. dest can be either a directory or a file.
         */
        int sflink(string dest_to_link_to,string new_file_name);
        /*
         * opens an existing file returning a file descriptor (fd). The fd returned should always be the lowest one still free. Flag can be set to "read_only" or "read_and_write". Always remember to check when reading or writing that the file was actually opened for that purpose, otherwise return an error. Your flags should be in the exact syntax specified.
         */
        int open (string file_name,int flags);
        /*
         *  _ close the file associated with the given file descriptor. Release locks on this file held by this process.
         */
        int close (int fd);
        /*
         *  _ Read num_bytes from the file that is identified by int fd. Returns the string read, followed by a newline and then the number of bytes that were actually read. Remember read uses the current file pointer to know where to start reading from, and the operation advances the file pointer. The output should be shown on stdout.
         */
        int read (int fd,int num_bytes);
        /*
         *  writes the string to the file. Returns the number of bytes actually written.  Remember write uses the current file pointer to know where to start writing from, and the operation advances the file pointer.
         */
        int write (int fd,string buffer);
        /*
         *imports a real file from linux's file system from the given address into the file int fd. In this way files can be imported into the system.
         */
        int writeFile (int fd,string fileName);
        /*
         *locks file, so that only read operations are allowed. There is no limit on the number of processes permitted to read the file. No processes are allowed to write to the file while a process holds a read lock to the file. It is not obligatory to lock the file to read it. You should not block if it is locked, rather return an error.
         */
        int lck_rd (int fd);
        /*
         * locks file, so that only one process may hold the lock. This process may write to the file, and no other processes may write to it or read from it. A child does not inherit this lock. It is not obligatory to lock the file to write to it. You should not block if it is locked, rather return an error.
         */
        int lck_wr(int fd);
        /*
         * releases the reading lock held by this process. No other process may release this lock.
         */
        int rlslck_rd(int fd);
        /*
         * releases the writing lock held by this process. Only the process which locked the file may release the lock.
         */
        int rlslck_wr(int fd);
        /*
         *moves the file pointer to the given offset from start of file. If the pointer is set to an offset after the end of the file, this is o.k. If data is later written at this point, subsequent reads of the data in the gap return bytes of zeros (until data is actually written into the gap). The size of the file should not be updated until data is written to that point.
         */
        int lseek (int fd,int offset);
        /*
         *deletes the file.
         */
        int rm (string file_name);
        /*
         *deletes the directory. A directory can only be deleted if it is empty.
         */
        int rmdir (string dir_name);
        /*
         * recursively deletes directory, all sub_directories and all files. If one of the files is open, terminate and return error. It is o.k. if the system starts deleting files and then finds one open, to then return an error. This means that the command may be executed partially.
         */
        int rmdir_r(string dir_name);
        /*
         *returns a list of the files in the given directory, each file in a different line and including the file_name and its size (in bytes). If ls is given without an argument, the ls of the current directory should be returned. Output should be as follows: each file/dir in the given directory in a separate line, directories have the "/" char appended to the end of their name.
         */
        list<string> ls (string dir_name);
        /*
         * changes the old_file_name to new file name. This can be just changing its name in the same directory, or even moving the file to a different directory.
         */
        int mv(string old_file_name,string new_file_name);
        /*
         *changes current working directory of the current process to new_directory.
         */
        int cd(string new_directory );
        /*
         * Switches to the context of a different process. Make sure to also switch to the new process' int fd table and working directory.
         */
        int swprc(int new_process_num);
        /*
         * Creates a process with the given id. If a process with this id already exists, return an error.
         * If parent == -1, this process is created with no open files.
         * If parent ) _1, this process is defined as the child of the process with id parent.
         * This means that it is created with an exact copy of the int fd table of the parent.
         * If process with id parent doesn’t exist return an error.
         * This function doesn’t switch to the context of the newly created process,
         * rather it stays in the context of the current process.
         */
        int crprc(int id, int parent);
        /*
         * This function is needed to test the file system intensively.
         * It allows executing many commands quickly from a file given as input.
         * Do not wait for a command to return before dispatching the next command.
         * For example, if requested to read a large amount of data,
         * the shell should be able to receive another command even though the read command hasn't finished yet.
         * Each command has the following syntax: [commandName][space][first arg][space][second arg] etc.
         * Each space will be exactly one space. Each command is terminated by a '\n'.
         */
        int batch(string file_name);

        string getPwd() const
        {
            return _pwd;
        }

        void setPwd(string _pwd)
        {
            this->_pwd = _pwd;
        }

        int getPid() const
        {
            return _pid;
        }

        void setPid(int _pid)
        {
            this->_pid = _pid;
        }

};


#endif /* UI_H_ */

