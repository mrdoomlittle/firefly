# include "scale_pixelmap.h"
# include "mem_cpy.h"
# include "../memory/alloc_pixelmap.h"
# include "../types/byte_t.h"
ffly_err_t ffly_scale_nearest_neighbor(ffly_pixelmap_t __dst, ffly_pixelmap_t __src, mdl_uint_t __dst_width, mdl_uint_t __dst_height, mdl_uint_t __src_width, mdl_uint_t __src_height, mdl_u8_t __chn_c) {
    mdl_uint_t xa, ya = 0;
    while(ya != __dst_height) {
        xa = 0;
        while(xa != __dst_width) {
            mdl_uint_t src_off = (((mdl_uint_t)(xa*((float)__src_width/(float)__dst_width)))+(((mdl_uint_t)(ya*((float)__src_height/(float)__dst_height)))*__src_width))*__chn_c; 
            mdl_uint_t dst_off = (xa+(ya*__dst_width))*__chn_c;
            ffly_mem_cpy(__dst+dst_off, __src+src_off, __chn_c);
            xa++;
        }
        ya++;   
    }
}
# include "../system/nanosleep.h"
# include "../maths/floor.h"
# include "../system/io.h"
ffly_err_t ffly_scale_bilinear(ffly_pixelmap_t __dst, ffly_pixelmap_t __src, mdl_uint_t __dst_width, mdl_uint_t __dst_height, mdl_uint_t __src_width, mdl_uint_t __src_height, mdl_u8_t __chn_c) {
    mdl_uint_t xa, ya = 0;
    float a0 = ((float)__src_width/(float)__dst_width);
    float a1 = ((float)__src_height/(float)__dst_height);

    float a2 = ((float)__dst_width/(float)__src_width);
    float a3  = ((float)__dst_height/(float)__src_height);

    ffly_fprintf(ffly_out, "a0: %f, a1: %f\n", a0, a1);
    while(ya != __dst_height) {
        xa = 0;
        while(xa != __dst_width) {
            mdl_uint_t src_xa = (float)xa*a0;
            mdl_uint_t src_ya = (float)ya*a1;

            mdl_uint_t tl_off = (src_xa+(src_ya*__src_width))*__chn_c;
            mdl_uint_t tr_off = ((src_xa+1)+(src_ya*__src_width))*__chn_c;
            mdl_uint_t bl_off = ((src_xa)+((src_ya+1)*__src_width))*__chn_c;
            mdl_uint_t br_off = ((src_xa+1)+((src_ya+1)*__src_width))*__chn_c;

            ffly_byte_t tl, tr, bl, br;
            mdl_u8_t c = 0;
            for (;c != __chn_c-1;c++) {
                tl = __src[tl_off+c], tr = __src[tr_off+c], bl = __src[bl_off+c], br = __src[br_off+c];
                float x0 = (float)xa-(((float)xa/a2)*a2);
                float y0 = (float)ya-(((float)ya/a3)*a3);

                float x1 = a0-x0;
                float y1 = a1-y0;

                float t = (x0*((float)tl/a2))+(x1*((float)tr/a2));
                float b = (x0*((float)bl/a2))+(x1*((float)br/a2));
                float l = (y0*((float)tl/a3))+(y1*((float)bl/a3));
                float r = (y0*((float)tr/a3))+(y1*((float)br/a3));
                __dst[((xa+(ya*__dst_width))*__chn_c)+c] = (ffly_byte_t)((((y0*(t/a3))+(y1*(b/a3)))+((x0*(l/a2))+(x1*(r/a2))))*8.0);
            }

            __dst[((xa+(ya*__dst_width))*__chn_c)+3] = 255;
            xa++;
        }
        ya++;
    }
}
