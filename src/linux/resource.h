# ifndef __ffly__linux__resource__h
# define __ffly__linux__resource__h
# include "../ffint.h"
# include "types.h"
# include "time.h"

struct rusage {
	struct timeval ru_utime;
	struct timeval ru_stime;
	ff_s64_t ru_maxrss;
	ff_s64_t ru_ixrss;
	ff_s64_t ru_idrss;
	ff_s64_t ru_isrss;
	ff_s64_t ru_minflt;
	ff_s64_t ru_majflt;
	ff_s64_t ru_nswap;
	ff_s64_t ru_inblock;
	ff_s64_t ru_oublock;
	ff_s64_t ru_msgsnd;
	ff_s64_t ru_msgrcv;
	ff_s64_t ru_nsignals;
	ff_s64_t ru_nvcsw;
	ff_s64_t ru_nivcsw;
};

# endif /*__ffly__linux__resource__h*/
