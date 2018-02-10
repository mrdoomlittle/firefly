# ifndef __ffly__elf__h
# define __ffly__elf__h
# include <mdlint.h>

# define EI_MAG0 0
# define EI_MAG1 1 
# define EI_MAG2 2
# define EI_MAG3 3
# define EI_CLS 4
# define ELFCLS_NONE 0
# define ELFCLS32 1
# define ELFCLS64 2

# define EI_DAT 5
# define ELFDAT_NONE 0
# define ELFDAT2LSB 1
# define ELFDAT2MSB 2

# define EI_VERS 6
# define EV_NONE 0
# define EV_CURR 1

# define EI_OSABI 7
# define ELFOSABI_NONE 0

# define EI_ABIVERS 8
# define EI_PAD 9
# define EI_NIDENT 16

# define ET_NONE 0
# define ET_REL 1
# define ET_EXEC 2
# define ET_DYN 3
# define ET_CORE 4

# define EM_X86_64 62
typedef struct elf64_hdr {
	char unsigned ident[EI_NIDENT];
	mdl_u16_t type;
	mdl_u16_t machine;
	mdl_u32_t version;
	mdl_u64_t entry;
	mdl_u64_t ph_off;
	mdl_u64_t sh_off;
	mdl_u32_t flags;
	mdl_u16_t hdr_size;
	mdl_u16_t phtesize;
	mdl_u16_t phtec;
	mdl_u16_t shtesize;
	mdl_u16_t shtec;
	mdl_u16_t shstdx;
} *elf64_hdrp;

# define STV_DEFAULT 0
# define STB_GLOBAL 1

typedef struct elf64_sym {
	mdl_u32_t name;
	mdl_u8_t info;
	mdl_u8_t other;
	mdl_u16_t sdx;
	mdl_u64_t val;
	mdl_u64_t size;
} *elf64_symp;

# define SHT_NULL 0
# define SHT_SYMTAB 2
# define SHT_STRTAB 3
# define SHF_EXECINSTR 0x4

typedef struct elf64_shdr {
	mdl_u32_t name;
	mdl_u32_t type;
	mdl_u64_t flags;
	mdl_u64_t addr;
	mdl_u64_t offset;
	mdl_u64_t size;
	mdl_u32_t link;
	mdl_u32_t info;
	mdl_u64_t addralign;
	mdl_u64_t entsize;
} *elf64_shdrp;

typedef struct elf64_syminfo {

} *elf64_syminfop;

# endif /*__ffly__elf__h*/
