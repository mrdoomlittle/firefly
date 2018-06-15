if exists("b:current_syntax")
	finish
endif
syn sync fromstart

syn region brewlabel start="^\a" end=","he=e-1
syn region brewdirective start="^\." end="$"
syn keyword brewbuiltin echo exit end
syn region brewstr start=+"+hs=s+1 end=+"+he=e-1
syn match brewcomment "\s*\zs#.*$"

hi brewdirective ctermfg=216 guifg=#ffaf87 "rgb=255,175,135
hi brewcomment ctermfg=33 guifg=#0087ff "rgb=0,135,255
hi brewstr ctermfg=203 guifg=#ff5f5f "rgb=255,95,95
hi brewbuiltin ctermfg=131 guifg=#af5f5f "rgb=175,95,95
hi brewlabel ctermfg=224 guifg=#ffd7d7 "rgb=255,215,215
