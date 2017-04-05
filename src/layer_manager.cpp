# include "layer_manager.hpp"

boost::int8_t mdl::firefly::layer_manager::push_forwards(uint_t __layer_id) {
	if (this-> locked_layer(__layer_id)) return FFLY_NOP;

	this-> render_arrm[__layer_id] = this-> render_arrm[__layer_id + 1];
	this-> render_arrm[__layer_id + 1] = __layer_id;
}

boost::int8_t mdl::firefly::layer_manager::pull_backwards(uint_t __layer_id) {
	if (this-> locked_layer(__layer_id)) return FFLY_NOP;

	this-> render_arrm[__layer_id] = this-> render_arrm[__layer_id - 1];
	this-> render_arrm[__layer_id - 1] = __layer_id;
}

boost::int8_t mdl::firefly::layer_manager::swap(uint_t __layer_ida, uint_t __layer_idb) {
	if (this-> locked_layer(__layer_ida)) return FFLY_NOP;
	if (this-> locked_layer(__layer_idb)) return FFLY_NOP;

	this-> render_arrm[__layer_ida] = __layer_idb;
	this-> render_arrm[__layer_idb] = __layer_ida;
}
