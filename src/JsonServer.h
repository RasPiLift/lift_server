#ifndef JSONSERVER_H
#define JSONSERVER_H

#include <boost/thread.hpp>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>
#include "Worker.h"

using namespace jsonrpc;

class JsonServer : public AbstractServer<JsonServer> {
private:
	boost::thread m_Thread;
	
	Worker& workerRef_;
	
public:
	JsonServer(HttpServer &server, Worker &workerObj);
	void start();
	void join();
	void processQueue();
	void getStatus(const Json::Value& request, Json::Value& response);
	void startMotor(const Json::Value& request, Json::Value& response);
	void stopMotor(const Json::Value& request, Json::Value& response);
	void setDirection(const Json::Value& request, Json::Value& response);
	void changeDirection(const Json::Value& request, Json::Value& response);
	void setPwmValue(const Json::Value& request, Json::Value& response);
	void moveToIndex(const Json::Value& request, Json::Value& response);
	void moveToLabel(const Json::Value& request, Json::Value& response);
};

#endif
