#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <list>

struct level_s {
	int index;
	std::string label;
	int sensor;
	int led;
};

struct motor_s {
	int pin1;
	int pin2;
	int pwmValue;
	int pwmRange;
};

class Config {
public:
    int jsonserver_port;
    std::list<level_s> levels;
    motor_s motor;
    Config(const std::string &filename);
};

#endif
