/* sane - Scanner Access Now Easy.
   Copyright (C) 1997 David Mosberger-Tang
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
   If you do not wish that, delete this exception notice.  */

#include "../include/sane/config.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include "../include/sane/sane.h"
#include "../include/sane/sanei_wire.h"

#define BACKEND_NAME	sanei_wire
#include "../include/sane/sanei_backend.h"

void
sanei_w_space (Wire * w, size_t howmuch)
{
  size_t nbytes, left_over;
  int fd = w->io.fd;
  ssize_t nread, nwritten;

  DBG (3, "sanei_w_space: %lu bytes for wire %d\n", (u_long) howmuch, fd);

  if (howmuch > w->buffer.size)
    DBG (2, "sanei_w_space: bigger than buffer (%lu bytes), "
	 "may be flush()\n", (u_long) w->buffer.size);

  if (w->status != 0)
    {
      DBG (1, "sanei_w_space: wire is in invalid state %d\n", 
	   w->status);
      return;
    }

  if (w->buffer.curr + howmuch > w->buffer.end)
    {
      DBG (4, "sanei_w_space: free buffer size is %d\n",
	   w->buffer.end - w->buffer.curr);
      switch (w->direction)
	{
	case WIRE_ENCODE:
	  nbytes = w->buffer.curr - w->buffer.start;
	  w->buffer.curr = w->buffer.start;
	  DBG (4, "sanei_w_space: ENCODE: sending %lu bytes\n", 
	       (u_long) nbytes);
	  while (nbytes > 0)
	    {
	      nwritten = (*w->io.write) (fd, w->buffer.curr, nbytes);
	      if (nwritten < 0)
		{
		  DBG (1, "sanei_w_space: ENCODE: write failed (%d)\n", errno);
		  w->status = errno;
		  return;
		}
	      w->buffer.curr += nwritten;
	      nbytes -= nwritten;
	    }
	  w->buffer.curr = w->buffer.start;
	  w->buffer.end = w->buffer.start + w->buffer.size;
	  DBG (4, "sanei_w_space: ENCODE: free buffer is now %d\n",
	       w->buffer.size);
	  break;

	case WIRE_DECODE:
	  left_over = w->buffer.end - w->buffer.curr;

	  if ((signed) left_over < 0)
	    {
	      DBG (1, "sanei_w_space: DECODE: buffer underflow\n");
	      return;
	    }

	  if (left_over)
	    {
	      DBG (4, "sanei_w_space: DECODE: %lu bytes left in buffer\n",
		   (u_long) left_over);
	      memmove (w->buffer.start, w->buffer.curr, left_over);
	    }
	  w->buffer.curr = w->buffer.start;
	  w->buffer.end = w->buffer.start + left_over;

	  DBG (4, "sanei_w_space: DECODE: receiving data\n");
	  do
	    {
	      nread = (*w->io.read) (fd, w->buffer.end,
				     w->buffer.size - left_over);
	      if (nread <= 0)
		{
		  DBG (2, "sanei_w_space: DECODE: no data received (%d)\n",
		       errno);
		  if (nread == 0)
		    errno = EINVAL;
		  w->status = errno;
		  return;
		}
	      left_over += nread;
	      w->buffer.end += nread;
	    }
	  while (left_over < howmuch);
	  DBG (4, "sanei_w_space: DECODE: %d bytes read\n",
	       w->buffer.end - w->buffer.start);
	  break;

	case WIRE_FREE:
	  DBG (4, "sanei_w_space: FREE: doing nothing for free operation\n");
	  break;
	}
    }
  DBG (4, "sanei_w_space: done\n");
}

void
sanei_w_void (Wire * w)
{
  DBG (3, "sanei_w_void: wire %d (void debug output)\n", w->io.fd);
}

void
sanei_w_array (Wire * w, SANE_Word * len_ptr, void **v,
	       WireCodecFunc w_element, size_t element_size)
{
  SANE_Word len;
  char *val;
  int i;

  DBG (3, "sanei_w_array: wire %d, elements of size %lu\n", w->io.fd,
       (u_long) element_size);

  if (w->direction == WIRE_FREE)
    {
      if (*len_ptr && *v)
	{
	  DBG (4, "sanei_w_array: FREE: freeing array (%d elements)\n",
	       *len_ptr);
	  val = *v;
	  for (i = 0; i < *len_ptr; ++i)
	    {
	      (*w_element) (w, val);
	      val += element_size;
	    }
	  free (*v);
	}
      else
	DBG (1, "sanei_w_array: FREE: tried to free array but *len_ptr or *v "
	     "was NULL\n");
	
      DBG (4, "sanei_w_array: FREE: done\n");
      return;
    }

  if (w->direction == WIRE_ENCODE)
    len = *len_ptr;
  DBG (4, "sanei_w_array: send/receive array length\n");
  sanei_w_word (w, &len);
  DBG (4, "sanei_w_array: array has %d elements\n", len);

  if (w->direction == WIRE_DECODE)
    {
      *len_ptr = len;
      if (len)
	{
	  *v = malloc (len * element_size);
	  if (*v == 0)
	    {
	      /* Malloc failed, so return an error. */
	      DBG (1, "sanei_w_array: DECODE: not enough free memory\n");
	      w->status = ENOMEM;
	      return;
	    }
	}
      else
	*v = 0;
    }

  val = *v;
  DBG (4, "sanei_w_array: transferring array elements\n");
  for (i = 0; i < len; ++i)
    {
      (*w_element) (w, val);
      val += element_size;
    }
  DBG (4, "sanei_w_array: done\n");
}

void
sanei_w_ptr (Wire * w, void **v, WireCodecFunc w_value, size_t value_size)
{
  SANE_Word is_null;

  DBG (3, "sanei_w_ptr: wire %d, value pointer at is %lu bytes\n", w->io.fd,
       (u_long) value_size);

  if (w->direction == WIRE_FREE)
    {
      if (*v && value_size)
	{
	  DBG (4, "sanei_w_ptr: FREE: freeing value\n");
	  (*w_value) (w, *v);
	  free (*v);
	}
      else
	DBG (1, "sanei_w_ptr: FREE: tried to free value but *v or value_size "
	     "was NULL\n");

      DBG (4, "sanei_w_ptr: FREE: done\n");
      return;
    }
  if (w->direction == WIRE_ENCODE)
    is_null = (*v == 0);

  DBG (4, "sanei_w_ptr: send/receive is_null\n");
  sanei_w_word (w, &is_null);

  if (!is_null)
    {
      if (w->direction == WIRE_DECODE)
	{
	  DBG (4, "sanei_w_ptr: DECODE: receive data pointed at\n");
	  *v = malloc (value_size);
	  if (*v == 0)
	    {
	      /* Malloc failed, so return an error. */
	      DBG (1, "sanei_w_ptr: DECODE: not enough free memory\n");
	      w->status = ENOMEM;
	      return;
	    }
	}
      (*w_value) (w, *v);
    }
  else if (w->direction == WIRE_DECODE)
    *v = 0;

  DBG (4, "sanei_w_ptr: done\n");
}

void
sanei_w_byte (Wire * w, SANE_Byte * v)
{
  DBG (3, "sanei_w_byte: wire %d\n", w->io.fd);
  (*w->codec.w_byte) (w, v);
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_byte: value = %d\n", *v);
}

void
sanei_w_char (Wire * w, SANE_Char * v)
{
  DBG (3, "sanei_w_char: wire %d\n", w->io.fd);
  (*w->codec.w_char) (w, v);
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_char: value = %d\n", *v);
}

void
sanei_w_word (Wire * w, SANE_Word * v)
{
  DBG (3, "sanei_w_word: wire %d\n", w->io.fd);
  (*w->codec.w_word) (w, v);
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_word: value = %d\n", *v);
}

void
sanei_w_string (Wire * w, SANE_String * v)
{
  DBG (3, "sanei_w_string: wire %d\n", w->io.fd);
  (*w->codec.w_string) (w, v);
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_string: value = %s\n", *v);
}

void
sanei_w_status (Wire * w, SANE_Status * v)
{
  SANE_Word word = *v;

  DBG (3, "sanei_w_status: wire %d\n", w->io.fd);

  sanei_w_word (w, &word);
  if (w->direction == WIRE_DECODE)
    *v = word;

  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_status: value = %d\n", word);
}

void
sanei_w_bool (Wire * w, SANE_Bool * v)
{
  SANE_Word word = *v;

  DBG (3, "sanei_w_bool: wire %d\n", w->io.fd);
  sanei_w_word (w, &word);
  if (w->direction == WIRE_DECODE)
    *v = word;

  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_bool: value = %s\n",
	 ((word == SANE_TRUE) ? ("true") : ("false")));
}

void
sanei_w_constraint_type (Wire * w, SANE_Constraint_Type * v)
{
  SANE_Word word = *v;

  DBG (3, "sanei_w_constraint_type: wire %d\n", w->io.fd);

  sanei_w_word (w, &word);
  if (w->direction == WIRE_DECODE)
    *v = word;

  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_constraint_type: value = %d\n", word);
}

void
sanei_w_value_type (Wire * w, SANE_Value_Type * v)
{
  SANE_Word word = *v;

  DBG (3, "sanei_w_value_type: wire %d\n", w->io.fd);

  sanei_w_word (w, &word);
  if (w->direction == WIRE_DECODE)
    *v = word;
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_value_type: value = %d\n", word);
}

void
sanei_w_unit (Wire * w, SANE_Unit * v)
{
  SANE_Word word = *v;

  DBG (3, "sanei_w_unit: wire %d\n", w->io.fd);
  sanei_w_word (w, &word);
  if (w->direction == WIRE_DECODE)
    *v = word;

  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_unit: value = %d\n", word);
  /* gosh... all the sane_w_something should be a macro or something */
}

void
sanei_w_action (Wire * w, SANE_Action * v)
{
  SANE_Word word = *v;

  DBG (3, "sanei_w_action: wire %d\n", w->io.fd);

  sanei_w_word (w, &word);
  if (w->direction == WIRE_DECODE)
    *v = word;

  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_action: value = %d\n", word);
}

void
sanei_w_frame (Wire * w, SANE_Frame * v)
{
  SANE_Word word = *v;

  DBG (3, "sanei_w_frame: wire %d\n", w->io.fd);

  sanei_w_word (w, &word);
  if (w->direction == WIRE_DECODE)
    *v = word;

  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_frame: value = %d\n", word);
}

void
sanei_w_range (Wire * w, SANE_Range * v)
{
  DBG (3, "sanei_w_range: wire %d\n", w->io.fd);
  sanei_w_word (w, &v->min);
  sanei_w_word (w, &v->max);
  sanei_w_word (w, &v->quant);
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_range: min/max/step = %f/%f/%f\n",
	 SANE_UNFIX (v->min), SANE_UNFIX (v->max), SANE_UNFIX (v->quant));
}

void
sanei_w_device (Wire * w, SANE_Device * v)
{
  DBG (3, "sanei_w_device: wire %d\n", w->io.fd);
  sanei_w_string (w, (SANE_String *) & v->name);
  sanei_w_string (w, (SANE_String *) & v->vendor);
  sanei_w_string (w, (SANE_String *) & v->model);
  sanei_w_string (w, (SANE_String *) & v->type);
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_device: %s %s from %s (%s)\n", v->name, v->model,
	 v->vendor, v->type);
}

void
sanei_w_device_ptr (Wire * w, SANE_Device ** v)
{
  DBG (3, "sanei_w_device_ptr: wire %d\n", w->io.fd);
  sanei_w_ptr (w, (void **) v, (WireCodecFunc) sanei_w_device, sizeof (**v));
  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_device_ptr: device struct at %p\n", *v);
}

void
sanei_w_option_descriptor (Wire * w, SANE_Option_Descriptor * v)
{
  SANE_Word len;

  DBG (3, "sanei_w_option_descriptor: wire %d\n", w->io.fd);

  sanei_w_string (w, (SANE_String *) & v->name);
  sanei_w_string (w, (SANE_String *) & v->title);
  sanei_w_string (w, (SANE_String *) & v->desc);
  sanei_w_value_type (w, &v->type);
  sanei_w_unit (w, &v->unit);
  sanei_w_word (w, &v->size);
  sanei_w_word (w, &v->cap);
  sanei_w_constraint_type (w, &v->constraint_type);

  if (w->direction != WIRE_FREE)
    DBG (4, "sanei_w_option_descriptor: option %s\n", v->name);

  switch (v->constraint_type)
    {
    case SANE_CONSTRAINT_NONE:
      break;

    case SANE_CONSTRAINT_RANGE:
      sanei_w_ptr (w, (void **) &v->constraint.range,
		   (WireCodecFunc) sanei_w_range, sizeof (SANE_Range));
      break;

    case SANE_CONSTRAINT_WORD_LIST:
      if (w->direction != WIRE_DECODE)
	len = v->constraint.word_list[0] + 1;
      sanei_w_array (w, &len, (void **) &v->constraint.word_list,
		     w->codec.w_word, sizeof (SANE_Word));
      break;

    case SANE_CONSTRAINT_STRING_LIST:
      if (w->direction != WIRE_DECODE)
	{
	  for (len = 0; v->constraint.string_list[len]; ++len);
	  ++len;		/* send NULL string, too */
	}
      sanei_w_array (w, &len, (void **) &v->constraint.string_list,
		     w->codec.w_string, sizeof (SANE_String));
      break;
    }
  DBG (4, "sanei_w_option_descriptor: done\n");
}

void
sanei_w_option_descriptor_ptr (Wire * w, SANE_Option_Descriptor ** v)
{
  DBG (3, "sanei_w_option_descriptor_ptr: wire %d\n", w->io.fd);
  sanei_w_ptr (w, (void **) v,
	       (WireCodecFunc) sanei_w_option_descriptor, sizeof (**v));
  DBG (4, "sanei_w_option_descriptor_ptr: done\n");
}

void
sanei_w_parameters (Wire * w, SANE_Parameters * v)
{
  DBG (3, "sanei_w_parameters: wire %d\n", w->io.fd);
  sanei_w_frame (w, &v->format);
  sanei_w_bool (w, &v->last_frame);
  sanei_w_word (w, &v->bytes_per_line);
  sanei_w_word (w, &v->pixels_per_line);
  sanei_w_word (w, &v->lines);
  sanei_w_word (w, &v->depth);
  if (w->direction != WIRE_FREE)
    DBG (4,
	 "sanei_w_parameters: format/last/bpl/ppl/lines/depth = "
	 "%d/%d/%d/%d/%d/%d\n", v->format, v->last_frame, v->bytes_per_line,
	 v->pixels_per_line, v->lines, v->depth);
}

static void
flush (Wire * w)
{
  DBG (3, "flush: wire %d\n", w->io.fd);
  if (w->direction == WIRE_ENCODE)
    sanei_w_space (w, w->buffer.size + 1);
  else if (w->direction == WIRE_DECODE)
    w->buffer.curr = w->buffer.end = w->buffer.start;
  if (w->status != 0)
    DBG (2, "flush: error status %d\n", w->status);
  DBG (4, "flush: wire flushed\n");
}

void
sanei_w_set_dir (Wire * w, WireDirection dir)
{
  DBG (3, "sanei_w_set_dir: wire %d, old direction WIRE_%s\n", w->io.fd, 
       w->direction == WIRE_ENCODE ? "ENCODE" : 
       (w->direction == WIRE_DECODE ? "DECODE" : "FREE"));
  if (w->direction == WIRE_DECODE && w->buffer.curr != w->buffer.end)
    DBG (1, "sanei_w_set_dir: WARNING: will delete %d bytes from buffer\n",
	 w->buffer.end - w->buffer.curr);
  flush (w);
  w->direction = dir;
  DBG (4, "sanei_w_set_dir: direction changed\n");
  flush (w);
  DBG (3, "sanei_w_set_dir: wire %d, new direction WIRE_%s\n", w->io.fd, 
       dir == WIRE_ENCODE ? "ENCODE" : 
       (dir == WIRE_DECODE ? "DECODE" : "FREE"));
}

void
sanei_w_call (Wire * w,
	      SANE_Word procnum,
	      WireCodecFunc w_arg, void *arg,
	      WireCodecFunc w_reply, void *reply)
{

  DBG (3, "sanei_w_call: wire %d (old status %d)\n", w->io.fd, w->status);
  w->status = 0;
  sanei_w_set_dir (w, WIRE_ENCODE);

  DBG (4, "sanei_w_call: sending request (procedure number: %d)\n", procnum);
  sanei_w_word (w, &procnum);
  (*w_arg) (w, arg);

  if (w->status == 0)
    {
      DBG (4, "sanei_w_call: receiving reply\n");
      sanei_w_set_dir (w, WIRE_DECODE);
      (*w_reply) (w, reply);
    }

  if (w->status != 0)
    DBG (2, "sanei_w_call: error status %d\n", w->status);
  DBG (4, "sanei_w_call: done\n");
}

void
sanei_w_reply (Wire * w, WireCodecFunc w_reply, void *reply)
{
  DBG (3, "sanei_w_reply: wire %d (old status %d)\n", w->io.fd, w->status);
  w->status = 0;
  sanei_w_set_dir (w, WIRE_ENCODE);
  (*w_reply) (w, reply);
  flush (w);
  if (w->status != 0)
    DBG (2, "sanei_w_reply: error status %d\n", w->status);
  DBG (4, "sanei_w_reply: done\n");
}

void
sanei_w_free (Wire * w, WireCodecFunc w_reply, void *reply)
{
  WireDirection saved_dir = w->direction;

  DBG (3, "sanei_w_free: wire %d\n", w->io.fd);

  w->direction = WIRE_FREE;
  (*w_reply) (w, reply);
  w->direction = saved_dir;

  if (w->status != 0)
    DBG (2, "sanei_w_free: error status %d\n", w->status);
  DBG (4, "sanei_w_free: done\n");
}

void
sanei_w_init (Wire * w, void (*codec_init_func) (Wire *))
{
  DBG_INIT ();
  
  DBG (3, "sanei_w_init: initializing\n");
  w->status = 0;
  w->direction = WIRE_ENCODE;
  w->buffer.size = 8192;
  w->buffer.start = malloc (w->buffer.size);

  if (w->buffer.start == 0)
    {
      /* Malloc failed, so return an error. */
      w->status = ENOMEM;
      DBG (1, "sanei_w_init: not enough free memory\n");
    }

  w->buffer.curr = w->buffer.start;
  w->buffer.end = w->buffer.start + w->buffer.size;
  if (codec_init_func != 0)
    {
      DBG (4, "sanei_w_init: initializing codec\n");
      (*codec_init_func) (w);
    }
  DBG (4, "sanei_w_init: done\n");
}

void
sanei_w_exit (Wire * w)
{
  DBG (3, "sanei_w_exit: wire %d\n", w->io.fd);
  if (w->buffer.start)
    {
      DBG (4, "sanei_w_exit: freeing buffer\n");
      free (w->buffer.start);
    }
  w->buffer.start = 0;
  w->buffer.size = 0;
  DBG (4, "sanei_w_exit: done\n");
}
