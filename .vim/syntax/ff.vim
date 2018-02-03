if exists("b:current_syntax")
	finish
endif

syn match ffmacro "^\s*%\(include\|define\|ifndef\|ifdef\|endif\)\>"
syn region ffcomment_block start="/\*" end="\*/"
syn region ffstring start=+"+ end=+"+
syn match fftype "\s*\(uint_t\|int_t\|u8_t\|i8_t\|u16_t\|i16_t\|u32_t\|i32_t\|u64_t\|i64_t\|void\|float\|char\|struct\)\>"
syn match ffbuiltin "^\s*\(print\|exit\|typedef\)\>"
syn match ffstatment "^\s*\(if\|while\|fn\|ret\|brk\)\>"
syn region ffchar start=+'+ end=+'+
syn keyword ffconstant I8_MIN I8_MAX U8_MAX U16_MAX U32_MAX U64_MAX FFLY_SUCCESS FFLY_FAILURE FFLY_MUTEX_INIT
syn match ffcaps /[A-Z]/

hi ffcaps ctermfg=131 guifg=#af5f5f "rgb=175,95,95
hi ffconstant ctermfg=160 guifg=#d70000 "rgb=215,0,0
hi ffchar ctermfg=108 guifg=#87af87 "rgb=135,175,135
hi ffstatment ctermfg=203 guifg=#ff5f5f "rgb=255,95,95
hi ffbuiltin ctermfg=131 guifg=#af5f5f "rgb=175,95,95
hi fftype ctermfg=65 guifg=#5f875f "rgb=95,135,95
hi ffstring ctermfg=96 guifg=#875f87 "rgb=135,95,135
hi ffcomment_block ctermfg=33 guifg=#0087ff "rgb=0,135,255
hi ffmacro ctermfg=31 guifg=#0087af "rgb=0,135,175
hi ffdefine ctermfg=69 guifg=#5f87ff "rgb=95,135,255
