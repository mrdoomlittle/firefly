if exists("b:current_syntax")
	finish
endif

syn region ffc_string start=+"+ end=+"+
syn match ffc_comment "\s*\zs#.*$"
syn match ffc_spec "\s*\(_64l_u\|_32l_u\|_16l_u\|_8l_u\|_64l_s\|_32l_s\|_16l_s\|_8l_s\)\>"
hi ffc_spec ctermfg=160 guifg=#d70000 "rgb=215,0,0
hi ffc_string ctermfg=96 guifg=#875f87 "rgb=135,95,135
hi ffc_comment ctermfg=33 guifg=#0087ff "rgb=0,135,255
