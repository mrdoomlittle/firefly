# include "worker_manager.hpp"
bool mdl::firefly::worker_manager::to_shutdown = false;
mdl::firefly::types::err_t mdl::firefly::worker_manager::add_worker(types::id_t& __worker_id) {
	types::id_t worker_id;
	if ((worker_id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t))) == NULL) {
		fprintf(stderr, "worker_manager: failed to alloc memory for worker id, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	*worker_id = this-> worker_indx.size();
	__worker_id = worker_id;

	this-> worker_indx.resize(this-> worker_indx.size() + 1);

	if ((std::get<0>(this-> worker_indx[*worker_id]) = memory::alloc_pixmap(
		this-> chunk_xaxis_len,
		this-> chunk_yaxis_len,
		this-> chunk_zaxis_len
	)) == NULL) {
		fprintf(stderr, "worker_manager: failed to alloc memory for pixmap, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	memory::mem_init(std::get<0>(this-> worker_indx[*worker_id]), (this-> chunk_xaxis_len * this-> chunk_yaxis_len * this-> chunk_zaxis_len) * 4);
	if ((std::get<1>(this-> worker_indx[*worker_id]) = (types::worker_config_t *)memory::mem_alloc(sizeof(types::worker_config_t)) ) == NULL) {
		fprintf(stderr, "worker_manager: failed to alloc memory for worker config, errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	types::worker_config_t& worker_config = *std::get<1>(this-> worker_indx[*worker_id]);

	worker_config.chunk_xlen = this-> chunk_xaxis_len;
	worker_config.chunk_ylen = this-> chunk_yaxis_len;
	worker_config.chunk_zlen = this-> chunk_zaxis_len;

	worker_config.chunk_xaxis = this-> nxt_xaxis;
	worker_config.chunk_yaxis = this-> nxt_yaxis;
	worker_config.chunk_zaxis = this-> nxt_zaxis;

	std::get<2>(this-> worker_indx[*worker_id]) = worker_id;
	if (this-> nxt_xchunk_id == this-> xchunk_c) {
		this-> nxt_xchunk_id = 0;
		this-> nxt_xaxis = 0;

		if (this-> nxt_ychunk_id == this-> ychunk_c) {
			this-> nxt_ychunk_id = 0;
			this-> ychunk_c = 0;

			if (this-> nxt_zchunk_id == this-> zchunk_c) {
				// somthing
			} else {
				this-> nxt_zchunk_id ++;
				this-> nxt_zaxis += this-> chunk_zaxis_len;
			}

		} else {
			this-> nxt_ychunk_id ++;
			this-> nxt_yaxis += this-> chunk_yaxis_len;
		}
	} else {
		this-> nxt_xchunk_id ++;
		this-> nxt_xaxis += this-> chunk_xaxis_len;
	}

	this-> connected_workers ++;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::worker_manager::rm_worker(types::id_t __worker_id) {
	static boost::mutex mx;

	boost::mutex::scoped_lock scoped_lock(mx);
	if (__worker_id == nullptr || this-> connected_workers == 0) return FFLY_NOP;
	types::__id_t worker_id = *__worker_id;

	if (worker_id == this-> worker_indx.size() - 1) {
		memory::mem_free(std::get<0>(this-> worker_indx[worker_id]));
		memory::mem_free(std::get<2>(this-> worker_indx[worker_id]));
		memory::mem_free(std::get<1>(this-> worker_indx[worker_id]));
		this-> worker_indx.resize(this-> worker_indx.size() - 1);
	} else {
		this-> th_control.th_id = this-> worker_indx.size() - 1;
		this-> th_control.th_pause = true;
		while (!this-> th_control.th_waiting) {}

		memory::mem_free(std::get<0>(this-> worker_indx[worker_id]));
		memory::mem_free(std::get<1>(this-> worker_indx[worker_id]));

		*std::get<2>(*(this-> worker_indx.end() - 1)) = *std::get<2>(this-> worker_indx[worker_id]);
		memory::mem_free(std::get<2>(this-> worker_indx[worker_id]));

		this-> worker_indx[worker_id].swap(*(this-> worker_indx.end() - 1));

		this-> th_control.th_msg = worker_manager::INX_SHIFT;
		while(this-> th_control.th_reply == TH_NULL) {}

		if(this-> th_control.th_reply == TH_FAILURE) {
			return FFLY_FAILURE;
		}

		this-> th_control.th_msg = worker_manager::MSG_NULL;

		this-> worker_indx.resize(this-> worker_indx.size() - 1);
		this-> th_control.th_pause = false;
	}

	this-> connected_workers --;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::worker_manager::listen() {
	if (this-> tcp_server.listen() == FFLY_FAILURE)
		return FFLY_FAILURE;

	int sock, usock;
	this-> connected_workers = 0;

	do {
		printf("waiting for workers to connect.\n");
		if (this-> tcp_server.accept(sock) == FFLY_FAILURE) {
			fprintf(stderr, "worker_manager: failed to accept worker, errno: %d\n", errno);
			continue;
		}
		printf("extrblished tcp connection.\n");

		struct sockaddr_in client_addr;

		this-> udp_server.recv_ack(client_addr);
		this-> udp_server.send_ack(client_addr);

		types::id_t worker_id;
		if (this-> add_worker(worker_id) == FFLY_FAILURE) {
			fprintf(stderr, "worker_manager: failed to add worker\n");
			this-> tcp_server.close(sock);
			continue;
		}

		boost::thread(&worker_handler, this, sock, client_addr, worker_id);
		printf("worker_manager: worker has connected. ip: %s\n\n", this-> tcp_server.cl_ip_addr(sock));
	} while(!worker_manager::to_shutdown);
	this-> de_init();

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::worker_manager::de_init() {
	printf("worker_manager: deinitalizing.\n");
	while(!this-> worker_indx.empty()) {}
	this-> tcp_server.de_init();
	return FFLY_SUCCESS;
}
