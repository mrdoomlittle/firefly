# include "event.hpp"
# include <cstdio>
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

