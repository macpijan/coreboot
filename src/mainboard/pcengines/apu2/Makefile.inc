#
# This file is part of the coreboot project.
#
# Copyright (C) 2012 Advanced Micro Devices, Inc.
# Copyright (C) 2016 Eltan B.V.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

#romstage-y += agesawrapper.c
romstage-y += BiosCallOuts.c
romstage-y += PlatformGnbPcie.c

#ramstage-y += agesawrapper.c
ramstage-y += BiosCallOuts.c
ramstage-y += PlatformGnbPcie.c

# WIV20141001 START ADD SPD FROM FILE
## DIMM SPD for on-board memory
SPD_BIN = $(obj)/spd.bin

# Order of names in SPD_SOURCES is important!
SPD_SOURCES  = HYNIX-2G-1333 HYNIX-4G-1333-ECC
SPD_DEPS := $(foreach f, $(SPD_SOURCES), src/mainboard/$(MAINBOARDDIR)/spd/$(f).spd.hex)

# Include spd rom data
$(SPD_BIN): $(SPD_DEPS) src/mainboard/$(MAINBOARDDIR)/Makefile.inc
	echo "    create SPD $@"
	for f in $(SPD_DEPS); \
	  do for c in $$(cat $$f | grep -v ^#); \
	    do printf $$(printf '\%o' 0x$$c); \
	  done; \
	done > $@

cbfs-files-y += spd.bin
spd.bin-file := $(SPD_BIN)
spd.bin-type := 0xab
# WIV20141001 END ADD SPD FROM FILE
