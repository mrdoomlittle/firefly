# include "brew.h"
# include "../stdio.h"
# include "../malloc.h"
# include "../system/map.h"
# include "../data/str_len.h"
objp extern top;
objp extern end;


objp back = NULL;
struct ffly_map label;

objp nextobj() {
	objp ret = (objp)malloc(sizeof(struct obj));
	to_free(ret);
	if (!top)
		top = ret;

	if (end != NULL)
		end->next = ret;
	end = ret;
	return ret;
}

void op_exit() {
	objp o = nextobj();
	o->opcode = _op_exit;
}

void emit_cp(bucketp __p) {
	objp o = nextobj();
	o->opcode = _op_cp;
	o->src = __p->src;
	o->dst = __p->dst;
}

void emit_exit(bucketp __p) {
	op_exit();
}

void emit_label(bucketp __p) {
	ffly_map_put(&label, __p->p, ffly_str_len((char const*)__p->p), &end->next);
}

void(*emit[])(bucketp) = {
	emit_label,
	emit_cp,
	emit_exit
};

void oust(bucketp __p, mdl_u8_t __no) {
	emit[__no](__p);
}

void gen(bucketp __p) {
	ffly_map_init(&label, _ffly_map_127);
	bucketp cur = __p;
	while(cur != NULL) {
		oust(cur, cur->sort);
		cur = cur->fd;
	}

	if (end != NULL)
		end->next = NULL;
	ffly_map_de_init(&label);
}
