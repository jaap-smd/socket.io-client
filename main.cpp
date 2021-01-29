#include "lib/socket.io-client-cpp/src/sio_client.h"
#include "lib/json/single_include/nlohmann/json.hpp"

#include <memory>
#include <pthread.h>
#include <string>

using json = nlohmann::json;

// std::shared_ptr<sio::client> sioclient(nullptr);
std::string hub = "10.10.10.10:3000";

int main(int argc, char *argv[]) {
    // connect to hub
    auto sioclient = std::make_shared<sio::client>();
    sioclient->connect(hub);
      
    
    
    
    
    
    
    // end program
    sioclient->close();
}
