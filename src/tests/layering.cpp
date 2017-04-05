# include "layering.hpp"

void mdl::firefly::tests::layering(uint_t __max_xlen, uint_t __min_xlen, uint_t __max_ylen, uint_t __min_ylen, layer_manager *__layer_ptr, uint_t __amount, uint_t __spacing, boost::uint8_t __alpha) {
	uint_t xaxis = 0;
	uint_t yaxis = 0;

	for (std::size_t o = 0; o != __amount; o ++) {
		uint_t rand_xlen = rand() % __max_xlen + __min_xlen;
		uint_t rand_ylen = rand() % __max_ylen + __min_ylen;

		printf("rxlen: %d, rylen: %d\n", rand_xlen, rand_ylen);
		printf("xaxis: %d, yaxis: %d\n", xaxis, yaxis);

		mdl::firefly::graphics::colour_t colour;
		colour.r = rand() % 255 + 0;
		colour.g = rand() % 255 + 0;
		colour.b = rand() % 255 + 0;
		colour.a = __alpha;

		printf("r: %d, g: %d, b: %d, a: %d\n", colour.r, colour.g, colour.b, colour.a);

		uint_t layer_id = __layer_ptr-> add_layer(rand_xlen, rand_ylen, xaxis, yaxis);

		mdl::firefly::graphics::fill_pixmap(__layer_ptr-> get_layer_pixmap(layer_id), rand_xlen, rand_ylen, colour);

		xaxis += __spacing + 1;
		yaxis += __spacing + 1;
	}
}
