
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
int main() {
    ffly_io_init();
    ffly_err_t err;
    struct ffly_camera camera;
    ffly_camera_init(&camera, 34, 34);
    ffly_mem_set(camera.pixels, 0, 34*34*4);
    struct ffly_uni uni;
    ffly_uni_build(&uni, 128, 128, 4, 2);
    ffly_set_camerax(camera, 0);
    ffly_set_cameray(camera, 0);

    ffly_camera_bind(&camera, &uni);

    int fd;
    if ((fd = open("/dev/input/event2", O_RDONLY)) == -1) {
        ffly_printf("failed to open.\n");
        return -1;
    } 

    mdl_uint_t x, y;
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
            }
        }

        ffly_set_camerax(camera, x);
        ffly_set_cameray(camera, y);
        _sk:
        ffly_camera_handle(&camera);
        system("clear");
        printf("-------------- x: %u, y: %u -------------\n", x, y);
        ffly_camera_print(&camera);
        ffly_nanosleep(0, 100000000);
    }


    _exit:
    ffly_uni_free(&uni);
    close(fd);
    ffly_io_closeup();
}
