# include "layer_manager.hpp"

boost::int8_t mdl::firefly::layer::push_forwards(uint_t __layer_id) {
	this-> render_arrm[__layer_id] = this-> render_arrm[__layer_id + 1];
	this-> render_arrm[__layer_id + 1] = __layer_id;
}

boost::int8_t mdl::firefly::layer::pull_backwards(uint_t __layer_id) {
	this-> render_arrm[__layer_id] = this-> render_arrm[__layer_id - 1];
	this-> render_arrm[__layer_id - 1] = __layer_id;
}

boost::int8_t mdl::firefly::layer::swap(uint_t __layer_ida, uint_t __layer_idb) {
	this-> render_arrm[__layer_ida] = __layer_idb;
	this-> render_arrm[__layer_idb] = __layer_ida;
}
