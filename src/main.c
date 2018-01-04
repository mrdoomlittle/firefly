
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
int main() {
    ffly_io_init();
    ffly_err_t err;
    struct ffly_camera camera;
    ffly_camera_init(&camera, 34, 34);
    ffly_mem_set(camera.pixels, 0, 34*34*4);
    struct ffly_uni uni;
    ffly_uni_build(&uni, _ffly_uni_128, _ffly_uni_128, _ffly_uni_64, 3, _ffly_lotsize_8);

    ffly_set_camerax(camera, 0);
    ffly_set_cameray(camera, 0);

    ffly_camera_bind(&camera, &uni);

    ffly_objp obj0 = ffly_obj_alloc(&err);
    ffly_objp obj1 = ffly_obj_alloc(&err);
    obj0->texture = obj1->texture = (ffly_byte_t*)__ffly_mem_alloc(8*8*4);
    ffly_mem_set(obj0->texture, 200, 8*8*4);
    obj0->xal = 8;
    obj0->yal = 8;
    obj0->zal = 1;
    obj0->xa = 20;
    obj0->ya = 20;
    obj0->za = 0;

    obj1->xal = 8;
    obj1->yal = 8;
    obj1->zal = 1;
    obj1->xa = 10;
    obj1->ya = 0;
    obj1->za = 0;

    ffly_uni_attach_obj(&uni, obj0);
    ffly_uni_attach_obj(&uni, obj1);
    int fd;
    if ((fd = open("/dev/input/event2", O_RDONLY|O_NONBLOCK)) == -1) {
        ffly_printf("failed to open.\n");
        return -1;
    } 

    mdl_u64_t update = 20000, i = 0;

    mdl_i8_t dir = -1;
    mdl_uint_t cam_x = 0, cam_y = 0, x = obj0->xa, y = obj0->ya;
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
                    if (cam_x > 0) cam_x--;
                    break;
                }
                case KEY_KP6: {
                    cam_x++;
                    break;
                }
                case KEY_KP4: {
                    if (cam_y > 0) cam_y--;
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

        ffly_uni_obj_move(&uni, obj0, x, y, 0);
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
