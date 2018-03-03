if exists("b:current_syntax")
	finish
endif
syn match brewbuiltin "^\s*\(echo\|exit\)\>"
syn region brewstr start=+"+ end=+"+
syn match brewcomment "\s*\zs#.*$"

hi brewcomment ctermfg=33 guifg=#0087ff "rgb=0,135,255
hi brewstr ctermfg=203 guifg=#ff5f5f "rgb=255,95,95
hi brewbuiltin ctermfg=131 guifg=#af5f5f "rgb=175,95,95
