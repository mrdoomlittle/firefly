# include "allocr.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../system/printf.h"

void pr();
void pf();

void test0() {
    void *p0, *p1, *p2, *p3;
    p0 = ffly_alloc(10);
    p1 = ffly_alloc(20);
    p2 = ffly_alloc(30);
    p3 = ffly_alloc(40);
    ffly_free(p0);
    ffly_free(p1);
    ffly_free(p2);
    ffly_free(p3);



    p0 = ffly_alloc(200);
    p1 = ffly_alloc(200);
}

void test1() {
    void *list[20];
    mdl_u8_t i = 0;
    void *p;
    while(i != 20) {
        p = ffly_alloc((i+1)*10);
        list[i] = ffly_alloc(8);
        ffly_free(p);
        i++;
    }

    i = 0;
    while(i != 20) {
        ffly_free(list[i]);
        i++;
    }
}

# include "../system/vec.h"
void test2() {
    struct ffly_vec vec;
    ffly_vec_set_flags(&vec, VEC_AUTO_RESIZE);
    ffly_vec_init(&vec, 2);
    mdl_u16_t i = 0;
    mdl_u16_t *p;
    while(i != 800) {
        ffly_vec_push_back(&vec, (void**)&p);
        *p = i;
        i++;
    }

    mdl_u16_t v;
    i = 0;
    while(i != 800) {
        ffly_vec_pop_back(&vec, &v);
        ffly_printf("%u\n", i);
        i++;
    }
    
    ffly_vec_de_init(&vec);
}

# include "../data/str_cpy.h"
int main() {
    //ffly_ar_init();
    ffly_io_init();  
    test2(); 

    pr();
    pf();
    ffly_io_closeup();
    ffly_ar_cleanup();
}
