# include "rosin.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "m.h"

/*
	diffrent from resin 
	more solid less flex
*/

void ff_rosin_init(ffly_rosinp __ros) {
	__ros->stack = (ff_u8_t*)__ffly_mem_alloc(__ros->stack_size);
}

void ff_rosin_de_init(ffly_rosinp __ros) {
	__ffly_mem_free(__ros->stack);
}

void static
stack_put(ffly_rosinp __ros, ff_u8_t *__src, ff_uint_t __bc, ff_addr_t __addr) {
	ff_u8_t *p, *end;

	p = __src;
	end = p+__bc;
	ff_u8_t *dst;
	while(p != end) {
		dst = __ros->stack+(__addr+(p-__src));
		*dst = *(p++);
	}
}

void static
stack_get(ffly_rosinp __ros, ff_u8_t *__dst, ff_uint_t __bc, ff_addr_t __addr) {
	ff_u8_t *p, *end;

	p = __dst;
	end = __dst+__bc;
	ff_u8_t *src;
	while(p != end) {
		src = __ros->stack+(__addr+(p-__dst));
		*(p++) = *src;
	}
}

# define stack_at(__ros, __adr) \
	((void*)((__ros->stack)+(__adr)))

void static get(ffly_rosinp __ros, void *__buf, ff_uint_t __n) {
	ff_u8_t *p, *end;

	p = (ff_u8_t*)__buf;
	end = p+__n;
	while(p != end)
		*(p++) = __ros->fetch_byte(__ros->ip_off++);
}

void _ros_asb();
void _ros_asw();
void _ros_asd();
void _ros_asq();
void _ros_outb();
void _ros_outw();
void _ros_outd();
void _ros_outq();
void _ros_mu();
void _ros_dmu();
void _ros_exit();
void _ros_mi();
void _ros_dmi();
void _ros_rin();
static void(*op[])() = {
	_ros_asb,
	_ros_asw,
	_ros_asd,
	_ros_asw,
	_ros_outb,
	_ros_outw,
	_ros_outd,
	_ros_outq,
	_ros_mu,
	_ros_dmu,
	_ros_exit,
	_ros_mi,
	_ros_dmi,
	_ros_rin
};


# include "raster.h"
static void(*rin[])(struct rr_struc*) = {
	ffly_rraster
};

# define fi __asm__("jmp _ros_fi"); // finished
# define next __asm__("jmp _ros_next")
# define end __asm__("jmp _ros_end")
# define jmpto(__p) __asm__("jmp *%0" : : "r"(__p))
void ff_rosin_run(ffly_rosinp __ros) {
	ff_u8_t opno;
	ff_u8_t l;
	__asm__("_ros_next:\n");
	__ros->ip_off = 0;
	get(__ros, &opno, 1);
	jmpto(op[opno]);

	__asm__("_ros_asb:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _ros_as	\n"
			"_ros_asw:		\n\t"
			"movb $2, %0	\n\t"
			"jmp _ros_as	\n"
			"_ros_asd:		\n\t"
			"movb $4, %0	\n\t"
			"jmp _ros_as	\n"
			"_ros_asq:		\n\t"
			"movb $8, %0	\n"
			"_ros_as:		\n\t" : "=m"(l));
	{
		ff_addr_t to;
		get(__ros, &to, sizeof(ff_addr_t));

		ff_u64_t val;
		get(__ros, &val, l);
		stack_put(__ros, &val, l, to);
	}
	fi;

	__asm__("_ros_rin:\n\t"); {
		ff_u8_t r, n;
		get(__ros, &r, 1);
		get(__ros, &n, 1);
		ff_addr_t args, struc;
		get(__ros, &struc, sizeof(ff_addr_t));
		get(__ros, &args, sizeof(ff_addr_t));
		struct rr_struc *s;
		s = (struct rr_struc*)stack_at(__ros, struc);
		s->n = n;
		s->args = (void**)stack_at(__ros, args);
		rin[r](s);
	}
	fi;

	__asm__("_ros_dmu:\n\t"); {
		ffly_dmu();
	}
	fi;

	__asm__("_ros_mu:\n\t"); {
		ff_addr_t dst;
		get(__ros, &dst, sizeof(ff_addr_t));
		ff_u64_t val;
		val = ffly_mem_usage();
		stack_put(__ros, &val, 8, dst);
	}
	fi;


	__asm__("_ros_mi:\n\t"); {
		ff_addr_t dst;
		get(__ros, &dst, sizeof(ff_addr_t));
		struct ffly_meminfo mi;

		ffly_meminfo(&mi);
		stack_put(__ros, &mi, sizeof(struct ffly_meminfo), dst);
	}
	fi;

	__asm__("_ros_dmi:\n\t"); {
		ff_addr_t adr;
		get(__ros, &adr, sizeof(ff_addr_t));
		ffly_dmi((struct ffly_meminfo*)stack_at(__ros, adr));
	}
	fi;

	__asm__("_ros_outb:		\n\t"
			"movb $1, %0	\n\t"
			"jmp _ros_out	\n"
			"_ros_outw:		\n\t"
			"movb $2, %0	\n\t"
			"jmp _ros_out	\n"
			"_ros_outd:		\n\t"
			"movb $4, %0	\n\t"
			"jmp _ros_out	\n"
			"_ros_outq:		\n\t"
			"movb $8, %0	\n"
			"_ros_out:		\n\t" : "=m"(l));
	{
		ff_addr_t src;
		get(__ros, &src, sizeof(ff_addr_t));
		ff_u64_t val;
		val ^= val;

		stack_get(__ros, &val, l, src);
		ffly_printf("out: %u : %u\n", val, l);
	}
	fi;

	__asm__("_ros_exit:\n\t"
			"jmp _ros_end");

	__asm__("_ros_fi:\n");
	__ros->ip_incr(__ros->ip_off);
	next;
	__asm__("_ros_end:\n");
}
