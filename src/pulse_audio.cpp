# include "pulse_audio.hpp"
static const pa_sample_spec def_sample_spec = {
	.format = PA_SAMPLE_S16LE,
	.rate = 44100,
	.channels = 2
};

boost::int8_t mdl::firefly::pulse_audio::load_file(char const *__filedir, char const *__filename) {
	char *file_pth = strcmb(const_cast<char *>(__filedir), const_cast<char *>(__filename), STRCMB_FREE_NONE);

	int fd, any_error;
	if ((fd = open(file_pth, O_RDONLY)) < 0) {
		fprintf(stderr, "failed to open file, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	if (dup2(fd, STDIN_FILENO) < 0) {
		fprintf(stderr, "failed to dup file descriptor.\n");
		close(fd);
		return FFLY_FAILURE;
	}

	struct stat st;

	if (stat(file_pth, &st) == 0) {
		this-> file_size = st.st_size;
	} else {
		fprintf(stderr, "failed to get file size.\n");
		return FFLY_FAILURE;
	}
	printf("loading sound file with size: %d\n", this-> file_size);

	if (this-> buff != nullptr) memory::mem_free(this-> buff);

	this-> buff = (boost::uint8_t *)memory::mem_alloc(file_size);
	bzero(this-> buff, file_size);

	read(STDIN_FILENO, this-> buff, this-> file_size * sizeof(boost::uint8_t));

	close(fd);

	return FFLY_SUCCESS;
}

boost::int8_t mdl::firefly::pulse_audio::play_sample(char const *__name, char const *__stream_name, struct pa_sample_spec *__sample_spec, boost::uint8_t *__buff, std::size_t __file_size) {
	if (this-> buff == nullptr) {
		fprintf(stderr, "audio file buff empty.\n");
		return FFLY_FAILURE;
	}

	pa_simple *simple = NULL;
	int any_error;

	std::size_t r;

	if (!(simple = pa_simple_new(NULL, __name, PA_STREAM_PLAYBACK, NULL, __stream_name, __sample_spec == nullptr? &def_sample_spec : __sample_spec, NULL, NULL, &any_error))) {
		fprintf(stderr, "failed to create new pulse audio sample.\n");
		goto failure;
	}

	if (!simple) {
		fprintf(stderr, "look like it failed to create sample.\n");
		goto failure;
	}

	r = (__buff == nullptr? this-> file_size : __file_size) * sizeof(boost::uint8_t);
	if (pa_simple_write(simple, this-> buff, r * sizeof(boost::uint8_t), &any_error) < 0) {
		fprintf(stderr, "failed to write pa sample, errno: %s\n", pa_strerror(any_error));
		goto failure;
	}

	if (pa_simple_drain(simple, &any_error) < 0) {
		fprintf(stderr, "failed to drain pa sample, errno: %s\n", pa_strerror(any_error));
		goto failure;
	}

	if (simple) pa_simple_free(simple);
	return FFLY_SUCCESS;
	failure:

	if (simple) pa_simple_free(simple);
	return  FFLY_FAILURE;
}
