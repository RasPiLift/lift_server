#include "L293d.h"
#include <softPwm.h>

L293d::L293d(motor_s motor) : motor(motor) {
	softPwmCreate(motor.pin1, 0, motor.pwmRange);
	softPwmCreate(motor.pin2, 0, motor.pwmRange);
	
	direction = true;
	running = false;
}

void L293d::startMotor() {
	if (direction) {
		softPwmWrite(motor.pin1, 0);
		softPwmWrite(motor.pin2, motor.pwmValue);
		running = true;
	} else {
		softPwmWrite(motor.pin1, motor.pwmValue);
		softPwmWrite(motor.pin2, 0);
		running = true;
	}
}

void L293d::stopMotor() {
	softPwmWrite(motor.pin1, 0);
	softPwmWrite(motor.pin2, 0);
	running = false;
}

bool L293d::getDirection() {
	return direction;
}

void L293d::setDirection(bool direction) {
	this->direction = direction;
	if (running) {
		stopMotor();
		startMotor();
	}
}

void L293d::changeDirection() {
	direction = !direction;
	if (running) {
		stopMotor();
		startMotor();
	}
}

bool L293d::isRunning() {
	return running;
}

int L293d::getPwmValue() {
	return motor.pwmValue;
}

void L293d::setPwmValue(int pwmValue) {
	motor.pwmValue = pwmValue;
	if (running) {
		if (direction) {
			softPwmWrite(motor.pin2, motor.pwmValue);
		} else {
			softPwmWrite(motor.pin1, motor.pwmValue);
		}
	}
}

int L293d::getPwmRange() {
	return motor.pwmRange;
}
