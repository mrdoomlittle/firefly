if exists("b:current_syntax")
	finish
endif
syn sync fromstart

syn keyword dusbuiltin out syput cas set shell
syn region dusstr start=+"+hs=s+1 end=+"+he=e-1
syn match duscomment "\s*\zs#.*$"
hi dusstr ctermfg=203 guifg=#ff5f5f "rgb=255,95,95
hi dusbuiltin ctermfg=131 guifg=#af5f5f "rgb=175,95,95
hi duscomment ctermfg=33 guifg=#0087ff "rgb=0,135,255
