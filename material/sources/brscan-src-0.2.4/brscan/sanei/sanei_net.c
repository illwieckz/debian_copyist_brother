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

#include "../include/sane/sane.h"
#include "../include/sane/sanei_net.h"

void
sanei_w_init_req (Wire *w, SANE_Init_Req *req)
{
  sanei_w_word (w, &req->version_code);
  sanei_w_string (w, &req->username);
}

void
sanei_w_init_reply (Wire *w, SANE_Init_Reply *reply)
{
  sanei_w_status (w, &reply->status);
  sanei_w_word (w, &reply->version_code);
}

void
sanei_w_get_devices_reply (Wire *w, SANE_Get_Devices_Reply *reply)
{
  SANE_Word len;

  if (w->direction != WIRE_DECODE)
    {
      if (reply->device_list)
	{
	  for (len = 0; reply->device_list[len]; ++len);
	  ++len;
	}
      else
	len = 0;
    }
  sanei_w_status (w, &reply->status);
  sanei_w_array (w, &len, (void *) &reply->device_list,
		 (WireCodecFunc) sanei_w_device_ptr,
		 sizeof (reply->device_list[0]));
}

void
sanei_w_open_reply (Wire *w, SANE_Open_Reply *reply)
{
  sanei_w_status (w, &reply->status);
  sanei_w_word (w, &reply->handle);
  sanei_w_string (w, &reply->resource_to_authorize);
}

static void
w_option_value (Wire *w, SANE_Word type, SANE_Word size, void **value)
{
  SANE_Word len, element_size;
  WireCodecFunc w_value;

  switch (type)
    {
    case SANE_TYPE_BOOL:
    case SANE_TYPE_INT:
    case SANE_TYPE_FIXED:
      w_value = (WireCodecFunc) sanei_w_word;
      element_size = sizeof (SANE_Word);
      len = size / element_size;
      break;

    case SANE_TYPE_STRING:
      w_value = (WireCodecFunc) sanei_w_char;
      element_size = sizeof (SANE_Char);
      len = size;
      break;

    case SANE_TYPE_BUTTON:
    case SANE_TYPE_GROUP:
      w_value = (WireCodecFunc) sanei_w_void;
      len = 0;
      element_size = 0;
      break;

    default:
      w->status = EINVAL;
      return;
    }
  sanei_w_array (w, &len, value, w_value, element_size);
}

void
sanei_w_option_descriptor_array (Wire *w, SANE_Option_Descriptor_Array *a)
{
  sanei_w_array (w, &a->num_options, (void **) &a->desc,
		 (WireCodecFunc) sanei_w_option_descriptor_ptr,
		 sizeof (a->desc[0]));
}

void
sanei_w_control_option_req (Wire *w, SANE_Control_Option_Req *req)
{
  sanei_w_word (w, &req->handle);
  sanei_w_word (w, &req->option);
  sanei_w_word (w, &req->action);
  /* Up to and including version 2, we incorrectly attempted to encode
     the option value even the action was SANE_ACTION_SET_AUTO.  */
  if (w->version < 3 || req->action != SANE_ACTION_SET_AUTO)
    {
      sanei_w_word (w, &req->value_type);
      sanei_w_word (w, &req->value_size);
      w_option_value (w, req->value_type, req->value_size, &req->value);
    }
}

void
sanei_w_control_option_reply (Wire *w, SANE_Control_Option_Reply *reply)
{
  sanei_w_status (w, &reply->status);
  sanei_w_word (w, &reply->info);
  sanei_w_word (w, &reply->value_type);
  sanei_w_word (w, &reply->value_size);
  w_option_value (w, reply->value_type, reply->value_size, &reply->value);
  sanei_w_string (w, &reply->resource_to_authorize);
}

void
sanei_w_get_parameters_reply (Wire *w, SANE_Get_Parameters_Reply *reply)
{
  sanei_w_status (w, &reply->status);
  sanei_w_parameters (w, &reply->params);
}

void
sanei_w_start_reply (Wire *w, SANE_Start_Reply *reply)
{
  sanei_w_status (w, &reply->status);
  sanei_w_word (w, &reply->port);
  sanei_w_word (w, &reply->byte_order);
  sanei_w_string (w, &reply->resource_to_authorize);
}

void
sanei_w_authorization_req (Wire *w, SANE_Authorization_Req *req)
{
  sanei_w_string (w, &req->resource);
  sanei_w_string (w, &req->username);
  sanei_w_string (w, &req->password);
}
