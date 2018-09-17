if exists("b:current_syntax")
	finish
endif
syn sync fromstart

syn region bole_string start=+"+hs=s+1 end=+"+he=e-1
syn match bole_comment "\s*\zs#.*$"
syn region bole_label start="^\a" end=":"he=e-1
syn match bole_dec /[0-9]/
syn keyword bole_spec _64l_u _32l_u _16l_u _8l_u _64l_s _32l_s _16l_s _8l_s
hi bole_dec ctermfg=215 guifg=#ffaf5f "rgb=255,175,95
hi bole_spec ctermfg=160 guifg=#d70000 "rgb=215,0,0
hi bole_string ctermfg=96 guifg=#875f87 "rgb=135,95,135
hi bole_comment ctermfg=33 guifg=#0087ff "rgb=0,135,255
hi bole_label ctermfg=white guifg=white
