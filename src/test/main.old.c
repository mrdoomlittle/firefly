# include "xf86drm.h"
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
int main() {
/*
    drmDevicePtr list[20]; 
    int c = drmGetDevices(list, 20);
    int i = 0;
    while(i != c) {
        drmDevicePtr dev = list[i];
        printf("bus: %u\n", dev->businfo.pci->bus);
        printf("vendor-id: %u\n", dev->deviceinfo.pci->vendor_id);
        printf("device-id: %u\n", dev->deviceinfo.pci->device_id); 

        i++;
    } 
*/
    int fd = open("/dev/dri/card0", O_RDONLY);
//  int fd = drmOpen(NULL, "pci:0000:1f:00.0");
    if (fd == -1) {
        fprintf(stdout, "failed to open, err: %s\n", strerror(errno));
        return -1;
    }

    drmDevicePtr dev;
    drmGetDevice(fd, &dev);

    printf("bus: %u\n", dev->businfo.pci->bus);
    printf("vendor-id: %u\n", dev->deviceinfo.pci->vendor_id);
    printf("device-id: %u\n", dev->deviceinfo.pci->device_id);



    drmFreeDevice(&dev);
//  drmClose(fd);
    close(fd);
  //  drmFreeDevices(list, c);
    return 0;
}
