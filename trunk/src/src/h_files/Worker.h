/*
 * Worker.h
 *
 *  Created on: Jun 17, 2009
 *      Author: dikla
 */

#ifndef WORKER_H_
#define WORKER_H_

#include "Mailer.h"
#include "utils.h"
using namespace std;

class Mailer;

class Worker : public OSThread
{
	private:
		bool m_worker_exit;
		int m_worker_id;
		vector<int> m_workers_id;
		Mailer* m_mailer;
		RoutingTable* m_rt;
		priority_queue<Message*> m_inbox;

	public:
		Worker();
		Worker(int worker_num, vector<int>& workers_id, vector<int>& neighbors, Mailer* mailer);
		void run();
		bool studentSend(void *args);
		bool studentRecv(void **args);
		void print_rt();
		virtual ~Worker();
};



#endif /* WORKER_H_ */
