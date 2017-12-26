# include "asset_manager.h"
# include "system/flags.h"
# include "system/file.h"
# include <mdl/str_cmb.h>
# include "graphics/image.h"
# include "types/byte_t.h"
char const static* asset_kind_to_str(mdl::uint_t __kind) {
	switch(__kind) {
		case _ffly_ak_img: return "image";
		case _ffly_ak_aud: return "audio";
		case _ffly_ak_raw: return "raw";
	}
	return "unknown";
}

mdl::firefly::types::id_t mdl::firefly::asset_manager::load_asset(char *__dir, char *__name, uint_t __kind, uint_t __format, types::mode_t __mode, types::err_t& __err) {
	types::id_t id;
	if (__kind == _ffly_ak_img)
	{
		graphics::imagep img = static_cast<graphics::imagep>(memory::mem_alloc(sizeof(graphics::image)));
		graphics::ld_img(img, __dir, __name, __format);
		id = this->add_asset(reinterpret_cast<types::byte_t*>(img),  __kind, __err);
	}
	else if (__kind >= _ffly_ak_aud)
	{
/*
		types::byte_t *f;
		types::size_t fsize;
		ffly_aud_fformat_t fformat;
		switch(__kind) {
			case _ffly_ak_wav_file:
				fformat = _ffly_audf_wav;
			break;
		}

		if (ffly_ld_aud_file(__dir, __name, fformat, &f, &fsize) != FFLY_SUCCESS) {
			return nullptr;
		}

		ffly_aud_fad_t *aud_fad = static_cast<ffly_aud_fad_t*>(memory::mem_alloc(sizeof(ffly_aud_fad_t)));
		*aud_fad = (ffly_aud_fad_t) {
			fp:f,
			fsize:fsize,
			fformat:fformat
		};

		id = this->add_asset(reinterpret_cast<types::byte_t*>(aud_fad), __kind, __err);
		if (_err(__err)) {
			system::io::fprintf(ffly_err, "failed to add asset.\n");
			return ffly_null_id;
		}
*/
	}
	else if (__kind == _ffly_ak_raw) {
		types::byte_t *f;
		types::size_t size;

		char *path = mdl_str_cmb(__dir, __name, 0);
		types::err_t err;
		FF_FILE *file = ffly_fopen(path, FF_O_RDONLY, 0, &err);
		if (_err(err)) {
			system::io::fprintf(ffly_err, "failed to open asset file.\n");
			return ffly_null_id;
		}

		struct ffly_stat st;
		if (_err(__err = ffly_fstat(path, &st))) {
			system::io::fprintf(ffly_err, "failed to stat file.\n");
			return ffly_null_id;
		}

		size = st.size;
		if ((f = static_cast<types::byte_t*>(memory::mem_alloc(size))) == nullptr) {
			system::io::fprintf(ffly_err, "failed to allocate memory for asset.\n");
			__err = FFLY_FAILURE;
			return ffly_null_id;
		}

		if (_err(__err = ffly_fread(file, f, size))) {
			system::io::fprintf(ffly_err, "failed to read file.\n");
			return ffly_null_id;
		}

		if (_err(__err = ffly_fclose(file))) {
			system::io::fprintf(ffly_err, "failed to close file.\n");
			return ffly_null_id;
		}

		id = this->add_asset(f, __kind, __err);
		if (_err(__err)) {
			system::io::fprintf(ffly_err, "failed to add asset.\n");
			return ffly_null_id;
		}

		if (_err(__err = memory::mem_free(path))) {
			return ffly_null_id;
		}
	}
# ifdef __DEBUG_ENABLED
	system::io::fprintf(ffly_log, "asset_manager: loaded asset{id: %u, file: '%s%s', kind: %s}\n", *id, __dir, __name, asset_kind_to_str(__kind));
# endif
	__err = FFLY_SUCCESS;
	return id;
}

mdl::firefly::types::id_t mdl::firefly::asset_manager::add_asset(types::byte_t *__data, uint_t __kind, types::err_t& __err) {
	types::id_t id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t));
	if (id == nullptr) {
		system::io::fprintf(ffly_err, "failed to allocate memory for asset id.\n");
		__err = FFLY_FAILURE;
		return ffly_null_id;
	}

	if (_err(__err = this->asset_d.resize(this->asset_d.size()+1))) {
		system::io::fprintf(ffly_err, "failed to resize.\n");
		return ffly_null_id;
	}

	*id = this->asset_d.size()-1;
	this->asset_d[*id] = (asset)	{
		id:id,
		kind:__kind,
		data:__data
	};

	this->asset_ids.insert(id);
# ifdef __DEBUG_ENABLED
	system::io::fprintf(ffly_log, "asset_manager: added asset{id: %u}\n", *id);
# endif
	__err = FFLY_SUCCESS;
	return id;
}

void mdl::firefly::asset_manager::free_asset(types::id_t __id) {
	asset& _asset = this->asset_d[*__id];
	if (_asset.kind == _ffly_ak_img){
		graphics::imagep img = reinterpret_cast<graphics::imagep>(_asset.data);
		if (img->pixels != nullptr)
			memory::mem_free(img->pixels);
	}

	if (_asset.id != nullptr)
		memory::mem_free(_asset.id);
	if (_asset.data != nullptr)
		memory::mem_free(_asset.data);
}


void mdl::firefly::asset_manager::free_all() {
	types::size_t i = 0;
	for (;i != this->asset_d.size();i++) {
		system::io::fprintf(ffly_log, "asset_manager: freed memory for asset{id: %u}, size: %u bytes.\n", i, this->asset_d[i].bc);
		this->free_asset(this->asset_d[i].id);
	}
}

mdl::firefly::types::err_t mdl::firefly::asset_manager::de_init() {
	this->free_all();
	delete &this->asset_ids;
	delete &this->asset_d;
}

/*
void mdl::firefly::asset_manager::del_asset(types::id_t __id) {
	std::set<types::id_t>::iterator itr = this->asset_ids.find(__id);
	if (itr == this->asset_ids.end()) {
		printf("failed to look up asset, id? %d\n", *__id);
		return;
	}

	printf("deleting asset, id: %d\n", *__id);

	this->asset(__id) = *(this->asset_d.end()-1);
	*this->asset(__id).id = *__id;

	this->free_asset(__id);

	this->asset_d.resize(this->asset_d.size()-1);
	this->asset_ids.erase(itr);
}
*/
# include "system/err.h"
mdl::firefly::types::bool_t mdl::firefly::asset_manager::valid_asset_id(types::id_t __id) {
    types::err_t err;
    this->asset_ids.find(__id, err);
    if (_err(err))
        return 0;
    return 1;
}

mdl::firefly::types::byte_t* mdl::firefly::asset_manager::get_asset_data(types::id_t __id) {
	if (!this->valid_asset_id(__id)) return nullptr;
	return this->asset_d[*__id].data;
}

mdl::firefly::asset& mdl::firefly::asset_manager::get_asset(types::id_t __id) {
	if (!this->valid_asset_id(__id)) {
        // err
    }
	return this->asset_d[*__id];
}

extern "C" {
ffly_id_t ffly_load_asset(void *__clsp, char *__dir, char *__name, mdl_uint_t __kind, mdl_uint_t __format, ffly_err_t *__err) {
	static_cast<mdl::firefly::asset_manager*>(__clsp)->load_asset(__dir, __name, __kind, __format, 0x0, *__err);
}
ffly_id_t ffly_add_asset(void *__clsp, ffly_byte_t *__data, mdl_uint_t __kind, ffly_err_t *__err) {
	static_cast<mdl::firefly::asset_manager*>(__clsp)->add_asset(__data, __kind, *__err);
}
ffly_assetp ffly_get_asset(void *__clsp, ffly_id_t __id) {
	return &static_cast<mdl::firefly::asset_manager*>(__clsp)->get_asset(__id);
}
}
/*
int main() {
	mdl::firefly::asset_manager ast_manager;

	boost::uint8_t *asset_data = (boost::uint8_t *)malloc(8);
	mdl::uint_t *_iid;
	mdl::uint_t *asset_id;
	for (std::size_t o = 0; o != 12; o ++) {
		asset_id = ast_manager.add_asset(asset_data, AST_PIXMAP_T);
		if (o == 4) _iid = asset_id;
	}

	ast_manager.del_asset(_iid);
	printf("id: %d, %d\n", *_iid, *asset_id);
}*/
