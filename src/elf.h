# ifndef __ffly__elf__h
# define __ffly__elf__h
# include "ffint.h"

#define EI_MAG0 0
#define ELF_MAG0 0x7f

#define EI_MAG1 1 
#define ELF_MAG1 'E'
#define EI_MAG2 2
#define ELF_MAG2 'L'
#define EI_MAG3 3
#define ELF_MAG3 'F'

#define EI_CLS 4
#define ELFCLS_NONE 0
#define ELFCLS32 1
#define ELFCLS64 2

#define EI_DAT 5
#define ELFDAT_NONE 0
#define ELFDAT2LSB 1
#define ELFDAT2MSB 2

#define EI_VERS 6
#define EV_NONE 0
#define EV_CURR 1

#define EI_OSABI 7
#define ELFOSABI_NONE 0

#define EI_ABIVERS 8
#define EI_PAD 9
#define EI_NIDENT 16

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

#define EM_X86_64 62
#define EM_AMDGPU 224

#define SHF_EXECINSTR (1 << 2)

#define SHT_PROGBITS 1 
#define SHT_SYMTAB 2 
#define SHT_STRTAB 3
# define elf64_hdr_size sizeof(struct elf64_hdr)
# define elf64_shdr_size sizeof(struct elf64_shdr)
# define elf64_sym_size sizeof(struct elf64_sym)
typedef struct elf64_hdr {
	char unsigned ident[EI_NIDENT];
	ff_u16_t type;
	ff_u16_t machine;
	ff_u32_t version;
	ff_u64_t entry;
	ff_u64_t ph_off;
	ff_u64_t sh_off;
	ff_u32_t flags;
	ff_u16_t hdr_size;
	ff_u16_t phsize;
	ff_u16_t phc;
	ff_u16_t shsize;
	ff_u16_t shc;
	ff_u16_t shstdx;
} *elf64_hdrp;

# define STV_DEFAULT 0
# define STB_GLOBAL 1

typedef struct elf64_sym {
	ff_u32_t name;
	ff_u8_t info;
	ff_u8_t other;
	ff_u16_t sdx;
	ff_u64_t val;
	ff_u64_t size;
} *elf64_symp;

# define SHT_NULL 0
# define SHT_SYMTAB 2
# define SHT_STRTAB 3
# define SHF_EXECINSTR 0x4

#define ELF32_ST_BIND(val) (((unsigned char)(val))>>4)
#define ELF32_ST_TYPE(val) ((val)&0xf)
#define ELF32_ST_INFO(bind, type) (((bind)<<4)+((type)&0xf))

#define ELF64_ST_BIND(val) ELF32_ST_BIND (val)
#define ELF64_ST_TYPE(val) ELF32_ST_TYPE (val)
#define ELF64_ST_INFO(bind, type) ELF32_ST_INFO ((bind), (type))

#define STB_LOCAL 0
#define STB_GLOBAL 1
#define STB_WEAK 2
#define STB_NUM 3
#define STB_LOOS 10
#define STB_GNU_UNIQUE 10
#define STB_HIOS 12
#define STB_LOPROC 13
#define STB_HIPROC 15

#define STT_NOTYPE 0
#define STT_OBJECT 1
#define STT_FUNC 2
#define STT_SECTION 3
#define STT_FILE 4
#define STT_COMMON 5
#define STT_TLS 6
#define STT_NUM 7
#define STT_LOOS 10
#define STT_GNU_IFUNC 10
#define STT_HIOS 12
#define STT_LOPROC 13
#define STT_HIPROC 15 

typedef struct elf64_shdr {
	ff_u32_t name;
	ff_u32_t type;
	ff_u64_t flags;
	ff_u64_t addr;
	ff_u64_t offset;
	ff_u64_t size;
	ff_u32_t link;
	ff_u32_t info;
	ff_u64_t addralign;
	ff_u64_t entsize;
} *elf64_shdrp;

typedef struct elf64_syminfo {

} *elf64_syminfop;

# endif /*__ffly__elf__h*/
