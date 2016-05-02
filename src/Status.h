/*
 * Status.h
 *
 *  Created on: Feb 23, 2016
 *      Author: luke
 */

#ifndef STATUS_H_
#define STATUS_H_

struct StatusPacket {
	short temp, state;
};

class StatusClass {
public:
	//public stuff
	StatusClass();
	bool isConnected();
	int getTemp();
	int getState();
private:
	StatusPacket _status;
	//private stuff;
};

extern StatusClass Status;

#endif /* STATUS_H_ */
