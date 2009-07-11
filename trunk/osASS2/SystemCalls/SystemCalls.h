#ifndef SYSTEMCALLS_H_
#define SYSTEMCALLS_H_


class SystemCalls
{
private:

public:
	SystemCalls();
	virtual ~SystemCalls();
	/*
	 * creates and opens a new file named ‘file_name’ returning a fd to the new file.
	 * If type is soft-link mark it as such. Opens the file with the specified permissions.
	 */
	int MakeFile(char* file_name,int type,int flag_access_permissions);

	/*
	 * creates a hard link with name file_name, linked to target. Returns a new fd.
	 */
	int MakeHLink(char* target_file_name, char*file_name);

	/*
	 * creates a new directory named ‘dir_name’
	 */
	int MakeDir(char* dir_name);

	/*
	 * removes the directory ‘dir_name’
	 */
	int RmDir(char* dir_name);
	/*
	 * removes the file ‘file_name’
	 */
	int RmFile(char* file_name);
	/*
	 * lists the content of the location_directory. Return list in buf.
	 */
	int ls(char*dir_name, char * buf);

	/*
	 * opens the file ‘file_name’. Needs to deal with soft_links. Opens the file with the specified permissions.
	 */
	int Open(char* filename, int flag_access_permissions);

	/*
	 * closes the entry of the file in the file table, and frees the entry in the fd table.
	 */
	int Close(int fd);
	/*
	 * moves the file curser to ‘location’.
	 */
	int Seek(int fd, int location);
	/*
	 * reads nBytes from the received file into the buffer.
	 */
	int Read (int fd, int nBytes, char *Buffer);

	/*
	 * writes nBytes from buffer into the file.
	 */
	int Write (int fd, int nBytes,char * Buffer);
	/*
	 * Moves the child's i_node from its current directory to new destination.
	 */
	int moveFile(char* parendDir, char * new_dest);
	/*
	 * Ret urns whether this address is a directory or not. Needs to deal with soft_links.
	 */
	bool isDir(char * address);
	/*
	 * locks file, so that only read operations are allowed.
	 * There is no limit on the number of processes that are allowed to concurrently read the file.
	 * No processes are allowed to write to the file if there is an open read lock on the file.
	 * It is not obligatory to lock the file to read it. You should not block if it is locked rather return an error.
	 */
	int lockRead(int fd);
	/*
	 * locks file, so that only one process may hold the lock.
	 * This process may write to the file, and no other files may write to it or read from it.
	 * A child does not inherit this lock. It is not obligatory to lock the file to write to it.
	 * You should not block if it is locked rather return an error
	 */
	int lockWrite(int fd);
	/*
	 * releases the reading lock held by this process. Only the process which locked the file may release the lock.
	 */
	int releaseLockRead(int fd);
	/*
	 * releases the writing lock held by this process. Only the process that locked the file may release this lock.
	 */
	int releaseLockWrite(int fd);
};

#endif /*SYSTEMCALLS_H_*/