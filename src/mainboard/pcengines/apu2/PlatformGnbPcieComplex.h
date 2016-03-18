/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _PLATFORM_GNB_PCIE_COMPLEX_H
#define _PLATFORM_GNB_PCIE_COMPLEX_H

#include <Porting.h>
#include <AGESA.h>

VOID
OemCustomizeInitEarly (
  IN  OUT AMD_EARLY_PARAMS    *InitEarly
  );

#endif //_PLATFORM_GNB_PCIE_COMPLEX_H
