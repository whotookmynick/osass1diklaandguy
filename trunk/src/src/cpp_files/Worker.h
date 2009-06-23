/*
 * Worker.h
 *
 *  Created on: Jun 17, 2009
 *      Author: dikla
 */

#ifndef WORKER_H_
#define WORKER_H_

#include "Mailer.h"
#include "OSThread.h"
#include "Massage.h"
#include "RT.h"

class Worker : public OSThread
{
private:
	int workerID;
	vector <int> Nworkers_ID;
	vector <Massage> massagebourd;
	RT* myRT;

};

#endif /* WORKER_H_ */
