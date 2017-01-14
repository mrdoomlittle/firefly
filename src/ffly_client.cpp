# include "ffly_client.hpp"

mdl::ffly_client::ffly_client(uint_t __parts_x, uint_t __parts_y, uint_t __parts_z) {
    this-> uni_x = (__parts_x * PART_X);
    this-> uni_y = (__parts_y * PART_Y);
    this-> uni_z = (__parts_z * PART_Z);

    printf("ffly client: constructor called.\n");
    printf("uni: x = %ldp, y = %ldp, z = %ldp\n", uni_x, uni_y, uni_z);
}

boost::uint8_t mdl::ffly_client::initialize(uint_t __parts_x, uint_t __parts_y, char const * __wtitle) {
    sf::RenderWindow rr(sf::VideoMode(__parts_x * PART_X, __parts_y * PART_Y), __wtitle);

    for (;;) {
        if (!rr.isOpen()) break;

        sf::Event event;    
        while(rr.pollEvent(event))
            if (event.type == sf::Event::Closed)
                rr.close();

        rr.display();
        rr.clear();
    }
    
}
