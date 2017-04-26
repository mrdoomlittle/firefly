# include "ffly_audio.hpp"
mdl::firefly::types::err_t mdl::ffly_audio::load_aud_file(char const *__fdir, char const *__fname, firefly::types::afformat_t __afformat, u8_t*& __fbuff, off_t& __fbuff_size) {
	char *fext;
	switch(__afformat) {
		case firefly::system::WAV:
			fext = const_cast<char *>(".wav");
		break;
		default:
			fprintf(stderr, "ffly_audio: failed to lookup file format.\n");
			return FFLY_NOP;
	}
	char *fpth = strcmb(const_cast<char *>(__fdir), strcmb(const_cast<char *>(__fname), fext, STRCMB_FREE_NONE), STRCMB_FREE_SECOND);
	int fd;

	if ((fd = open(fpth, O_RDONLY)) < 0) {
		fprintf(stderr, "ffly_audio: failed to open file, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	if (dup2(fd, STDIN_FILENO) < 0) {
		fprintf(stderr, "ffly_audio: failed to duplicate file descriptor, errno: %d\n", errno);
		close(fd);
		return FFLY_FAILURE;
	}

	struct stat st;
	if (stat(fpth, &st) == 0)
		__fbuff_size = st.st_size;
	else {
		fprintf(stderr, "ffly_audio: failed to get file size.\n");
		return FFLY_FAILURE;
	}

	printf("loading sound file with size: %d\n", __fbuff_size);
	__fbuff = (u8_t *)firefly::memory::mem_alloc(__fbuff_size * sizeof(u8_t));
	bzero(__fbuff, __fbuff_size * sizeof(u8_t));

	read(STDIN_FILENO, __fbuff, __fbuff_size * sizeof(u8_t));
	close(fd);
	return FFLY_SUCCESS;
}

mdl::firefly::types::wav_disc_t mdl::ffly_audio::get_wav_fdisc(u8_t *__raw_data, firefly::types::err_t& __any_err) {
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

mdl::firefly::types::wav_disc_t mdl::ffly_audio::get_wav_fdisc(char const *__fdir, char const *__fname, firefly::types::err_t& __any_err) {
	char *fpth = strcmb(const_cast<char *>(__fdir), strcmb(const_cast<char *>(__fname), const_cast<char *>(".wav"), STRCMB_FREE_NONE), STRCMB_FREE_SECOND);
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
