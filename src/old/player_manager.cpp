# include "player_manager.hpp"
mdl::firefly::types::err_t mdl::firefly::player_manager::add_player(types::__id_t& __player_id) {
	printf("adding player.\n");
	types::__id_t player_id;

	if (!this-> unused_ids.empty()) {
		std::set<types::__id_t>::iterator itor;
		itor = this-> unused_ids.begin();

		player_id = *itor;

		this-> unused_ids.erase(itor);
	} else {
		player_id = this-> player_indx.size();
		this-> player_indx.resize(this-> player_indx.size() + 1);
	}

	this-> player_c++;
	return FFLY_SUCCESS;
}


mdl::firefly::types::err_t mdl::firefly::player_manager::rm_player(types::__id_t __player_id) {
	this-> unused_ids.insert(__player_id);
	this-> player_c--;
	return FFLY_SUCCESS;
}
/*
int main() {
	mdl::firefly::player_manager player_manager;

	mdl::firefly::types::__id_t player_id;
	player_manager.add_player(player_id);

}*/
