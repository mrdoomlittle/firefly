# include "job.h"
# include "../linux/mman.h"
# include "../linux/sched.h"
# include "../linux/unistd.h"
# include "../linux/stat.h"
# include "../linux/types.h"
# include "../linux/signal.h"
# include "../linux/wait.h"
# include "../malloc.h"
# include "../stdio.h"
struct job {
	struct job *next;
	__linux_pid_t pid;
};

static struct job *top = NULL;
void mould_job(char const *__file, char *const *__argv, char *const *__envp) {
	struct job *j = (struct job*)malloc(sizeof(struct job));

	__linux_pid_t pid = fork();
	if (pid == 0) {
		int fd = open("stdout", O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
		dup2(fd, 1);
		dup2(fd, 2);
		close(fd);
		if (execve(__file, __argv, __envp) == -1) {
			// error
		}
		exit(0);
	}

	j->pid = pid;
	j->next = top;
	top = j;
}

void jobs_wait(void) {
	printf("waiting for jobs to finish.\n");
	struct job *cur = top, *bk;
	while(cur != NULL) {
		wait4(cur->pid, NULL, __WALL, NULL);	
		bk = cur;
		cur = cur->next;
		free(bk);
	}
	top = NULL;
}
