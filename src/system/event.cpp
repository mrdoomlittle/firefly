# include "event.hpp"
# include "queue.h"
# include "errno.h"
static mdl::firefly::system::queue<mdl::firefly::types::event_t> event_queue;
mdl::firefly::types::err_t mdl::firefly::system::event_add(types::event_t __event) {
	if (event_queue.size() > 20) return FFLY_SUCCESS;
	if (event_queue.push(__event) != FFLY_SUCCESS) {
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::system::next_event(types::event_t& __event) {
	types::err_t any_err;
	__event = event_queue.pop(any_err);
	return FFLY_SUCCESS;
}

mdl::firefly::types::bool_t mdl::firefly::system::pending_event() {
	return event_queue.size()>0;
}

/*
std::queue<mdl::firefly::data::pair<mdl::firefly::types::event_desc_t, void*>> mdl::firefly::system::event::event_queue;
mdl::uint_t static _max_queue_size = 21;

mdl::firefly::types::err_t mdl::firefly::system::event::event_add(types::event_desc_t __event_desc, void *__event_data) {
	if (event_queue.size() > _max_queue_size) {
		ffly_errno = FF_ERR_CEBF;
		return FFLY_FAILURE;
	}

	data::pair<types::event_desc_t, void*> event = {__event_desc, __event_data};
	event_queue.push(event);
	return FFLY_SUCCESS;
}

void* mdl::firefly::system::event::nxt_event(types::event_desc_t& __event_desc) {
	if (event_queue.empty()) return nullptr;
	__event_desc = event_queue.front().first;
	return event_queue.front().second;
}

void mdl::firefly::system::event::event_pop() {event_queue.pop();}
mdl::uint_t mdl::firefly::system::event::max_queue_size() {
	return _max_queue_size;
}

void mdl::firefly::system::event::max_queue_size(uint_t __max_queue_size) {
	_max_queue_size = __max_queue_size;
}
*/

