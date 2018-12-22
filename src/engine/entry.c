# include "../ffint.h"
# include "../types.h"
# include "view.h"
ff_i8_t(*ffe_init)(void);
ff_i8_t(*ffe_de_init)(void);
void ffe_loop(void);

/*
	TOBE renamed
*/
void ffe_entry(void);

ff_i8_t _ffe_init(void);
ff_i8_t _ffe_de_init(void);
# include "../context.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	__ffctx__ = ff_ctx_new();
	ffe_entry();

	_ffe_init();
	ffe_init();

	ffe_loop();


	ffe_de_init();
	_ffe_de_init();
}
