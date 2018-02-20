# include "brew.h"
# include "../stdio.h"
# include "../ffly_def.h"
objp top = NULL, end = NULL;
void op_cp(objp __p) {

}

void(*op[])(objp) = {
	op_cp,
	NULL
};

void exec() {
	objp cur = top;
	while(cur != NULL) {
		if (cur->opcode == _op_exit) {
			printf("goodbye.\n");
			break;
		}
		op[cur->opcode](cur);
		cur = cur->next;
	}
}
