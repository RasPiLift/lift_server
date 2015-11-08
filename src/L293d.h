#ifndef L293D_H
#define L293D_H

#include "Config.h"

class L293d {
private:
	motor_s motor;
	
	bool direction;
	bool running;
	
public:	
	L293d(motor_s motor);
	
	void startMotor();
	void stopMotor();
	
	bool getDirection();
	void setDirection(bool direction);
	void changeDirection();
	
	bool isRunning();
	
	int getPwmValue();
	void setPwmValue(int pwmValue);
	
	int getPwmRange();
};

#endif
