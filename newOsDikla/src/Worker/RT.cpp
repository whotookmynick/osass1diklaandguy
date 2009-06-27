/*
 * RT.cpp
 *
 *  Created on: Jun 26, 2009
 *      Author: dikla
 */

#include "RT.h"

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

RT::RT(int targetNumber,int owner):
	_numberOfTargets(targetNumber),_owner(owner),_rtTable() {
	_rtTable.resize(_numberOfTargets+1);
	initRT();
}

RT::~RT() {

}

void RT::initRT(){
	for(int i=1; i<=_numberOfTargets;i++){
		_rtTable[i]->target = i;
		if( i==_owner){
			_rtTable[i]->distance = 0;
			_rtTable[i]->next = i;
		}
		else{
			_rtTable[i]->distance = -1;//infinit
			_rtTable[i]->next = -1;//empty
		}//end else
	}//end for
}//end initRT

//---------------------------------------------------------------------------------
//							get RT and Change minimal distance if needed
//--------------------------------- -----------------------------------------------

//return true if my rt was change
bool RT::setRT(RT* nighborRT){
	bool setMinimal = false;
	vector<rtStruct*> yourRt = nighborRT->getRtTable();
	for(int i=1; i<=_numberOfTargets;i++){
		if(i==_owner){
			//do nothing
		}else{
			if(yourRt[i]->distance == -1){
				//do nothing
			}//end if
			else if(yourRt[i]->distance<_rtTable[i]->distance){
				_rtTable[i]->distance = yourRt[i]->distance+1;
				setMinimal = true;//my rtTable was change
			}//end else if
		}//end else
	}//end for
	return setMinimal;
}

void RT::setRtDis(int target,int dis,int next){
	_rtTable[target]->distance = dis;
	_rtTable[target]->next = next;
}


//---------------------------------------------------------------------------------
//									getters and setters
//--------------------------------- -----------------------------------------------

vector<rtStruct*> RT::getRtTable(){
	return _rtTable;
}

int RT::getID(){
			return _owner;
}


//---------------------------------------------------------------------------------
//									print
//--------------------------------- -----------------------------------------------

void RT::printRT(){
	cout<<" prit RT of worker " << _owner<<endl;
	cout<< " ********************************* " <<endl;
	for(int i=1; i<=_numberOfTargets;i++){

		int dis =  _rtTable[i]->distance;
		int tar =  _rtTable[i]->target;
		int next = _rtTable[i]->next;

		cout<<" target: "<<tar;
		cout<<" distance: "<<dis;
		cout<<" next: "<<next<<endl;
		cout<<" ------------------------------------------- "<<endl;
	}//end for
}//end printRT




