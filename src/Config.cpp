#include "Config.h"
#include <exception>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
Config::Config(const std::string &filename) {
    using boost::property_tree::ptree;
    ptree pt;
    
    read_xml(filename, pt);
    
    jsonserver_port = pt.get<int>("lift.jsonserver.port");
	
	int index = 0;
	BOOST_FOREACH(ptree::value_type &v, pt.get_child("lift.levels")) {
		level_s ls;
		
		ls.index = index++;
		ls.label = v.second.get<std::string>("label");
		ls.sensor = v.second.get<int>("sensor");
		ls.led = v.second.get<int>("led");
		
		levels.push_back(ls);
	}
	
	motor.pin1 = pt.get<int>("lift.motor.pins.pin1");
	motor.pin2 = pt.get<int>("lift.motor.pins.pin2");
	motor.pwmValue = pt.get<int>("lift.motor.pwm.value");
	motor.pwmRange = pt.get<int>("lift.motor.pwm.range");
}
