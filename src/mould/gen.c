# include "mould.h"
# include "../stdio.h"
# include "../malloc.h"
# include "../system/vec.h"
# include "../system/map.h"
# include "../dep/str_len.h"
objp static top;
objp static end;

objp static back = NULL;
struct ffly_map static label;
struct ffly_vec static jmpto;

typedef struct jmpd {
	char const *label;
	objp *to;
} *jmpdp;

objp static
nextobj() {
	objp ret = (objp)malloc(sizeof(struct obj));
	to_free(ret);
	if (!top)
		top = ret;

	if (end != NULL)
		end->next = ret;
	end = ret;
	return ret;
}

void static
op_exit() {
	objp o = nextobj();
	o->op = _op_exit;
}

void static
op_jobs_wait() {
	objp o = nextobj();
	o->op = _op_jobs_wait;
}

void static
emit_cp(bucketp __p) {
	objp o = nextobj();
	o->op = _op_cp;
	o->src = __p->src;
	o->dst = __p->dst;
}

void static
emit_exit(bucketp __p) {
	op_exit();
}

void static
emit_label(bucketp __p) {
	ffly_map_put(&label, __p->p, ffly_str_len((char const*)__p->p), &end->next);
}

void static
emit_end(bucketp __p) {
	objp o = nextobj();
	o->op = _op_end;
}

void static
emit_jump(bucketp __p) {
	objp o = nextobj();
	o->op = _op_jump;
	jmpdp p;
	ffly_vec_push_back(&jmpto, (void**)&p);
	p->label = (char const*)__p->p;
	p->to = &o->to;
}

void static
emit_echo(bucketp __p) {
	objp o = nextobj();
	o->op = _op_echo;
	o->p = __p->p;
}

void static 
emit_shell(bucketp __p) {
	objp o = nextobj();
	o->op = _op_shell;
	o->p = __p->p;
}

void static
emit_waitall(bucketp __p) {
	op_jobs_wait();
}

static void(*emit[])(bucketp) = {
	emit_label,
	emit_cp,
	emit_exit,
	emit_end,
	emit_jump,
	emit_shell,
	emit_echo,
	emit_waitall
};

void oust(bucketp __p, ff_u8_t __no) {
	emit[__no](__p);
}

char const static* nn(bucketp __node) {
	switch(__node->sort) {
		case _label: return "label";
		case _cp: return "cp";
		case _exit: return "exit";
		case _end: return "end";
		case _jump: return "jump";
		case _shell: return "shell";
		case _echo: return "echo";
	}
	return "unknown";
}

# include "../system/err.h"
void gen(bucketp __p, objp *__top) {
	ffly_map_init(&label, _ffly_map_127);
	ffly_vec_set_flags(&jmpto, VEC_AUTO_RESIZE);
	ffly_vec_init(&jmpto, sizeof(struct jmpd));
	bucketp cur = __p;
	while(cur != NULL) {
		oust(cur, cur->sort);
		printf("oust: %s", nn(cur));
		if (cur->sort == _jump)
			printf(", to: %s", cur->p);
		else if (cur->sort == _label)
			printf(", name: %s", cur->p);
		printf("\n");
		cur = cur->fd;
	}

	if (end != NULL)
		end->next = NULL;

	ff_err_t err;
	jmpdp jmp, end;
	___ffly_vec_nonempty(&jmpto) {
		jmp = (jmpdp)ffly_vec_begin(&jmpto);
		end = (jmpdp)ffly_vec_end(&jmpto);
		while(!vec_at_deadstop(jmp, end)) {
			objp *p = (objp*)ffly_map_get(&label, jmp->label, ffly_str_len((char const*)jmp->label), &err);
			if (_err(err) || !p) {
				printf("label does not exist.\n");
				break;
			}
			*jmp->to = *p;
			jmp++;
		}
	}

	ffly_vec_de_init(&jmpto);
	ffly_map_de_init(&label);
	*__top = top;
}
