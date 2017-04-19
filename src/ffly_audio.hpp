# ifndef __ffly__audio__hpp
# define __ffly__audio__hpp
# include "types/aftype_t.h"
# include "types/audio/wav_disc_t.h"
# include <errno.h>
# include <fcntl.h>
# include <cstdio>
# include "types/err_t.h"
# include <strcmb.hpp>
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
namespace mdl {
namespace ffly_audio {
firefly::types::wav_disc_t static get_wav_fdisc(u8_t *__raw_data, firefly::types::err_t& __any_err) {
	firefly::types::wav_disc_t wav_disc{0};
	u16_t byte_point = 0;
	auto nxt_byte = [&byte_point]() -> u16_t {
		u16_t this_byte = byte_point;
		byte_point ++;
		return this_byte;
	};

	wav_disc.cnk_id[0] = __raw_data[nxt_byte()];
	wav_disc.cnk_id[1] = __raw_data[nxt_byte()];
	wav_disc.cnk_id[2] = __raw_data[nxt_byte()];
	wav_disc.cnk_id[3] = __raw_data[nxt_byte()];

	wav_disc.cnk_size |= (wav_disc.cnk_size & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.cnk_size |= (wav_disc.cnk_size & 0xFF) | __raw_data[nxt_byte()] << 8;
	wav_disc.cnk_size |= (wav_disc.cnk_size & 0xFF) | __raw_data[nxt_byte()] << 16;
	wav_disc.cnk_size |= (wav_disc.cnk_size & 0xFF) | __raw_data[nxt_byte()] << 24;

	wav_disc.format[0] = __raw_data[nxt_byte()];
	wav_disc.format[1] = __raw_data[nxt_byte()];
	wav_disc.format[2] = __raw_data[nxt_byte()];
	wav_disc.format[3] = __raw_data[nxt_byte()];

	wav_disc.sub_cnk1_id[0] = __raw_data[nxt_byte()];
	wav_disc.sub_cnk1_id[1] = __raw_data[nxt_byte()];
	wav_disc.sub_cnk1_id[2] = __raw_data[nxt_byte()];
	wav_disc.sub_cnk1_id[3] = __raw_data[nxt_byte()];

	wav_disc.sub_cnk1_size |= (wav_disc.sub_cnk1_size & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.sub_cnk1_size |= (wav_disc.sub_cnk1_size & 0xFF) | __raw_data[nxt_byte()] << 8;
	wav_disc.sub_cnk1_size |= (wav_disc.sub_cnk1_size & 0xFF) | __raw_data[nxt_byte()] << 16;
	wav_disc.sub_cnk1_size |= (wav_disc.sub_cnk1_size & 0xFF) | __raw_data[nxt_byte()] << 24;

	wav_disc.aud_format |= (wav_disc.aud_format & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.aud_format |= (wav_disc.aud_format & 0xFF) | __raw_data[nxt_byte()] << 8;

	wav_disc.chn_c |= (wav_disc.chn_c & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.chn_c |= (wav_disc.chn_c & 0xFF) | __raw_data[nxt_byte()] << 8;

	wav_disc.sample_rate |= (wav_disc.sample_rate & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.sample_rate |= (wav_disc.sample_rate & 0xFF) | __raw_data[nxt_byte()] << 8;
	wav_disc.sample_rate |= (wav_disc.sample_rate & 0xFF) | __raw_data[nxt_byte()] << 16;
	wav_disc.sample_rate |= (wav_disc.sample_rate & 0xFF) | __raw_data[nxt_byte()] << 24;

	wav_disc.byte_rate |= (wav_disc.byte_rate & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.byte_rate |= (wav_disc.byte_rate & 0xFF) | __raw_data[nxt_byte()] << 8;
	wav_disc.byte_rate |= (wav_disc.byte_rate & 0xFF) | __raw_data[nxt_byte()] << 16;
	wav_disc.byte_rate |= (wav_disc.byte_rate & 0xFF) | __raw_data[nxt_byte()] << 24;

	wav_disc.blk_align |= (wav_disc.blk_align & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.blk_align |= (wav_disc.blk_align & 0xFF) | __raw_data[nxt_byte()] << 8;

	wav_disc.bit_depth |= (wav_disc.bit_depth & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.bit_depth |= (wav_disc.bit_depth & 0xFF) | __raw_data[nxt_byte()] << 8;

	wav_disc.sub_cnk2_id[0] = __raw_data[nxt_byte()];
	wav_disc.sub_cnk2_id[1] = __raw_data[nxt_byte()];
	wav_disc.sub_cnk2_id[2] = __raw_data[nxt_byte()];
	wav_disc.sub_cnk2_id[3] = __raw_data[nxt_byte()];

	wav_disc.sub_cnk2_size |= (wav_disc.sub_cnk2_size & 0xFF) | __raw_data[nxt_byte()];
	wav_disc.sub_cnk2_size |= (wav_disc.sub_cnk2_size & 0xFF) | __raw_data[nxt_byte()] << 8;
	wav_disc.sub_cnk2_size |= (wav_disc.sub_cnk2_size & 0xFF) | __raw_data[nxt_byte()] << 16;
	wav_disc.sub_cnk2_size |= (wav_disc.sub_cnk2_size & 0xFF) | __raw_data[nxt_byte()] << 24;

	__any_err = FFLY_SUCCESS;
	return wav_disc;
}
firefly::types::wav_disc_t static get_wav_fdisc(char const *__fdir, char const *__fname, firefly::types::err_t& __any_err) {
	char *fpth = strcmb(const_cast<char *>(__fdir), const_cast<char *>(__fname), STRCMB_FREE_NONE);
	firefly::types::wav_disc_t wav_disc;

	int fd;
	if ((fd = open(fpth, O_RDONLY)) < 0) {
		fprintf(stderr, "ffly_audio: failed to open file, errno: %d\n", errno);
		__any_err = FFLY_FAILURE;
		return wav_disc;
	}

	if (dup2(fd, STDIN_FILENO) < 0) {
		fprintf(stderr, "ffly_audio: failed to duplicate file descriptor, errno: %d\n", errno);
		close(fd);
	}

	u8_t *raw_data = (u8_t *)firefly::memory::mem_alloc(sizeof(firefly::types::wav_disc_t));
	read(STDIN_FILENO, raw_data, sizeof(firefly::types::wav_disc_t));
	close(fd);
	wav_disc = get_wav_fdisc(raw_data, __any_err);
	firefly::memory::mem_free(raw_data);
	return wav_disc;
}
}
}

# endif /*__ffly__audio__hpp*/
