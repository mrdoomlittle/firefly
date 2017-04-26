#!/bin/sh
. ./compile.sh bin/ffly_test src/ffly_test.o --ffly-test "--debug-enabled --with-mem-tracker --force-cuda"
