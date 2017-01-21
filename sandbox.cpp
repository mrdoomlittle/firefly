# include "src/ffly_client.hpp"
# include <iostream>
int main() {
    mdl::client_settings_t client_settings;

    client_settings.server_ip_addr = "192.168.0.100";
    client_settings.server_port_num = 21299;

    mdl::init_options_t init_options;
   
    init_options.internal_loop = true;
    init_options.no_graphics = false;

    mdl::ffly_client fflyc(12, 12, 12);
    fflyc.initialize(128, 128, "FireFly Engine", init_options);
}
