#include "JsonServer.h"
#include <wiringPi.h>
#include <string>

using namespace std;
using namespace jsonrpc;

JsonServer::JsonServer(HttpServer &server, Worker &workerObj) : AbstractServer<JsonServer>(server), workerRef_(workerObj) {
	this->bindAndAddMethod(Procedure("getStatus", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::getStatus);
	this->bindAndAddMethod(Procedure("startMotor", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::startMotor);
	this->bindAndAddMethod(Procedure("stopMotor", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::stopMotor);
	this->bindAndAddMethod(Procedure("setDirection", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::setDirection);
	this->bindAndAddMethod(Procedure("changeDirection", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::changeDirection);
	this->bindAndAddMethod(Procedure("setPwmValue", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::setPwmValue);
	this->bindAndAddMethod(Procedure("moveToIndex", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::moveToIndex);
	this->bindAndAddMethod(Procedure("moveToLabel", PARAMS_BY_NAME, JSON_STRING, NULL), &JsonServer::moveToLabel);
}

void JsonServer::start() {
	m_Thread = boost::thread(&JsonServer::processQueue, this);
}

void JsonServer::join() {
	m_Thread.join();
}

void JsonServer::processQueue() {
	try {
		if (StartListening()) {
			cout << "JsonRPC-Server started" << endl;
			getchar();
			StopListening();
		} else {
			cout << "Error starting JsonRPC-Server" << endl;
		}
	} catch (jsonrpc::JsonRpcException& e) {
		cerr << e.what() << endl;
	}
}

void JsonServer::getStatus(const Json::Value& request, Json::Value& response) {
	response["position"] = workerRef_.printPosition();
	response["between1Label"] = workerRef_.getBetween1Label();
	response["between2Label"] = workerRef_.getBetween2Label();
	response["direction"] = workerRef_.l293dRef_.getDirection();
	response["pwmValue"] = workerRef_.l293dRef_.getPwmValue();
	response["pwmRange"] = workerRef_.l293dRef_.getPwmRange();
}

void JsonServer::startMotor(const Json::Value& request, Json::Value& response) {
	workerRef_.manual();
	workerRef_.l293dRef_.startMotor();
}

void JsonServer::stopMotor(const Json::Value& request, Json::Value& response) {
	workerRef_.manual();
	workerRef_.l293dRef_.stopMotor();
}

void JsonServer::setDirection(const Json::Value& request, Json::Value& response) {
	workerRef_.manual();
	try {
		workerRef_.l293dRef_.setDirection(request[0].asBool());
	} catch (...) { }
}

void JsonServer::changeDirection(const Json::Value& request, Json::Value& response) {
	workerRef_.manual();
	workerRef_.l293dRef_.changeDirection();
}

void JsonServer::setPwmValue(const Json::Value& request, Json::Value& response) {
	try {
		workerRef_.l293dRef_.setPwmValue(request[0].asInt());
	} catch (...) { }
}

void JsonServer::moveToIndex(const Json::Value& request, Json::Value& response) {
	try {
		workerRef_.moveToIndex(request[0].asInt());
	} catch (...) { }
}

void JsonServer::moveToLabel(const Json::Value& request, Json::Value& response) {
	try {
		workerRef_.moveToLabel(request[0].asString());
	} catch (...) { }
}
