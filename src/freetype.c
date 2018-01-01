# include "freetype.h"
# include "system/errno.h"
# include "system/io.h"
# include "data/str_cmb.h"
# include "memory/mem_free.h"
ffly_err_t ffly_ft_init(struct ffly_ft *__ft) {
    if (FT_Init_FreeType(&__ft->library) != 0) {
        ffly_fprintf(ffly_err, "failed to init freetype.\n");
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}

ffly_err_t ffly_ft_ld(struct ffly_ft *__ft, char *__dir, char *__name) {
    char *path = ffly_str_cmb(__dir, ffly_str_cmb("/", __name, 0), _ffly_stc_free_rhs); 
    if (FT_New_Face(__ft->library, path, 0, &__ft->face) != 0) {
        ffly_fprintf(ffly_err, "failed to load new face.\n");
        return FFLY_FAILURE;
    }
    __ffly_mem_free(path);
    return FFLY_SUCCESS;
}

ffly_err_t ffly_ft_set_pixel_size(struct ffly_ft *__ft, mdl_uint_t __width, mdl_uint_t __height) {
    if (FT_Set_Pixel_Sizes(__ft->face, __width, __height) != 0) {
        ffly_fprintf(ffly_err, "failed to set pixel size.\n");
        return FFLY_FAILURE;
    }
    return FFLY_SUCCESS;
}
