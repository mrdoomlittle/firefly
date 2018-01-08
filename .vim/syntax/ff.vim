if exists("b:current_syntax")
	finish
endif

syn match macro "^\s*%\(ifdef\|define\|include\|ifndef\|endif\)\>"
syn region comment_block start="/\*" end="\*/"
syn region string start=+"+ end=+"+
syn match type "\s*\(uint_t\|int_t\|u8_t\|i8_t\|u16_t\|i16_t\|u32_t\|i32_t\|u64_t\|i64_t\|void\)\>"
syn match builtin "^\s*\(print\|exit\)\>"
syn match statment "^\s*\(if\|while\|fn\)\>"
syn region char start=+'+ end=+'+

hi char ctermfg=108 guifg=#87af87 "rgb=135,175,135
hi statment ctermfg=210 guifg=#ff8787 "rgb=255,135,135
hi builtin ctermfg=170 guifg=#d75fd7 "rgb=215,95,215
hi type ctermfg=110 guifg=#87afd7 "rgb=135,175,215
hi string ctermfg=96 guifg=#875f87 "rgb=135,95,135
hi comment_block ctermfg=80 guifg=#5fd7d7 "rgb=95,215,215
hi macro_start ctermfg=67 guifg=#5f87af "rgb=95,135,175
hi define ctermfg=69 guifg=#5f87ff "rgb=95,135,255
