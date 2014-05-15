/* sane - Scanner Access Now Easy.
   Copyright (C) 2000 Jochen Eisinger <jochen.eisinger@gmx.net>
   This file is part of the SANE package.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.

   As a special exception, the authors of SANE give permission for
   additional uses of the libraries contained in this release of SANE.

   The exception is that, if you link a SANE library with other files
   to produce an executable, this does not by itself cause the
   resulting executable to be covered by the GNU General Public
   License.  Your use of that executable is in no way restricted on
   account of linking the SANE library code into it.

   This exception does not, however, invalidate any other reasons why
   the executable file might be covered by the GNU General Public
   License.

   If you submit changes to SANE to the maintainers to be included in
   a subsequent release, you agree by submitting the changes that
   those changes may be distributed with this exception intact.

   If you write modifications of your own for SANE, it is your choice
   whether to permit this exception to apply to your modifications.
   If you do not wish that, delete this exception notice. 

   This file implements an interface for the Mustek PP chipset A4S2 */

#ifndef sanei_pa4s2_h
#define sanei_pa4s2_h

#include <sys/types.h>
#include <sane/sane.h>

/* options to controll interface operations */
#define SANEI_PA4S2_OPT_DEFAULT		0	/* normal mode */
#define SANEI_PA4S2_OPT_TRY_MODE_UNI	1	/* enable UNI protocoll */
#define SANEI_PA4S2_OPT_ALT_LOCK	2	/* use alternative lock cmd */

/* 
   opens *dev as pa4s2 device, possible values for *dev are "0x378",
   "0x278" and "0x3BC"
 */
extern SANE_Status sanei_pa4s2_open (const char *dev, int *fd);

/* closes the device */
extern void sanei_pa4s2_close (int fd);

/* 
   sets/gets interface options. Options will be taken over, when set is
   SANE_TRUE. These options should be set before the firt device is opened
 */
extern SANE_Status sanei_pa4s2_options (u_int * options, int set);

/*
   enables/disables the device. possible values for enable are SANE_TRUE
   and SANE_FALSE. When the device is disabled, the printer can accessed,
   when it's enabled data can be read/written 
 */
extern SANE_Status sanei_pa4s2_enable (int fd, int enable);

/*
   the function to read a register is split up in three parts, so a register
   can be read more than once
 */

/* selects a register */
extern SANE_Status sanei_pa4s2_readbegin (int fd, u_char reg);
/* reads a register */
extern SANE_Status sanei_pa4s2_readbyte (int fd, u_char * val);
/* terminates reading sequence */
extern SANE_Status sanei_pa4s2_readend (int fd);

/* reads a register */
extern SANE_Status sanei_pa4s2_writebyte (int fd, u_char reg, u_char val);


/*

   these are the values you can read from an A4S2-chipset:

   reg  | bits     | description
   ------+----------+---------------------------------------------
   0x00 | 76543210 | ASIC-ID: 0xA8 - 1013
   |          |          0xA5 - 1015
   |          |          0xA2 - 1505
   ------+----------+---------------------------------------------
   0x01 | 76543210 | pixel: depending on the selected channel,
   |          |        this is a gray/red/green/blue value
   ------+----------+---------------------------------------------
   0x02 |      2   | ASIC 1013: CCD-Type (0/4)
   |        0 | ASIC 1013: 0 = scanner is home
   |          |
   |      2 0 | ASIC 1015: CCD-Type (0/1/4 - 5?)
   |       1  | ASIC 1015: 0 = scanner is home
   |     3    | ASIC 1015: 1 = motor is moving
   ------+----------+---------------------------------------------
   0x03 |      210 | ASIC 1013/1015: bank count
   ------+----------+---------------------------------------------


   register 0x03 can also be written
   registers 0x05 and 0x06 are used for controlling the scanner
 */
#endif
