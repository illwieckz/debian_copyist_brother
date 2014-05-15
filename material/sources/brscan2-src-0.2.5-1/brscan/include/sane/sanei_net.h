/* sane - Scanner Access Now Easy.
   Copyright (C) 1997-1999 David Mosberger-Tang and Andreas Beck
   This file is part of the SANE package.

   This file is in the public domain.  You may use and modify it as
   you see fit, as long as this copyright message is included and
   that there is an indication as to what modifications have been
   made (if any).

   SANE is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   This file declares SANE application interface.  See the SANE
   standard for a detailed explanation of the interface.  */

#ifndef sanei_net_h
#define sanei_net_h

#include <sane/sane.h>
#include <sane/sanei_wire.h>

#define SANEI_NET_PROTOCOL_VERSION	3

typedef enum
  {
    SANE_NET_LITTLE_ENDIAN = 0x1234,
    SANE_NET_BIG_ENDIAN = 0x4321
  }
SANE_Net_Byte_Order;

typedef enum
  {
    SANE_NET_INIT = 0,
    SANE_NET_GET_DEVICES,
    SANE_NET_OPEN,
    SANE_NET_CLOSE,
    SANE_NET_GET_OPTION_DESCRIPTORS,
    SANE_NET_CONTROL_OPTION,
    SANE_NET_GET_PARAMETERS,
    SANE_NET_START,
    SANE_NET_CANCEL,
    SANE_NET_AUTHORIZE,
    SANE_NET_EXIT
  }
SANE_Net_Procedure_Number;

typedef struct
  {
    SANE_Word version_code;
    SANE_String username;
  }
SANE_Init_Req;

typedef struct
  {
    SANE_Status status;
    SANE_Word version_code;
  }
SANE_Init_Reply;

typedef struct
  {
    SANE_Status status;
    SANE_Device **device_list;
  }
SANE_Get_Devices_Reply;

typedef struct
  {
    SANE_Status status;
    SANE_Word handle;
    SANE_String resource_to_authorize;
  }
SANE_Open_Reply;

typedef struct
  {
    SANE_Word num_options;
    SANE_Option_Descriptor **desc;
  }
SANE_Option_Descriptor_Array;

typedef struct
  {
    SANE_Word handle;
    SANE_Word option;
    SANE_Word action;
    SANE_Word value_type;
    SANE_Word value_size;
    void *value;
  }
SANE_Control_Option_Req;

typedef struct
  {
    SANE_Status status;
    SANE_Word info;
    SANE_Word value_type;
    SANE_Word value_size;
    void *value;
    SANE_String resource_to_authorize;
  }
SANE_Control_Option_Reply;

typedef struct
  {
    SANE_Status status;
    SANE_Parameters params;
  }
SANE_Get_Parameters_Reply;

typedef struct
  {
    SANE_Status status;
    SANE_Word port;
    SANE_Word byte_order;
    SANE_String resource_to_authorize;
  }
SANE_Start_Reply;

typedef struct
  {
    SANE_String resource;
    SANE_String username;
    SANE_String password;
  }
SANE_Authorization_Req;

extern void sanei_w_init_req (Wire *w, SANE_Init_Req *req);
extern void sanei_w_init_reply (Wire *w, SANE_Init_Reply *reply);
extern void sanei_w_get_devices_reply (Wire *w, SANE_Get_Devices_Reply *reply);
extern void sanei_w_open_reply (Wire *w, SANE_Open_Reply *reply);
extern void sanei_w_option_descriptor_array (Wire *w,
					   SANE_Option_Descriptor_Array *opt);
extern void sanei_w_control_option_req (Wire *w, SANE_Control_Option_Req *req);
extern void sanei_w_control_option_reply (Wire *w,
					  SANE_Control_Option_Reply *reply);
extern void sanei_w_get_parameters_reply (Wire *w,
					  SANE_Get_Parameters_Reply *reply);
extern void sanei_w_start_reply (Wire *w, SANE_Start_Reply *reply);
extern void sanei_w_authorization_req (Wire *w, SANE_Authorization_Req *req);

#endif /* sanei_net_h */
