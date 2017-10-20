# include "asset_manager.h"
# include "system/flags.h"
char const static* asset_kind_to_str(mdl::uint_t __kind) {
	switch(__kind) {
		case _ffly_ak_png_file: return "png file";
		case _ffly_ak_wav_file: return "wav file";
	}
	return "unknown";
}

mdl::firefly::types::id_t mdl::firefly::asset_manager::load_asset(char *__fdir, char *__fname, uint_t __kind, types::mode_t __mode) {
	types::id_t asset_id;

	if (__kind >= _ffly_ak_png_file && __kind <=_ffly_ak_png_file)
	{
/*
		types::_2d_dsize_t<> *dsize = (types::_2d_dsize_t<> *)memory::mem_alloc(sizeof(types::_2d_dsize_t<>));
		types::pixmap_t pixmap;

		if (graphics::load_png_file("", __fname, pixmap, *dsize) != FFLY_SUCCESS) {
			return nullptr;
		}

		asset_id = this->add_asset(pixmap, 0, __kind);

		this->asset_info(asset_id) = dsize;
*/
	}
	else if (__kind >= _ffly_ak_wav_file && __kind <=_ffly_ak_wav_file)
	{
		ffly_byte_t *f;
		ffly_size_t fsize;
		ffly_aud_fformat_t fformat;
		switch(__kind) {
			case _ffly_ak_wav_file:
				fformat = _ffly_audf_wav;
			break;
		}

		if (ffly_ld_aud_file(__fdir, __fname, fformat, &f, &fsize) != FFLY_SUCCESS) {
			return nullptr;
		}

		ffly_aud_fad_t *aud_fad = static_cast<ffly_aud_fad_t*>(memory::mem_alloc(sizeof(ffly_aud_fad_t)));
		*aud_fad = (ffly_aud_fad_t) {
			fp:f,
			fsize:fsize,
			fformat:fformat
		};

		asset_id = this->add_asset(reinterpret_cast<types::byte_t*>(aud_fad), __kind);
	}
# ifdef __DEBUG_ENABLED
	system::io::printf(stdout, "asset_manager: loaded asset{id: %u, file: '%s%s'}\n", *asset_id, __fdir, __fname);
# endif
	return asset_id;
}

mdl::firefly::types::id_t mdl::firefly::asset_manager::add_asset(types::byte_t *__data, uint_t __kind) {
	types::id_t asset_id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t));
	this->asset_d.resize(this->asset_d.size()+1);

	*asset_id = this->asset_d.size()-1;
	this->asset_d[*asset_id] = (types::asset_t)	{
		id:asset_id,
		kind:__kind,
		data:__data
	};

	this->asset_ids.insert(asset_id);
# ifdef __DEBUG_ENABLED
	system::io::printf(stdout, "asset_manager: added asset{id: %u}\n", *asset_id);
# endif
	return asset_id;
}

void mdl::firefly::asset_manager::free_asset(types::id_t __asset_id) {
	types::asset_t& asset = this->asset_d[*__asset_id];
	if (asset.kind >= _ffly_ak_wav_file && asset.kind <=_ffly_ak_wav_file)
	{
		ffly_aud_fad_t *aud_fad = reinterpret_cast<ffly_aud_fad_t*>(asset.data);
		if (aud_fad->fp != nullptr) memory::mem_free(aud_fad->fp);
		memory::mem_free(aud_fad);
	}

	if (asset.id != nullptr) memory::mem_free(asset.id);
	if (asset.data != nullptr) memory::mem_free(asset.data);
}

mdl::firefly::types::err_t mdl::firefly::asset_manager::de_init() {
	types::size_t i = 0;
	for (;i != this->asset_d.size(); i++)
	{
		system::io::printf(stdout, "asset_manager: freed memory for asset{id: %u}\n", i);
		this->free_asset(this->asset_d[i].id);
	}

	delete &this->asset_ids;
	delete &this->asset_d;
}

void mdl::firefly::asset_manager::del_asset(types::id_t __asset_id) {
	std::set<types::id_t>::iterator itr = this->asset_ids.find(__asset_id);
	if (itr == this->asset_ids.end()) {
		printf("failed to look up asset, id? %d\n", *__asset_id);
		return;
	}

	printf("deleting asset, id: %d\n", *__asset_id);

	this->asset(__asset_id) = *(this->asset_d.end()-1);
	*this->asset(__asset_id).id = *__asset_id;

	this->free_asset(__asset_id);

	this->asset_d.resize(this->asset_d.size()-1);
	this->asset_ids.erase(itr);
}

mdl::firefly::types::bool_t mdl::firefly::asset_manager::valid_asset_id(types::id_t __asset_id) {
	return !(this->asset_ids.find(__asset_id) == this->asset_ids.end());}

mdl::u8_t* mdl::firefly::asset_manager::get_asset_data(types::id_t __asset_id) {
	if (!this->valid_asset_id(__asset_id)) return nullptr;
	return this->asset_d[*__asset_id].data;
}

mdl::firefly::types::asset_t mdl::firefly::asset_manager::get_asset(types::id_t __asset_id) noexcept {
	if (!this->valid_asset_id(__asset_id)) throw;
	return this->asset_d[*__asset_id];
}

mdl::firefly::types::asset_t& mdl::firefly::asset_manager::asset(types::id_t __asset_id) {
	return this->asset_d[*__asset_id];}

extern "C" {
ffly_id_t ffly_load_asset(void *__clsp, char *__fdir, char *__fname, mdl_uint_t __kind) {
	static_cast<mdl::firefly::asset_manager*>(__clsp)->load_asset(__fdir, __fname, __kind, 0x0);
}
ffly_id_t ffly_add_asset(void *__clsp, ffly_byte_t *__data, mdl_uint_t __kind) {
	static_cast<mdl::firefly::asset_manager*>(__clsp)->add_asset(__data, __kind);
}
ffly_asset_t* ffly_asset(void *__clsp, ffly_id_t __id) {
	return &static_cast<mdl::firefly::asset_manager*>(__clsp)->asset(__id);
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
