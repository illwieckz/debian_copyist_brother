#include "sane/sanei_backend.h"

/* Now define the wrappers (we could use aliases here, but go for
   robustness for now...: */

SANE_Status
sane_init (SANE_Int *vc, SANE_Auth_Callback cb)
{
  return ENTRY(init) (vc, cb);
}

SANE_Status
sane_get_devices (const SANE_Device ***dl, SANE_Bool local)
{
  return ENTRY(get_devices) (dl, local);
}

SANE_Status
sane_open (SANE_String_Const name, SANE_Handle *h)
{
  return ENTRY(open) (name, h);
}

const SANE_Option_Descriptor *
sane_get_option_descriptor (SANE_Handle h, SANE_Int opt)
{
  return ENTRY(get_option_descriptor) (h, opt);
}

SANE_Status
sane_control_option (SANE_Handle h, SANE_Int opt, SANE_Action act,
		     void *val, SANE_Word *info)
{
  return ENTRY(control_option) (h, opt, act, val, info);
}

SANE_Status
sane_get_parameters (SANE_Handle h, SANE_Parameters *parms)
{
  return ENTRY(get_parameters) (h, parms);
}

SANE_Status
sane_start (SANE_Handle h)
{
  return ENTRY(start) (h);
}

SANE_Status
sane_read (SANE_Handle h, SANE_Byte *buf, SANE_Int maxlen, SANE_Int *lenp)
{
  return ENTRY(read) (h, buf, maxlen, lenp);
}

SANE_Status
sane_set_io_mode (SANE_Handle h, SANE_Bool non_blocking)
{
  return ENTRY(set_io_mode) (h, non_blocking);
}

SANE_Status
sane_get_select_fd (SANE_Handle h, SANE_Int *fdp)
{
  return ENTRY(get_select_fd) (h, fdp);
}

void
sane_cancel (SANE_Handle h)
{
  ENTRY(cancel) (h);
}

void
sane_close (SANE_Handle h)
{
  ENTRY(close) (h);
}

void
sane_exit (void)
{
  ENTRY(exit) ();
}
