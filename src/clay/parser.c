# include "parser.h"
# include "lexer.h"
# include "input.h"
# include "../ffly_def.h"
# include "../stdio.h"
# include "memalloc.h"
# include "hash.h"
# include "../system/string.h"
# include "../string.h"
# define new_node ((clay_nodep)clay_mem_alloc(sizeof(struct clay_node)))

clay_nodep static
val_or_trove(ffly_clayp __clay) {
	ff_u8_t tok;
	tok = clay_nexttok(__clay);

	char const *name;
	name = (char const*)clay_tok_data;
	printf("name: %s\n", name);

	clay_nodep n;

	n = new_node;
	n->name = name;
	n->nlen = clay_tok_sz;

	tok = clay_nexttok(__clay);
	if (tok == _clay_tok_str) {
		n->type = _clay_val;
		n->val_id = _clay_str;
		n->sz = clay_tok_sz;
		n->p = clay_tok_data;
	} else if (tok == _clay_tok_no) {
		n->type = _clay_val;
		*(ff_u64_t*)n->val = ffly_stno(clay_tok_data);
		
		clay_reckontok(__clay, _clay_tok_keywd, _clay_tilde);

		tok = clay_nexttok(__clay);
		char *p;
		p = ((char*)clay_tok_data)+1;
		printf("--> %s\n", p-1);
		switch(*p) {
			case '8':
				goto _8;
			case '1':
				goto _16;
			case '3':
				goto _32;
			case '6':
				goto _64;
		}	

		_64:
		n->val_id = _clay_64;
		goto _end;
		_32:
		n->val_id = _clay_32;
		goto _end;
		_16:
		n->val_id = _clay_16;
		goto _end;
		_8:
		n->val_id = _clay_8;
		goto _end;
	} else if (clay_tok_val == _clay_l_brace) {
		n->type = _clay_trove;
		n->link = NULL;
		clay_nodep nn;
		nn = NULL;
	_again:
		if (!clay_nexttokis(__clay, _clay_tok_keywd, _clay_r_brace)) {
			nn = val_or_trove(__clay);
			nn->link = n->link;
			n->link = nn;	
			goto _again;
		}
	}
_end:
	return n;
}

clay_nodep clay_parser(ffly_clayp __clay) {
	ff_u8_t tok;
	clay_nodep top = NULL, end = NULL, n;
_again:
	if (clay_at_eof())
		goto _end;
	tok = clay_peektok(__clay);

	switch(tok) {
		case _clay_tok_str:
			n = val_or_trove(__clay);
		break;
	}

	if (!top)
		top = n;
	if (end != NULL) {
		end->next = n;
	}
	end = n;
	goto _again;
_end:
	end->next = NULL;
	return top;
}
