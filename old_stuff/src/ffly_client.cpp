# include "ffly_client.hpp"

mdl::ffly_client::ffly_client(uint_t __parts_x, uint_t __parts_y, uint_t __parts_z) {
    this-> uni_x = (__parts_x * PART_X);
    this-> uni_y = (__parts_y * PART_Y);
    this-> uni_z = (__parts_z * PART_Z);

    printf("ffly client: constructor called.\n");
    printf("uni: x = %ldp, y = %ldp, z = %ldp\n", uni_x, uni_y, uni_z);
}

boost::uint8_t mdl::ffly_client::event_tick() {







    if (!this-> rw_ptr-> isOpen()) return 1;

    sf::Event event;
    while (this-> rw_ptr-> pollEvent(event))
        if (event.type == sf::Event::Closed)
            this-> rw_ptr-> close();

    this-> rw_ptr-> display();
    this-> rw_ptr-> clear();
    return 0; 
}

mdl::ffly_client::~ffly_client() {
    std::free(this-> uni);
}

boost::uint8_t mdl::ffly_client::initialize(uint_t __parts_x, uint_t __parts_y, 
    char const * __wtitle, init_options_t & __init_options) {

    this-> init_options = __init_options;
    
    /* all init stuff hear
    */
   
    this-> uni = new boost::int8_t ** [uni_x];
    for (int_t x = uni_x; x != uni_x; x ++) {
        this-> uni[x] = new boost::int8_t * [uni_y];
        for (int_t y = uni_y; y != uni_y; y ++) {
            this-> uni[x][y] = new boost::int8_t[uni_z];
            for (int_t z = uni_z; x != uni_z; z ++) {
                this-> uni[x][y][z] = 0x0; 
            }
        }
    }

    if (__init_options.no_graphics) return 0; 

    static sf::RenderWindow rr(sf::VideoMode(__parts_x * PART_X, __parts_y * PART_Y), __wtitle);

    this-> rw_ptr = &rr;

    if (!__init_options.internal_loop) return 0;   

    for (;;) {
        if (this-> event_tick()) break;
    }
    
    return 0;
}
