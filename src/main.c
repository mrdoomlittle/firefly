
# include "uni.h"
# include "system/io.h"
# include "obj.h"
# include "ui/camera.h"
# include "data/mem_set.h"
# include "system/nanosleep.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <termios.h>
# include <time.h>
# include <linux/input-event-codes.h>
# include <linux/input.h>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "types/byte_t.h"
# include "data/mem_set.h"
# include "obj_manager.h"
# include "polygon.h"
# include "data/bzero.h"
# include "maths/abs.h"
# include "maths/min.h"
# include "maths/max.h"
char depth[] = {'@', '#', '$', '&', '%', '*', '-', '.'};
mdl_int_t const xal = 50;
mdl_int_t const yal = 50;
mdl_int_t const zal = 30;
char *frame;

/*

*/
# include "maths/dot.h"



mdl_u8_t inside(mdl_int_t __x, mdl_int_t __y, ffly_vertex_t *__a, ffly_vertex_t *__b, ffly_vertex_t *__c) {
    ffly_vertex_t v0 = {
        .xa=__b->xa-__a->xa,
        .ya=__b->ya-__a->ya
    };

    ffly_vertex_t v1 = {
        .xa=__c->xa-__a->xa, 
        .ya=__c->ya-__a->ya
    };

    ffly_vertex_t v2 = {
        .xa=__x-__a->xa,
        .ya=__y-__a->ya
    };

    float d00 = ffly_dot(v0.xa, v0.xa, v0.ya, v0.ya);
    float d01 = ffly_dot(v0.xa, v1.xa, v0.ya, v1.ya); 
    float d11 = ffly_dot(v1.xa, v1.xa, v1.ya, v1.ya);
    float d02 = ffly_dot(v0.xa, v2.xa, v0.ya, v2.ya);
    float d12 = ffly_dot(v1.xa, v2.xa, v1.ya, v2.ya);

    float den = 1.0/(d00*d11-d01*d01);
    float a = (d11*d02-d01*d12)*den;
    float b = (d00*d12-d01*d02)*den;
    float g = (1.0-a)-b;
 
    return (a>0.0 && b>0.0 && g>0.0);
}

int main() {
    ffly_io_init();
/*
    struct ffly_polygon poly;
    ffly_bzero(&poly, sizeof(struct ffly_polygon));
    

    ffly_vertex3(&poly, 10, 10, 0);
    ffly_vertex3(&poly, 30, 0, 0);
    ffly_vertex3(&poly, 10, 30, 0);
  //  ffly_vertex3(&poly, 10, 10, 0);
    print_poly(&poly);

    ffly_io_closeup();
    return 0;
*/
    ffly_err_t err;
    struct ffly_obj_man obj_man;
    ffly_obj_man_prepare(&obj_man);

    struct ffly_camera camera;
    ffly_camera_init(&camera, 34, 34);
    ffly_mem_set(camera.pixels, 0, 34*34*4);
    struct ffly_uni uni;
    ffly_uni_build(&uni, _ffly_uni_128, _ffly_uni_128, _ffly_uni_64, 3, _ffly_lotsize_8);

    ffly_set_camerax(camera, 0);
    ffly_set_cameray(camera, 0);

    ffly_camera_bind(&camera, &uni);

    ffly_id_t obj0 = ffly_obj_man_add(&obj_man, ffly_obj_alloc(&err));
    ffly_id_t obj1 = ffly_obj_man_add(&obj_man, ffly_obj_alloc(&err));

    ffly_objp obj = ffly_obj_man_get(&obj_man, obj0);
    ffly_byte_t *texture = (ffly_byte_t*)__ffly_mem_alloc(8*8*4);
    ffly_mem_set(texture, 200, 8*8*4);
    obj->texture = texture;
    obj->xal = 8;
    obj->yal = 8;
    obj->zal = 1;
    obj->xa = 20;
    obj->ya = 20;
    obj->za = 0;

    ffly_uni_attach_obj(&uni, obj);

    obj = ffly_obj_man_get(&obj_man, obj1);
    obj->texture = texture;
    obj->xal = 8;
    obj->yal = 8;
    obj->zal = 1;
    obj->xa = 10;
    obj->ya = 0;
    obj->za = 0;

    ffly_uni_attach_obj(&uni, obj);
    int fd;
    if ((fd = open("/dev/input/event2", O_RDONLY|O_NONBLOCK)) == -1) {
        ffly_printf("failed to open.\n");
        return -1;
    } 

    mdl_u64_t update = 200000, i = 0;

    mdl_i8_t dir = -1;
    mdl_uint_t cam_x = 0, cam_y = 0, x = ffly_obj_man_get(&obj_man, obj0)->xa, y = ffly_obj_man_get(&obj_man, obj0)->ya;
    while(1) {
        struct input_event event;
        if (read(fd, &event, sizeof(struct input_event)) < 0) goto _sk;
        if (event.type == EV_KEY) {
            switch(event.code) {
                case KEY_ESC:
                    goto _exit;
                break;
                case KEY_W: {
                    if (y > 0) y--;
                    break;
                }
                case KEY_S: {
                    y++;
                    break;
                }
                case KEY_A: {
                    if (x > 0) x--;
                    break;
                }
                case KEY_D: {
                    x++;
                    break;
                }
                case KEY_KP8: {
                    if (cam_y > 0) cam_y--;
                    break;
                }
                case KEY_KP2: {
                    cam_y++;
                    break;
                }
                case KEY_KP6: {
                    cam_x++;
                    break;
                }
                case KEY_KP4: {
                    if (cam_x > 0) cam_x--;
                    break;
                }
                
            }
        }

        ffly_set_camerax(camera, cam_x);
        ffly_set_cameray(camera, cam_y);
        _sk:

        if (x == 0)
            dir = 1;
        if (x == 20)
            dir = -1;
        x+=dir;


        if (y == 0)
            dir = 1;
        if (y == 20)
            dir = -1;
        y+=dir;

        ffly_uni_obj_move(&uni, ffly_obj_man_get(&obj_man, obj0), x, y, 0);
        if (i > update) {            
            ffly_camera_handle(&camera);
            system("clear");
            printf("-------------- x: %u:%u, y: %u:%u -------------\n", x, cam_x, y, cam_y);
            ffly_camera_print(&camera);
            i = 0;
        } else
            i++;
    }


    _exit:

    ffly_chunk_cleanup();
    ffly_obj_cleanup();
    ffly_lot_cleanup();
    ffly_uni_free(&uni);
    close(fd);
    ffly_io_closeup();
}
