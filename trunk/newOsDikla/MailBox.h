
#include "SysMsg.h"
#include "RegMsg.h"
#include "InitMsg.h"
#include <pthread.h>
#include <queue>
#include <list>
using namespace std;
class Mailbox {
	private:

		pthread_mutex_t _QMutex;
		list<SysMsg*> _sysQueue;//TODO remove
		list<RegMsg*> _RegQueue;
		list<InitMsg*> _IntQueue;

	public:
			Mailbox();
			virtual ~Mailbox();

			bool readMsg(std::queue<Message*>& destinyMsgQ);// Reads messages from the mailbox to destinyMsgQ.
															//System messages are read first.true on success
			bool insertMsg(Message* m);//insert message in the mailbox

};
