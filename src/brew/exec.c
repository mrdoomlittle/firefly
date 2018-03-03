# include "brew.h"
# include "../stdio.h"
# include "../ffly_def.h"
objp top = NULL, end = NULL;
objp rtbuf[20];
objp *retto = rtbuf;
void op_cp(objp *__p) {

}

void op_jump(objp *__p) {
	*(retto++) = (*__p)->next;
	*__p = (*__p)->to;
}

void op_echo(objp *__p) {
	objp p = *__p;
	printf("%s", (char*)p->p);
}

void op_end(objp *__p) {
	objp p = *__p;
	*__p = *(--retto);
}

void(*op[])(objp*) = {
	op_cp,
	NULL,
	op_jump,
	op_echo,
	op_end
};

void exec() {
	printf("exec.\n");
	objp cur = top, bk;
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
