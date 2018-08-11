if exists("b:current_syntax")
	finish
endif
syn sync fromstart
syn keyword statment func ret r struct
syn keyword type object model body
syn region comment_line start="//"hs=s+2 end="$"
hi comment_line ctermfg=33 guifg=#0087ff "rgb=0,135,255
hi statment ctermfg=203 guifg=#ff5f5f "rgb=255,95,95
hi type ctermfg=65 guifg=#5f875f "rgb=95,135,95
