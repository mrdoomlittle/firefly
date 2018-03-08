# include "brew.h"
# include "../stdio.h"
# include "../ffly_def.h"
# define RTBUF_SIZE 20

objp rtbuf[RTBUF_SIZE];
objp static *retto = rtbuf;
void static
op_cp(objp *__p) {

}

void static
op_jump(objp *__p) {
	if (retto-rtbuf >= RTBUF_SIZE) {
		printf("rtbuf overflow.\n");
		return;
	}
	*(retto++) = (*__p)->next;
	*__p = (*__p)->to;
}

void static
op_echo(objp *__p) {
	objp p = *__p;
	printf("%s", (char*)p->p);
}

void static
op_end(objp *__p) {
	objp p = *__p;
	*__p = *(--retto);
}

static void(*op[])(objp*) = {
	op_cp,
	NULL,
	op_jump,
	op_echo,
	op_end
};

void brew_exec(objp __top) {
	printf("exec.\n");
	objp cur = __top, bk;
	while(cur != NULL) {
		if (cur->opcode == _op_exit) {
			printf("goodbye.\n");
			break;
		}
		bk = cur;
		op[cur->opcode](&cur);
		if (bk != cur)
			continue;
		cur = cur->next;
	}
}
