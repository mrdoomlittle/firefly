# include "io.h"
# include "vec.h"
# include "map.h"
int main() {
    ffly_io_init();
/*
    ffly_err_t err;
    ffly_vecp vec = ffly_vec(sizeof(mdl_u64_t), VEC_AUTO_RESIZE|VEC_NONCONTINUOUS|VEC_BLK_CHAIN, &err);

    mdl_u64_t *list[0xff];
    mdl_u8_t i = 0;
    while(i != 0xff) {
        mdl_u64_t *p;
        ffly_vec_push_back(vec, (void**)&p);
        *p = i;
        list[i] = p;
        ffly_printf("push: %u\n", i);
        i++;
    }

    i = 254;
    while(i != 200) {
        ffly_vec_del(vec, list[i]);
        i--;
    }
    mdl_u64_t *p = (mdl_u64_t*)ffly_vec_rend(vec);
    ffly_printf("%lu\n", *p);
*/
/*
    mdl_u64_t *itr = ffly_vec_begin(vec);
    while(itr != NULL) {
        ffly_printf("%lu\n", *itr);
        ffly_vec_itr(vec, (void**)&itr, VEC_ITR_FD, 1);
    }
*/
//    ffly_vec_destroy(vec);
    ffly_io_closeup();
}
