
#ifndef	_SAMPLE_THREAD_H_
#define	_SAMPLE_THREAD_H_

/* A sample thread class that extends the OSThread class.
 * In this example, the thread starts by "sending" two messages,
 * and "receiving" two messages.
 */

#include "OSThread.h"
#include <string>

using namespace std;

class SampleThread : public OSThread
{
	private:
		/* An inner class that represents a simple message */
		struct Message
		{
			string content;
		};

	public:
		/* C'tor - initializes the SampleThread */
		SampleThread();

		/* D'tor - destroys the SampleThread */
		~SampleThread();

		/* run - the main virtual method of the thread */
		virtual void run();

		/* An example of a send method */
		virtual bool studentSend(void *args);

		/* An example of a receive method */
		virtual bool studentRecv(void **args);

		
};


#endif	// _SAMPLE_THREAD_H_

