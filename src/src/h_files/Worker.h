/*
 * Worker.h
 *
 *  Created on: Jun 24, 2009
 *      Author: dikla
 */

#ifndef WORKER_H_
#define WORKER_H_

#include "Message.h"
#include "OSThread.h"
#include "Mailer.h"
#include "RT.h"
#include <stdlib.h>
#include <vector>
#include <queue>
#include <ostream>
#include <set>
#include <pthread.h>


using namespace std;

class Worker : public OSThread
{
private:


	int _numOfWorkers;
	vector <Message> massagebourd;
	RT* myRT;
	//Mailer & _mailer;
    std::set<int> _myNeighborsId;
    int _workerId;
    RT _rtTable;
    std::vector<RT> _allRTs;
    bool _isOn;
    pthread_mutex_t _enabledBusy;
    pthread_cond_t _enabledCond;

public:
        /**
         * constractor to new worker
         */

		Worker();

        virtual ~Worker();
        void printRT(std::ostream& out);
        int getID() const;
        void stopWorker();
        void restartWorker();
        bool isOn();
        virtual void run();
        virtual bool studentSend(void *arg);
        virtual bool studentRecv(void **args);

private:
		void waitToDisabledSignal();
		void sendMyRTtoAllNighbors();
		void send(const Message& m);
		void recv(std::queue<Message>& into);
		void getMsg(queue<Message*>& messages);


};




#endif /* WORKER_H_ */
