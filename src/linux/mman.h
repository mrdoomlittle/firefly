# ifndef __ffly__linux__mmap__h
# define __ffly__linux__mmap__h

# define PROT_READ 0x1
# define PROT_WRITE 0x2
# define PROT_EXEC 0x4	

# define MAP_SHARED 0x01
# define MAP_PRIVATE 0x02	
# define MAP_ANONYMOUS 0x20

void* mmap(void*, mdl_u64_t, mdl_u64_t, mdl_u64_t, mdl_u64_t, mdl_u64_t);
void munmap(void*, mdl_u64_t);
# endif /*__ffly__linux__mmap__h*/
