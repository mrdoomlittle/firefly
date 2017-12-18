# include "event.hpp"
# include "queue.h"
# include "errno.h"
# include "err.h"
# include "io.h"
static mdl::firefly::system::queue<mdl::firefly::types::event_t*> event_queue;
mdl::firefly::types::err_t mdl::firefly::system::event_push(types::event_t *__event) {
	if (event_queue.size() > 20) return FFLY_SUCCESS;
	types::err_t err;
	if (_err(err = event_queue.push(__event))) {
		return err;
	}
	system::io::fprintf(ffly_log, "event pushed into queue.\n");
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::system::next_event(types::event_t*& __event) {
	types::err_t err;
	__event = event_queue.pop(err);
	if (_err(err)) {
		return err;
	}
	system::io::fprintf(ffly_log, "event poped from queue.\n");
	return FFLY_SUCCESS;
}

mdl::firefly::types::bool_t mdl::firefly::system::pending_event() {
	return event_queue.size()>0;
}
