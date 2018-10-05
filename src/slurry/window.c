# include "window.h"
# include <stdio.h>
# include <malloc.h>
struct s_window* _s_window_new(void) {
	struct s_window *wd;
	wd = (struct s_window*)malloc(sizeof(struct s_window));
	wd->next = NULL;
	return wd;
}

void _s_window_destroy(struct s_window *__wd) {
	free(__wd);
}
