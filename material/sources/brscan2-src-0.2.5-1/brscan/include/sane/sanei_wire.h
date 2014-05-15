/* sane - Scanner Access Now Easy.
   Copyright (C) 1997 David Mosberger-Tang
   This file is part of the SANE package.

   SANE is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   SANE is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with sane; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Support routines to translate internal datatypes into a wire-format
   (used for RPCs and to save/restore options).  */

#ifndef sanei_wire_h
#define sanei_wire_h

#include <sys/types.h>

typedef enum
  {
    WIRE_ENCODE = 0,
    WIRE_DECODE,
    WIRE_FREE
  }
WireDirection;

struct Wire;

typedef void (*WireCodecFunc) (struct Wire *w, void *val_ptr);
typedef ssize_t (*WireReadFunc) (int fd, void * buf, size_t len);
typedef ssize_t (*WireWriteFunc) (int fd, const void * buf, size_t len);

typedef struct Wire
  {
    int version;		/* protocol version in use */
    WireDirection direction;
    int status;
    struct
      {
	WireCodecFunc w_byte;
	WireCodecFunc w_char;
	WireCodecFunc w_word;
	WireCodecFunc w_string;
      }
    codec;
    struct
      {
	size_t size;
	char *curr;
	char *start;
	char *end;
      }
    buffer;
    struct
      {
	int fd;
	WireReadFunc read;
	WireWriteFunc write;
      }
    io;
  }
Wire;

extern void sanei_w_init (Wire *w, void (*codec_init)(Wire *));
extern void sanei_w_exit (Wire *w);
extern void sanei_w_space (Wire *w, size_t howmuch);
extern void sanei_w_void (Wire *w);
extern void sanei_w_byte (Wire *w, SANE_Byte *v);
extern void sanei_w_char (Wire *w, SANE_Char *v);
extern void sanei_w_word (Wire *w, SANE_Word *v);
extern void sanei_w_bool (Wire *w, SANE_Bool *v);
extern void sanei_w_ptr (Wire *w, void **v, WireCodecFunc w_value,
                         size_t value_size);
extern void sanei_w_string (Wire *w, SANE_String *v);
extern void sanei_w_status (Wire *w, SANE_Status *v);
extern void sanei_w_constraint_type (Wire *w, SANE_Constraint_Type *v);
extern void sanei_w_value_type (Wire *w, SANE_Value_Type *v);
extern void sanei_w_unit (Wire *w, SANE_Unit *v);
extern void sanei_w_action (Wire *w, SANE_Action *v);
extern void sanei_w_frame (Wire *w, SANE_Frame *v);
extern void sanei_w_range (Wire *w, SANE_Range *v);
extern void sanei_w_range_ptr (Wire *w, SANE_Range **v);
extern void sanei_w_device (Wire *w, SANE_Device *v);
extern void sanei_w_device_ptr (Wire *w, SANE_Device **v);
extern void sanei_w_option_descriptor (Wire *w, SANE_Option_Descriptor *v);
extern void sanei_w_option_descriptor_ptr (Wire *w,
					   SANE_Option_Descriptor **v);
extern void sanei_w_parameters (Wire *w, SANE_Parameters *v);

extern void sanei_w_array (Wire *w, SANE_Word *len, void **v,
			   WireCodecFunc w_element, size_t element_size);

extern void sanei_w_set_dir (Wire *w, WireDirection dir);
extern void sanei_w_call (Wire *w, SANE_Word proc_num,
			  WireCodecFunc w_arg, void *arg,
			  WireCodecFunc w_reply, void *reply);
extern void sanei_w_reply (Wire *w, WireCodecFunc w_reply, void *reply);
extern void sanei_w_free (Wire *w, WireCodecFunc w_reply, void *reply);

#endif /* sanei_wire_h */
