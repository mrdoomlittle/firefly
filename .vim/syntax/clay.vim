if exists("b:current_syntax")
	finish
endif
syn sync fromstart

syn region clay_string start=+"+hs=s+1 end=+"+he=e-1
syn match clay_comment "\s*\zs#.*$"
syn match clay_dec /[0-9]/
syn keyword clay_spec _64 _32 _16 _8
hi clay_dec ctermfg=215 guifg=#ffaf5f "rgb=255,175,95
hi clay_spec ctermfg=160 guifg=#d70000 "rgb=215,0,0
hi clay_string ctermfg=96 guifg=#875f87 "rgb=135,95,135
