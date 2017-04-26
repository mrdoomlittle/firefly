# include "png_loader.hpp"
boost::int8_t mdl::firefly::graphics::load_png_file(char const *__filedir, char const *__filename, types::pixmap_t&__pixmap, uint_t *__pm_size) {
	char *abs_fpath = strcmb(const_cast<char *>(__filedir), strcmb(const_cast<char *>(__filename), ".png", STRCMB_FREE_NONE), STRCMB_FREE_SECOND);
	FILE *file = fopen(abs_fpath, "rb");

	if (file == NULL) {
		fprintf(stderr, "error cant open png file. at %s\n", abs_fpath);
		return FFLY_FAILURE;
	}

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return FFLY_FAILURE;

	png_infop info = png_create_info_struct(png);
	if (!info) return FFLY_FAILURE;

	if(setjmp(png_jmpbuf(png))) return FFLY_FAILURE;

	png_init_io(png, file);
	png_read_info(png, info);

	__pm_size[0] = png_get_image_width(png, info);
	__pm_size[1] = png_get_image_height(png, info);
	printf("loading img. size %dx%d\n", __pm_size[0], __pm_size[1]);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_RGB_ALPHA) return FFLY_FAILURE;

	types::pixmap_t pixmap = static_cast<types::pixmap_t>(memory::mem_alloc((__pm_size[0] * __pm_size[1]) * 4));
	bzero(pixmap, (__pm_size[0] * __pm_size[1]) * 4);

	for (std::size_t y = 0; y != __pm_size[1]; y ++)
		png_read_row(png, pixmap + (y * (__pm_size[0] * 4)), NULL);

	fclose(file);
	std::free(abs_fpath);

	__pixmap = pixmap;

	return FFLY_SUCCESS;
}

boost::int8_t mdl::firefly::graphics::load_png_file(char const *__filedir, char const *__filename, types::pixmap_t&__pixmap, types::_2d_dsize_t<>& __pm_size) {
	uint_t pm_size[2] = {0};
	boost::int8_t result = load_png_file(__filedir, __filename, __pixmap, pm_size);
	__pm_size.xaxis_len = pm_size[0];
	__pm_size.yaxis_len = pm_size[1];
	return result;
}
