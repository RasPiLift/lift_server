#ifndef WORKER_H
#define WORKER_H

#include <boost/thread.hpp>
#include <queue>
#include <string>
#include "L293d.h"
#include "Config.h"

class Worker {
private:
	int between1;
	int between2;
	
	std::list<level_s> levels;
	
	boost::thread_group threads;
	
	boost::thread moverthread;
	std::queue<level_s> levelqueue;
	
public:
	L293d& l293dRef_;
	
	Worker(std::list<level_s> levels, L293d &l293dObj);

	void start();
	void join();
	void sleep(int ms);
	void processQueue(level_s level);
	void mover();
	void toggleLight();
	void calculateBetweenDirection();
	std::string printPosition();
	std::string getBetween1Label();
	std::string getBetween2Label();
	void moveToIndex(int target);
	void moveToLabel(std::string target);
	void manual();
};

#endif
