#ifndef HUB_HPP
#define HUB_HPP

#include <sio_client.h>
// #include "lib/json/single_include/nlohmann/json.hpp"

#include <fstream>
#include <map>

class Hub
{
  public:
    Hub(std::shared_ptr<sio::client> host, std::string hub_address = "http://10.10.10.10:3000");

  private:    
    void connect();
    
    // socketio callbacks:
    const void on_connected();
    
    // void on_register(sio::event &ev);
    void on_register(const sio::message::list &msglist);
    
    void on_disconnect(sio::event &ev);

  public:
    void start();
   
    void stop();


  private:
    std::shared_ptr<sio::client> _host;
    
    std::ofstream _logfile;
    
    std::string _logfile_name = "logfile.txt";

    std::string _hub_address;

    bool _connected = false;
};


#endif // HUB_HPP
