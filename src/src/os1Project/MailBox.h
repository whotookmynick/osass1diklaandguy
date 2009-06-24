/*
 * MailBox.h
 *
 *  Created on: Jun 24, 2009
 *      Author: dikla
 */

#ifndef MAILBOX_H_
#define MAILBOX_H_

#include "Message.h"
#include <pthread.h>
#include <queue>

class Mailbox {
	private:

		pthread_mutex_t _QMutex;
		std::queue<Message*> _sysQueue;
		std::queue<Message*> _RegQueue;


	public:
			Mailbox();
			virtual ~Mailbox();

			bool readMsg(std::queue<Message*>& destinyMsgQ);// Reads messages from the mailbox to destinyMsgQ.
															//System messages are read first.true on success
			void insertMsg(Message* m);//insert message in the mailbox
			void insertMsToSystemQ(Message* m);
			void insertMstToregularQ(Message* m);

};




#endif /* MAILBOX_H_ */
