##
## This file is part of the coreboot project.
##
## Copyright (C) 2012 The ChromiumOS Authors.  All rights reserved.
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
##

PROGRAM = cbmem
ROOT = ../../src
CC     ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -O2
CFLAGS += -Wall -Werror
CPPFLAGS += -iquote $(ROOT)/include -iquote $(ROOT)/src/arch/x86

OBJS = $(PROGRAM).o

all: $(PROGRAM)

$(PROGRAM): $(OBJS)

clean:
	rm -f $(PROGRAM) *.o *~

distclean: clean
	rm -f .dependencies

.dependencies:
	@$(CC) $(CFLAGS) $(CPPFLAGS) -MM *.c > .dependencies

.PHONY: all clean distclean

-include .dependencies

junit.xml:
	echo '<?xml version="1.0" encoding="utf-8"?><testsuite>' > $@.tmp
	$(MAKE) clean; \
	echo "<testcase classname='cbmem' name='cbmem'>" >> $@.tmp; \
	$(MAKE) >> $@.tmp.2 2>&1 && type="system-out" || type="failure"; \
	if [ $$type = "failure" ]; then \
		echo "<failure type='buildFailed'>" >> $@.tmp; \
	else \
		echo "<$$type>" >> $@.tmp; \
	fi; \
	echo '<![CDATA[' >> $@.tmp; \
	cat $@.tmp.2 >> $@.tmp; \
	echo "]]></$$type>" >>$@.tmp; \
	rm -f $@.tmp.2; \
	echo "</testcase>" >> $@.tmp; \
	echo "</testsuite>" >> $@.tmp
	mv $@.tmp $@
