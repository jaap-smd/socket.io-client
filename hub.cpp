#include "hub.hpp"

#include <ctime>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

Hub::Hub(std::shared_ptr<sio::client> host, std::string hub_address)
    : _host(host),
    _hub_address(hub_address)
{
}

void Hub::connect()
{
    try
    {
        // reconnect options
        _host->set_reconnect_attempts(1000);
        _host->set_reconnect_delay(5000);
        _host->set_reconnect_delay_max(100000);
                
        while (!_connected)
        {
            std::cerr << "connecting\n";

            // connect to hub
            _host->connect(_hub_address);

            std::this_thread::sleep_for(
                std::chrono::milliseconds{100}); // wait before checking the connection
            if (_connected)
            {
                break;
            }
            std::cerr << "WARNING: Failed to connect to hub: " << _hub_address
                      << "\nReconnect in 2 seconds." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds{2}); // try to reconnect
        }
    }
    catch (...)
    {
        std::cerr << "Something did go wrong in connecting to the hub.\n";
    }
}


// callback functions:
const void Hub::on_connected()
{
    std::cerr << "on_connected called" << std::endl;
    _connected = true;

    // CAMERA SETTINGS:
    
    // setting1 number range:
    std::map<std::string, sio::message::ptr> setting1_map_prop {
        {"min", sio::string_message::create("-4")},
        {"max", sio::string_message::create("4")},
        {"step", sio::string_message::create("1")},
        {"value", sio::string_message::create("0")}
    };
    auto setting1_prop = sio::object_message::create();
    setting1_prop->get_map() = setting1_map_prop;
    std::map<std::string, sio::message::ptr> setting1_map {
        {"name", sio::string_message::create("ev")},
        {"display_name", sio::string_message::create("EV")},
        {"properties", setting1_prop}
    };
    auto setting1 = sio::object_message::create();
    setting1->get_map() = setting1_map;

    // setting2 option selection:
    std::vector<sio::message::ptr> setting2_params {
        {sio::string_message::create("0 standard external")},
        {sio::string_message::create("3 Skip frames")},
        {sio::string_message::create("14 Overlapped exposure")},
        {sio::string_message::create("15 Multi-shot")}
    };
    auto setting2_prop_params = sio::array_message::create();
    setting2_prop_params->get_vector() = setting2_params;
    std::map<std::string, sio::message::ptr> setting2_map_prop {
        {"params", setting2_prop_params},
        {"value", sio::string_message::create("0 standard external")}
    };
    auto setting2_prop = sio::object_message::create();
    setting2_prop->get_map() = setting2_map_prop;
    std::map<std::string, sio::message::ptr> setting2_map {
        {"name", sio::string_message::create("trigmode")},
        {"display_name", sio::string_message::create("Trigger mode")},
        {"properties", setting2_prop}
    };
    auto setting2 = sio::object_message::create();
    setting2->get_map() = setting2_map;


    // add settings to object:
    std::vector<sio::message::ptr> settings_array {
        {setting1},
        {setting2}
    };
    auto settings = sio::array_message::create();
    settings->get_vector() = settings_array;

    // create register object
    std::map<std::string, sio::message::ptr> reg_basics {
        {"vendor", sio::string_message::create("FLIR")},
        {"id", sio::string_message::create("camera20001")},
        {"type", sio::string_message::create("worker")},
        {"display_name", sio::string_message::create("ladybug01")},
        {"settings", settings}
    };
    auto reg = sio::object_message::create();
    reg->get_map() = reg_basics;
    
    
    
    // send register object 
    _host->socket()->emit("register", reg, std::bind(&Hub::on_register, this, std::placeholders::_1));
    // void emit(std::string const& name, message::list const& msglist, std::function<void (message::ptr const&)> const& ack)

}

void Hub::on_register(const sio::message::list &msglist)
{
    std::cerr << "on_register called" << std::endl;
    _connected = true;

    try
    {
        // const sio::message::list &msglist(ev.get_messages());
        for(int i = 0; i != msglist.size(); i++) {
            auto &element = msglist.at(i);
            auto msg_map = element->get_map();
            auto return_code = msg_map.find("code"); // ->second->get_string();
            // std::cerr << "return code: " << return_code << std::endl;
            // std::string message = element->get_map().find("message")->second->get_string();
            // std::cerr << "msglist index: " << i << ". Register code: " << return_code << ". Message: " << message << std::endl;
        }
    }
    catch (...)
    {
        std::cerr << "Something did go wrong in Socketio connection.\n";
    }
}

void Hub::on_disconnect(sio::event &ev)
{
    try
    {
        const sio::message::list &msglist(ev.get_messages());

        auto &element = msglist.at(0);

        std::string camera = element->get_map().find("id")->second->get_string();
        std::string value = element->get_map().find("value")->second->get_string();
    }
    catch (...)
    {
        std::cerr << "Something did go wrong in Socketio connection.\n";
    }
}


// Start stop functions
void Hub::start() {
    _host->set_open_listener(std::bind(&Hub::on_connected, this));
    
    _host->socket()->on(
        "disconnected",
        std::bind(&Hub::on_disconnect, this, std::placeholders::_1));

    try {
        _logfile.open(_logfile_name, std::ios::out | std::ios::app);

        while (true) {
            if (!_connected) {
                connect();
            }

            _logfile.flush();
        }
        
        _logfile.close();

    }
    catch (...) {
        std::cerr << "WARNING: something went wrong in connecting to the hub." << std::endl;
    }
}


void Hub::stop() {
	_host->close();
	std::cerr << "Disconnected from hub." << std::endl;
}
