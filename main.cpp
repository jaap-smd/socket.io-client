#include <sio_client.h>
// #include "lib/json/single_include/nlohmann/json.hpp"

#include "hub.hpp"

#include <memory>
#include <pthread.h>
#include <string>

// using json = nlohmann::json;

std::string hub_address = "http://10.0.0.88:3000";

int main(int argc, char *argv[]) {
    // connect to hub
    auto sioclient = std::make_shared<sio::client>();
    auto hub = std::make_shared<Hub>(sioclient, hub_address);
    hub->start();
    
    
    
    
    
    
    
    // end program
    hub->stop();
}

// @todo: 
// arguments:
// hub address, logfile

// json register string
// register ack callback
// disconnect

