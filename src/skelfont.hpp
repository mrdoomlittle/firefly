# ifndef __skelfont__hpp
# define __skelfont__hpp
# include "types/skelfont_t.hpp"
# include "graphics/skelmap_loader.hpp"
# include <dirent.h>
# include <sys/types.h>
# include <iostream>
# include "asset_manager.hpp"
# include "combstr.hpp"
# include <map>
namespace mdl {
namespace firefly {
types::skelfont_t* load_skelfont(char const *__dir, asset_manager *asset_man) {
	static types::skelfont_t skelfont;

	DIR *sf_dir = opendir(__dir);
	if (!sf_dir) return nullptr;

	struct dirent *dir_;
	uint_t curr_skel_file = 0;

	while ((dir_ = readdir(sf_dir)) != NULL) {
		if (strlen(dir_-> d_name) < 12) continue;

		if (!strncmp("letter_", dir_->d_name, 7)){
			if (strcmp(".skel", dir_->d_name + (strlen(dir_->d_name) - 5))) continue;

//			skelfont.font_data.resize(curr_skel_file + 1);

			char * curr_dir = combstr(__dir, "/");
			char * path = combstr(curr_dir, dir_->d_name);
			std::free(curr_dir);

			uint_t asset_id = asset_man-> load_asset(path, SKELMAP_T);
			std::free(path);

			skelfont.font_data[dir_->d_name[7]] = asset_id;

			skelfont.font_size ++;
			curr_skel_file ++;
		}
	}

	closedir(sf_dir);

	return &skelfont;
}
}
}


# endif /*__skelfont__hpp*/
