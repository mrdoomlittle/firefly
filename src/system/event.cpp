# include "event.hpp"
std::queue<mdl::firefly::data::pair<mdl::firefly::system::xevent::event_id_t, void*>> mdl::firefly::system::xevent::event_queue;
uint_t static max_queue_size;

mdl::firefly::types::err_t mdl::firefly::system::xevent::event_add(event_id_t __event_id, void *__event_data) {
	if (event_queue.size() > max_queue_size) return FFLY_FAILURE;
	data::pair<event_id_t, void*> event = {__event_id, __event_data};
	event_queue.push(event);
	return FFLY_SUCCESS;
}

void* mdl::firefly::system::xevent::nxt_event(event_id_t& __event_id) {
	__event_id = event_queue.front().first;
	return event_queue.front().second;
}

void mdl::firefly::system::xevent::event_pop() {event_queue.pop();}
mdl::uint_t mdl::firefly::system::xevent::max_queue_size() {
	return max_queue_size;
}

void mdl::firefly::system::xevent::max_queue_size(uint_t __max_queue_size) {
	max_queue_size = __max_queue_size;
}

