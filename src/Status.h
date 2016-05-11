/*
 * Status.h
 *
 *  Created on: Feb 23, 2016
 *      Author: luke
 */

#ifndef STATUS_H_
#define STATUS_H_

#define DISCONNECTED -1
#define IDLE 0
#define BREW_HEAT 1
#define BREW_READY 2
#define STEAM_HEAT 3
#define STEAM_READY 4

struct StatusPacket {
	short temp, state;
};

class StatusClass {
public:
	//public stuff
	StatusClass();
	bool isConnected();
	void refresh();
	int getTemp();
	short getState();
	StatusPacket getStatusPacket();
private:
	StatusPacket _status;
	//private stuff;
};

extern StatusClass Status;

#endif /* STATUS_H_ */
