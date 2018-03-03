# include "brew.h"
# include "../stdio.h"
# include "../malloc.h"
# include "../system/vec.h"
# include "../system/map.h"
# include "../dep/str_len.h"
objp extern top;
objp extern end;


objp back = NULL;
struct ffly_map label;
struct ffly_vec jmpto;

typedef struct jmpd {
	char const *label;
	objp *to;
} *jmpdp;

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

void emit_end(bucketp __p) {

}

void emit_jump(bucketp __p) {
	objp o = nextobj();
	o->opcode = _op_jump;
	jmpdp p;
	ffly_vec_push_back(&jmpto, (void**)&p);
	p->label = (char const*)__p->p;
	p->to = &o->to;
}

void(*emit[])(bucketp) = {
	emit_label,
	emit_cp,
	emit_exit,
	emit_end,
	emit_jump
};

void oust(bucketp __p, mdl_u8_t __no) {
	emit[__no](__p);
}

void gen(bucketp __p) {
	ffly_map_init(&label, _ffly_map_127);
	ffly_vec_set_flags(&jmpto, VEC_AUTO_RESIZE);
	ffly_vec_init(&jmpto, sizeof(struct jmpd));
	bucketp cur = __p;
	while(cur != NULL) {
		oust(cur, cur->sort);
		cur = cur->fd;
	}

	if (end != NULL)
		end->next = NULL;

	ffly_err_t err;
	jmpdp jmp;
	___ffly_vec_nonempty(&jmpto) {
		jmp = (jmpdp)ffly_vec_begin(&jmpto);
		while(jmp != NULL) {
			printf("-- jmp.\n");
			*jmp->to = *(objp*)ffly_map_get(&label, jmp->label, ffly_str_len((char const*)jmp->label), &err);
			ffly_vec_fd(&jmpto, (void**)&jmp);
		}
	}


	ffly_vec_de_init(&jmpto);
	ffly_map_de_init(&label);
}
