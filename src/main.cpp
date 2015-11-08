#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <wiringPi.h>
#include <iostream>
#include "Config.h"
#include "L293d.h"
#include "Worker.h"
#include "JsonServer.h"

using namespace std;
using namespace jsonrpc;

int main(int argc, char* argv[]) {
	cout << "main: startup" << endl;
	
    try {
        Config config("config.xml");
        
        wiringPiSetup();

		L293d l(config.motor);
		Worker w(config.levels, l);
		
		HttpServer server(config.jsonserver_port);
		JsonServer j(server, w);
		
		w.start();
		j.start();
		
		w.join();
		j.join();
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
	cout << "main: done" << endl;
	
	return 0;
}
