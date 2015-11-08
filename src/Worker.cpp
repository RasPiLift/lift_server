#include "Worker.h"
#include <boost/foreach.hpp>
#include <wiringPi.h>

Worker::Worker(std::list<level_s> levels, L293d &l293dObj) : levels(levels), l293dRef_(l293dObj) {
	between1 = 0;
	between2 = 0;
	
	BOOST_FOREACH(level_s level, levels) {
		pinMode(level.sensor, INPUT);
		pinMode(level.led, OUTPUT);
	}
}

void Worker::start() {
	BOOST_FOREACH(level_s level, levels) {
		threads.add_thread(new boost::thread(&Worker::processQueue, this, level));
	}
	moverthread = boost::thread(&Worker::mover, this);
}

void Worker::join() {
	threads.join_all();
	moverthread.join();
}

void Worker::sleep(int ms) {
	boost::posix_time::milliseconds workTime(ms);
	boost::this_thread::sleep(workTime);
}

void Worker::processQueue(level_s level) {
	while(true) {
		int l = 0;
		bool bs = false;
		
		while(digitalRead(level.sensor) == LOW)  {
			l++;
			if (l > 24 && !bs) {
				bs = true;
				between1 = level.index;
				between2 = level.index;
				toggleLight();
			}
			sleep(1);
		}
		
		if (bs) {
			calculateBetweenDirection();
			toggleLight();
		}
		
		sleep(1);           
	}
}

void Worker::mover() {
	while (true) {
		if (!levelqueue.empty()) {
			level_s ls = levelqueue.front();
			
			while (true) {
				if (between1 == between2) {
					if (digitalRead(ls.sensor) == LOW) {
						l293dRef_.stopMotor();
						break;
					} else if (between1 > ls.index) {
						l293dRef_.setDirection(false);
					} else if (between1 < ls.index) {
						l293dRef_.setDirection(true);
					}
				} else if (between1 != between2) {
					if (between1 > between2) {
						if (ls.index > between2) {
							l293dRef_.setDirection(true);
						} else if (ls.index < between2) {
							l293dRef_.setDirection(false);
						}
					} else if (between1 < between2) {
						if (ls.index > between1) {
							l293dRef_.setDirection(true);
						} else if (ls.index < between1) {
							l293dRef_.setDirection(false);
						}
					}
				}
				l293dRef_.startMotor();
				sleep(1);
			}
			
			levelqueue.pop();
		}
		
		sleep(2000);
	}
}

void Worker::toggleLight() {
	int led1, led2;
	BOOST_FOREACH(level_s level, levels) {
		digitalWrite(level.led, LOW);
		if (level.index == between1) {
			led1 = level.led;
		} else if (level.index == between2) {
			led2 = level.led;
		}
	}
	
	if (between1 == between2) {
		digitalWrite(led1, HIGH);
	} else {
		digitalWrite(led1, HIGH);
		digitalWrite(led2, HIGH);
	}
}

void Worker::calculateBetweenDirection() {
	if (between2 == levels.front().index) {
		l293dRef_.setDirection(true);
	} else if (between2 == levels.back().index) {
		l293dRef_.setDirection(false);
	}
	
	between2 = between1 + (l293dRef_.getDirection() ? 1 : -1);
}

std::string Worker::printPosition() {
	std::string b1, b2;
	BOOST_FOREACH(level_s level, levels) {
		if (level.index == between1) {
			b1 = level.label;
		} else if (level.index == between2) {
			b2 = level.label;
		}
	}
	
	if (between1 == between2) {
		return b1;
	} else {
		return std::string(b1) + std::string(l293dRef_.getDirection() ? " → " : " ← ") + std::string(b2);
	}
	
	return "";
}

std::string Worker::getBetween1Label() {
	BOOST_FOREACH(level_s level, levels) {
		if (level.index == between1) {
			return level.label;
		}
	}
	return "";
}

std::string Worker::getBetween2Label() {
	BOOST_FOREACH(level_s level, levels) {
		if (level.index == between2) {
			return level.label;
		}
	}
	return "";
}

void Worker::moveToIndex(int target) {
	BOOST_FOREACH(level_s level, levels) {
		if (level.index == target) {
			levelqueue.push(level);
		}
	}
	moverthread.join();
}

void Worker::moveToLabel(std::string target) {
	BOOST_FOREACH(level_s level, levels) {
		if (level.label == target) {
			levelqueue.push(level);
		}
	}
	moverthread.join();
}

void Worker::manual() {
	moverthread.interrupt();
	
	std::queue<level_s> empty;
	std::swap(levelqueue, empty);
}
