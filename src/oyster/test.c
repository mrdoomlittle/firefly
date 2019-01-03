# include "connect.h"
# include "../ffly_def.h"
# include "../string.h"
# include "../stdio.h"
# include "../system/util/hash.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	char const **arg = __argv+1;
	char const *port = NULL;
	char const *ip_adr = NULL;
	while(arg != __argv+__argc) {
		if (!strcmp(*arg, "-port"))
			port = *(++arg);
		else if (!strcmp(*arg, "-ip"))
			ip_adr = *(++arg);
		arg++;
	}

	if (!port) {
		printf("missing port number.\n");
		return -1;
	}

	if (!ip_adr) {
		printf("missing ip address.\n");
		return -1;
	}

	printf("port: %s, address: %s\n", port, ip_adr);
	ff_db_ctrp ctor;
	// create connector
	ff_err_t err;
	ctor = ff_db_ctr(ffly_hash("firefly", 7), ip_adr, ffly_stno(port), &err);
	if (err != FFLY_SUCCESS) {

	}

	ff_db_ctr_login(ctor, "root", ffly_hash("none", 4));

	char buf[67];
	ff_uint_t pile, rec;
	if (ff_db_ctr_exist(ctor, 0, &err) == -1) {
		ff_db_ctr_creat_pile(ctor, &pile);
		ff_db_ctr_rivet(ctor, 0, pile);
		ff_db_ctr_bind(ctor, 0, pile, offsetof(struct ffdb_pile, no));

		printf("pile, slotno: %u\n", pile);

		ff_db_ctr_creat_record(ctor, pile, &rec,67);
		ff_db_ctr_rivet(ctor, 1, rec);
		ff_db_ctr_bind(ctor, 1, rec, offsetof(struct ffdb_record, no));

		printf("record, slotno: %u\n", rec);

		ff_db_ctr_record_alloc(ctor, rec);
		strcpy(buf, "https://github.com/mrdoomlittle/");
		ff_db_ctr_write(ctor, pile, rec, 0, buf, 67);
	} else {
		ff_db_ctr_acquire_slot(ctor, &pile);
		ff_db_ctr_acquire_slot(ctor, &rec);

		ff_db_ctr_rivetto(ctor, 0, pile);
		ff_db_ctr_rivetto(ctor, 1, rec);

		ff_db_ctr_read(ctor, pile, rec, 0, buf, 67);
		printf("record: %s\n", buf);
	}

	ff_db_ctr_logout(ctor);
	ff_db_ctr_shutdown(ctor);

	ff_db_ctr_destroy(ctor);
	return 0;
}
