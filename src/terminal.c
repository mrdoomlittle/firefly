# include "types.h"
# include "system/err.h"
# include "system/pipe.h"
# include "system/io.h"
# include "dep/str_cmp.h"
# include "dep/str_dup.h"
# include "memory/mem_free.h"
# include "system/file.h"
# include "hatch.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
char static*
read_part(char **__p, char *__buf) {
	char *p = __buf;
	while(**__p != ' ' && **__p != '\n' && **__p != '\0') *(p++) = *((*__p)++);
	*p = '\0';
	(*__p)++;
	return __buf;
}

char static*
read_part_and_dupe(char **__p, char *__buf) {
	char *part = read_part(__p, __buf);
	return ffly_str_dupe(part);
}

char const *help = "commands:\n"
				   "   connect [hatch-key{/tmp/hatch.XXX}],\n"
				   "   meminfo,\n"
				   "   lsvec,\n"
				   "   exit\n";

ff_err_t static
sndop(ff_u8_t __op, ff_uint_t __pipe) {
	ff_err_t ret;
	ffly_printf("sending op.\n");
	ret= ffly_pipe_wr8l(__op, __pipe);
	ffly_printf("sent.\n");
	return ret;
}

ff_err_t static
lsvec(ff_uint_t __pipe) {
	if (_err(sndop(_ffly_ho_lsvec, __pipe)))
		reterr;

	ff_err_t err;
	ff_uint_t l;
	if ((ff_i8_t)ffly_pipe_rd8l(__pipe, &err) == -1) {
		ffly_printf("nothing to be recved.\n");
		retok; 
	}

	if (_err(err))
		reterr;

	ffly_pipe_read(&l, sizeof(ff_uint_t), __pipe); 
	if (_err(err))
		reterr;
	ffly_printf("buffer size: %u\n", l);
	char *s = (char*)__ffly_mem_alloc(l);
	_next:
	ffly_pipe_read(s, l, __pipe);
	if (_err(err))
		reterr;
	ffly_printf("%s", s);
	if (!ffly_pipe_rd8l(__pipe, &err)) {
		goto _next;
	}
	if (_err(err))
		reterr;

	__ffly_mem_free(s);
}

void static lsmap(ff_uint_t __pipe) {
}

ff_err_t ffmain(int __argc, char **__argv) {
	char buf[200];
	char line[200];
	char *p;
	ff_err_t err;
	ff_uint_t pipe;
	ff_i8_t conn = -1;
	while(1) {
		p = line;
		ffly_printf("~: ");
		ffly_rdline(line, 200, ffly_in);
		char *part = read_part(&p, buf);
		if (!conn) {
			if (!ffly_str_cmp(part, "meminfo")) {
				if (_err(sndop(_ffly_ho_meminfo, pipe)))
					break;
				struct ffly_meminfo info;
				ffly_pipe_read(&info, sizeof(struct ffly_meminfo), pipe);
				if (_err(err))
					break;
				ffly_printf("used: %u\n", info.used);
			} else if (!ffly_str_cmp(part, "lsvec"))
				lsvec(pipe);
			else if (!ffly_str_cmp(part, "lsmap"))
				lsmap(pipe);
			else if (!ffly_str_cmp(part, "shutdown")) {
				if (_err(sndop(_ffly_ho_shutdown, pipe)))
					break;
				conn = -1;
				ffly_pipe_close(pipe);
			} else
				goto _sk;
			continue;
		}

	_sk:
		if (!ffly_str_cmp(part, "connect")) {
			char *key = read_part_and_dupe(&p, buf);
			FF_FILE *file = ffly_fopen(key, FF_O_RDONLY, 0, &err);

			ff_uint_t shm_id;
			ffly_fread(file, &shm_id, sizeof(ff_uint_t));	 
			ffly_printf("connecting to pipe with shmid: %u\n", shm_id);
			ff_err_t err;
			pipe = ffly_pipe(8, FF_PIPE_SHMM, shm_id, &err);
			ffly_pipe_connect(pipe);
			conn = 0;  
			ffly_fclose(file); 
			__ffly_mem_free(key);
		} else if (!ffly_str_cmp(part, "help"))
			ffly_printf("%s", help);
		else if (!ffly_str_cmp(part, "exit")) {
			if (!conn)
				if (_err(sndop(_ffly_ho_disconnect, pipe)))
					break;
			ffly_printf("goodbye.\n");
			break;
		} else
			ffly_printf("unknown command, are you connected?\n");
	}

	if (!conn) {
		ffly_pipe_close(pipe);
	}
	retok;
}
