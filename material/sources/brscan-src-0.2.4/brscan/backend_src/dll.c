/* sane - Scanner Access Now Easy.
   Copyright (C) 1996, 1997 David Mosberger-Tang
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

   This file implements a dynamic linking based SANE meta backend.  It
   allows managing an arbitrary number of SANE backends by using
   dynamic linking to load backends on demand.  */

/* Please increase version number with every change 
   (don't forget to update dll.desc) */
#define DLL_VERSION "1.0.5"

#ifdef _AIX
# include "lalloca.h"   /* MUST come first for AIX! */
#endif

#include "sane/config.h"
#include "lalloca.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(HAVE_DLOPEN) && defined(HAVE_DLFCN_H)
# include <dlfcn.h>

  /* Older versions of dlopen() don't define RTLD_NOW and RTLD_LAZY.
     They all seem to use a mode of 1 to indicate RTLD_NOW and some do
     not support RTLD_LAZY at all.  Hence, unless defined, we define
     both macros as 1 to play it safe.  */
# ifndef RTLD_NOW
#  define RTLD_NOW      1
# endif
# ifndef RTLD_LAZY
#  define RTLD_LAZY     1
# endif
# define HAVE_DLL
#endif

/* HP/UX DLL support */
#if defined (HAVE_SHL_LOAD) && defined(HAVE_DL_H)
# include <dl.h>
# define HAVE_DLL
#endif

#include <sys/types.h>

#include "sane/sane.h"
#include "sane/sanei.h"

#define BACKEND_NAME dll
#include "sane/sanei_backend.h"

#ifndef PATH_MAX
# define PATH_MAX       1024
#endif

#include "sane/sanei_config.h"
#define DLL_CONFIG_FILE "dll.conf"
#define DLL_ALIASES_FILE "dll.aliases"

enum SANE_Ops
  {
    OP_INIT = 0,
    OP_EXIT,
    OP_GET_DEVS,
    OP_OPEN,
    OP_CLOSE,
    OP_GET_OPTION_DESC,
    OP_CTL_OPTION,
    OP_GET_PARAMS,
    OP_START,
    OP_READ,
    OP_CANCEL,
    OP_SET_IO_MODE,
    OP_GET_SELECT_FD,
    NUM_OPS
  };

struct backend
  {
    struct backend *next;
    const char *name;
    u_int permanent : 1;        /* is the backend preloaded? */
    u_int loaded : 1;           /* are the functions available? */
    u_int inited : 1;           /* has the backend been initialized? */
    void *handle;               /* handle returned by dlopen() */
    void *(*op[NUM_OPS]) ();
  };

#define BE_ENTRY(be,func)       sane_##be##_##func

#define PRELOAD_DECL(name)                              \
  extern void *BE_ENTRY(name,init) ();                  \
  extern void *BE_ENTRY(name,exit) ();                  \
  extern void *BE_ENTRY(name,get_devices) ();           \
  extern void *BE_ENTRY(name,open) ();                  \
  extern void *BE_ENTRY(name,close) ();                 \
  extern void *BE_ENTRY(name,get_option_descriptor) (); \
  extern void *BE_ENTRY(name,control_option) ();        \
  extern void *BE_ENTRY(name,get_parameters) ();        \
  extern void *BE_ENTRY(name,start) ();                 \
  extern void *BE_ENTRY(name,read) ();                  \
  extern void *BE_ENTRY(name,cancel) ();                \
  extern void *BE_ENTRY(name,set_io_mode) ();           \
  extern void *BE_ENTRY(name,get_select_fd) ();

#define PRELOAD_DEFN(name)                      \
{                                               \
  0 /* next */, #name,                          \
  1 /* permanent */,                            \
  1 /* loaded */,                               \
  0 /* inited */,                               \
  0 /* handle */,                               \
  {                                             \
    BE_ENTRY(name,init),                        \
    BE_ENTRY(name,exit),                        \
    BE_ENTRY(name,get_devices),                 \
    BE_ENTRY(name,open),                        \
    BE_ENTRY(name,close),                       \
    BE_ENTRY(name,get_option_descriptor),       \
    BE_ENTRY(name,control_option),              \
    BE_ENTRY(name,get_parameters),              \
    BE_ENTRY(name,start),                       \
    BE_ENTRY(name,read),                        \
    BE_ENTRY(name,cancel),                      \
    BE_ENTRY(name,set_io_mode),                 \
    BE_ENTRY(name,get_select_fd)                \
  }                                             \
}

#include "dll-preload.c"

struct meta_scanner
  {
    struct backend *be;
    SANE_Handle handle;
  };

struct alias
  {
    struct alias *next;
    char *oldname;
    char *newname;
  };

/*
 * List of available devices, allocated by sane_get_devices, released
 * by sane_exit()
 */
static const SANE_Device **devlist = NULL;
static int devlist_size = 0, devlist_len = 0;

static struct alias *first_alias;
static SANE_Auth_Callback auth_callback;
static struct backend *first_backend;
static const char *op_name[] =
{
  "init", "exit", "get_devices", "open", "close", "get_option_descriptor",
  "control_option", "get_parameters", "start", "read", "cancel",
  "set_io_mode", "get_select_fd"
};

static void *
op_unsupported (void)
{
  DBG(1, "op_unsupported: call to unsupported backend operation\n");
  return (void *) (long) SANE_STATUS_UNSUPPORTED;
}


static SANE_Status
add_backend (const char *name, struct backend **bep)
{
  struct backend *be, *prev;

  DBG(3, "add_backend: adding backend %s\n", name);

  if (strcmp(name,"dll") == 0)
    {
      DBG(0, "add_backend: remove the dll-backend from your dll.conf!\n");
      return SANE_STATUS_GOOD;
    }

  for (prev = 0, be = first_backend; be; prev = be, be = be->next)
    if (strcmp (be->name, name) == 0)
      {
        DBG(1, "add_backend: %s is already there\n", name);
        /* move to front so we preserve order that we'd get with
           dynamic loading: */
        if (prev)
          {
            prev->next = be->next;
            be->next = first_backend;
            first_backend = be;
          }
        if (bep)
          *bep = be;
        return SANE_STATUS_GOOD;
      }

  be = calloc (1, sizeof (*be));
  if (!be)
    return SANE_STATUS_NO_MEM;

  be->name = strdup (name);
  if (!be->name)
    return SANE_STATUS_NO_MEM;
  be->next = first_backend;
  first_backend = be;
  if (bep)
    *bep = be;
  return SANE_STATUS_GOOD;
}

static SANE_Status
load (struct backend *be)
{
#ifdef HAVE_DLL
  int mode = 0;
  char *funcname, *src, *dir, *path = 0;
  char libname[PATH_MAX];
  int i;
  FILE *fp = 0;

#if defined(HAVE_DLOPEN)
# define PREFIX "libsane-"
# ifdef __hpux
#   define POSTFIX ".sl.%u"
# else
#   define POSTFIX ".so.%u"
# endif
  mode = getenv ("LD_BIND_NOW") ? RTLD_NOW : RTLD_LAZY;
#elif defined(HAVE_SHL_LOAD)
# define PREFIX "libsane-"
# define POSTFIX ".sl.%u"
  mode = BIND_DEFERRED;
#else
# error "Tried to compile unsupported DLL."
#endif /* HAVE_DLOPEN */

  DBG(3, "load: loading backend %s\n", be->name);

  /* initialize all ops to "unsupported" so we can "use" the backend
     even if the stuff later in this function fails */
  be->loaded = 1;
  be->handle = 0;
  for (i = 0; i < NUM_OPS; ++i)
    be->op[i] = op_unsupported;

  dir = STRINGIFY(LIBDIR);
  while (dir)
    {
      snprintf (libname, sizeof (libname), "%s/"PREFIX"%s"POSTFIX,
                dir, be->name, V_MAJOR);
      fp = fopen (libname, "r");
      if (fp)
        break;

      if (!path)
        {
          path = getenv ("LD_LIBRARY_PATH");
          if (!path)
            {
              path = getenv ("SHLIB_PATH");     /* for HP-UX */
              if (!path)
                path = getenv ("LIBPATH");      /* for AIX */
            }
          if (!path)
            break;

          path = strdup (path);
          src = path;
        }
      dir = strsep (&src, ":");
    }
  if (path)
    free (path);
  if (!fp)
    {
      DBG(1, "load: couldn't find %s (%s)\n",
          libname, strerror (errno));
      return SANE_STATUS_INVAL;
    }
  fclose (fp);
  DBG(3, "load: dlopen()ing `%s'\n", libname);

#ifdef HAVE_DLOPEN
  be->handle = dlopen (libname, mode);
#elif defined(HAVE_SHL_LOAD)
  be->handle = (shl_t)shl_load (libname, mode, 0L);
#else
# error "Tried to compile unsupported DLL."
#endif /* HAVE_DLOPEN */
  if (!be->handle)
    {
#ifdef HAVE_DLOPEN
      DBG(1, "load: dlopen() failed (%s)\n", dlerror());
#else
      DBG(1, "load: dlopen() failed (%s)\n", strerror (errno));
#endif
      return SANE_STATUS_INVAL;
    }

  /* all is dandy---lookup and fill in backend ops: */
  funcname = alloca (strlen (be->name) + 64);
  for (i = 0; i < NUM_OPS; ++i)
    {
      void *(*op) (void);

      sprintf (funcname, "_sane_%s_%s", be->name, op_name[i]);

      /* First try looking up the symbol without a leading underscore. */
#ifdef HAVE_DLOPEN
      op = (void *(*)(void)) dlsym (be->handle, funcname + 1);
#elif defined(HAVE_SHL_LOAD)
      shl_findsym ((shl_t*)&(be->handle), funcname + 1, TYPE_UNDEFINED, &op);
#else
# error "Tried to compile unsupported DLL."
#endif /* HAVE_DLOPEN */
      if (op)
        be->op[i] = op;
      else
        {
          /* Try again, with an underscore prepended. */
#ifdef HAVE_DLOPEN
          op = (void *(*)()) dlsym (be->handle, funcname);
#elif defined(HAVE_SHL_LOAD)
          shl_findsym (be->handle, funcname, TYPE_UNDEFINED, &op);
#else
# error "Tried to compile unsupported DLL."
#endif /* HAVE_DLOPEN */
          if (op)
            be->op[i] = op;
        }
      if (NULL == op)
        DBG(1, "load: unable to find %s\n", funcname);
    }

  return SANE_STATUS_GOOD;

# undef PREFIX
# undef POSTFIX
#else /* HAVE_DLL */
  DBG(1, "load: ignoring attempt to load `%s'; compiled without dl support\n",
      be->name);
  return SANE_STATUS_UNSUPPORTED;
#endif /* HAVE_DLL */
}

static SANE_Status
init (struct backend *be)
{
  SANE_Status status;
  SANE_Int version;

  if (!be->loaded)
    {
      status = load (be);
      if (status != SANE_STATUS_GOOD)
        return status;
    }

  DBG(3, "init: initializing backend `%s'\n", be->name);

  status = (long) (*be->op[OP_INIT]) (&version, auth_callback);
  if (status != SANE_STATUS_GOOD)
    return status;

  if (SANE_VERSION_MAJOR (version) != V_MAJOR)
    {
      DBG(1, "init: backend `%s' has a wrong major version (%d instead of %d)\n",
	  be->name,  SANE_VERSION_MAJOR (version), V_MAJOR); 
      return SANE_STATUS_INVAL;
    }
  DBG(4, "init: backend `%s' is version %d.%d.%d\n", be->name,
      SANE_VERSION_MAJOR(version), SANE_VERSION_MINOR(version),
      SANE_VERSION_BUILD(version));

  be->inited = 1;

  return SANE_STATUS_GOOD;
}


static void
add_alias (char *line)
{
  const char *command;
  enum { CMD_ALIAS, CMD_HIDE } cmd;
  const char *oldname, *oldend, *newname, *newend;
  size_t oldlen, newlen;
  struct alias *alias;

  command = sanei_config_skip_whitespace(line);
  if( !*command )
    return;
  line = strpbrk(command, " \t");
  if( !line )
    return;
  *line++ = '\0';

  if( strcmp(command, "alias") == 0 )
    cmd = CMD_ALIAS;
  else
  if( strcmp(command, "hide") == 0 )
    cmd = CMD_HIDE;
  else
    return;

  newlen = 0;
  newname = NULL;
  if( cmd == CMD_ALIAS )
    {
      newname = sanei_config_skip_whitespace(line);
      if( !*newname )
        return;
      if( *newname == '\"' )
        {
          ++newname;
          newend = strchr(newname, '\"');
        }
      else
          newend = strpbrk(newname, " \t");
      if( !newend )
        return;

      newlen = newend - newname;
      line = (char*)(newend+1);
    }

  oldname = sanei_config_skip_whitespace(line);
  if( !*oldname )
    return;
  oldend = oldname + strcspn(oldname, " \t");

  oldlen = oldend - oldname;

  alias = malloc(sizeof(struct alias));
  if( alias )
    {
      alias->oldname = malloc(oldlen + newlen + 2);
      if( alias->oldname )
        {
          strncpy(alias->oldname, oldname, oldlen);
          alias->oldname[oldlen] = '\0';
          if( cmd == CMD_ALIAS )
            {
              alias->newname = alias->oldname + oldlen + 1;
              strncpy(alias->newname, newname, newlen);
              alias->newname[newlen] = '\0';
            }
          else
              alias->newname = NULL;

          alias->next = first_alias;
          first_alias = alias;
          return;
        }
      free(alias);
    }
  return;
}


SANE_Status
sane_init (SANE_Int * version_code, SANE_Auth_Callback authorize)
{
  char backend_name[PATH_MAX];
  size_t len;
  FILE *fp;
  int i;

  DBG_INIT();

  auth_callback = authorize;

  DBG(1, "sane_init: SANE dll backend version %s from %s\n", DLL_VERSION,
      PACKAGE_VERSION);

  /* chain preloaded backends together: */
  for (i = 0; i < NELEMS(preloaded_backends); ++i)
    {
      if (!preloaded_backends[i].name)
        continue;
      preloaded_backends[i].next = first_backend;
      first_backend = &preloaded_backends[i];
    }

  /* Return the version number of the sane-backends package to allow
     the frontend to print them. This is done only for net and dll,
     because these backends are usually called by the frontend. */
  if (version_code)
    *version_code = SANE_VERSION_CODE (SANE_DLL_V_MAJOR, SANE_DLL_V_MINOR,
				       SANE_DLL_V_BUILD);

  fp = sanei_config_open (DLL_CONFIG_FILE);
  if (!fp)
    {
      DBG(1, "sane_init: Couldn't open config file (%s): %s\n", 
	  DLL_CONFIG_FILE, strerror (errno));
      return SANE_STATUS_GOOD;    /* don't insist on config file */
    }

  while (sanei_config_read (backend_name, sizeof (backend_name), fp))
    {
      if (backend_name[0] == '#')       /* ignore line comments */
        continue;
      len = strlen (backend_name);

      if (!len)
        continue;               /* ignore empty lines */

      add_backend (backend_name, 0);
    }
  fclose (fp);

  fp = sanei_config_open (DLL_ALIASES_FILE);
  if (!fp)
    return SANE_STATUS_GOOD;    /* don't insist on aliases file */

  while (sanei_config_read (backend_name, sizeof (backend_name), fp))
    {
      if (backend_name[0] == '#')       /* ignore line comments */
        continue;
      len = strlen (backend_name);

      if (!len)
        continue;               /* ignore empty lines */

      add_alias (backend_name);
    }
  fclose (fp);
  return SANE_STATUS_GOOD;
}

void
sane_exit (void)
{
  struct backend *be, *next;
  struct alias *alias;

  DBG(2, "sane_exit: exiting\n");

  for (be = first_backend; be; be = next)
    {
      next = be->next;
      if (be->loaded)
        {
          DBG(3, "sane_exit: calling backend `%s's exit function\n", be->name);
          (*be->op[OP_EXIT]) ();
#ifdef HAVE_DLL

#ifdef HAVE_DLOPEN
          if (be->handle)
            dlclose (be->handle);
#elif defined(HAVE_SHL_LOAD)
          if (be->handle)
            shl_unload(be->handle);
#else
# error "Tried to compile unsupported DLL."
#endif /* HAVE_DLOPEN */

#endif /* HAVE_DLL */
        }
      if (!be->permanent)
        {
          if (be->name)
            free ((void *) be->name);
          free (be);
        }
    }
  first_backend = 0;

  while( (alias = first_alias) != NULL )
    {
      first_alias = first_alias->next;
      free(alias->oldname);
      free(alias);
    }

  if (NULL != devlist)
    { /* Release memory allocated by sane_get_devices(). */
      int i = 0;
      while (devlist[i])
        free(devlist[i++]);
      free(devlist);

      devlist = NULL;
      devlist_size = 0;
      devlist_len = 0;
    }
  DBG(3, "sane_exit: finished\n");
}

/* Note that a call to get_devices() implies that we'll have to load
   all backends.  To avoid this, you can call sane_open() directly
   (assuming you know the name of the backend/device).  This is
   appropriate for the command-line interface of SANE, for example.
 */
SANE_Status
sane_get_devices (const SANE_Device *** device_list, SANE_Bool local_only)
{
  const SANE_Device **be_list;
  struct backend *be;
  SANE_Status status;
  char *full_name;
  int i, num_devs;
  size_t len;
#define ASSERT_SPACE(n)                                                    \
  {                                                                        \
    if (devlist_len + (n) > devlist_size)                                  \
      {                                                                    \
        devlist_size += (n) + 15;                                          \
        if (devlist)                                                       \
          devlist = realloc (devlist, devlist_size * sizeof (devlist[0])); \
        else                                                               \
          devlist = malloc (devlist_size * sizeof (devlist[0]));           \
        if (!devlist)                                                      \
          return SANE_STATUS_NO_MEM;                                       \
      }                                                                    \
  }

  if (devlist)
    for (i = 0; i < devlist_len; ++i)
      free ((void *) devlist[i]);
  devlist_len = 0;

  for (be = first_backend; be; be = be->next)
    {
      if (!be->inited)
        if (init (be) != SANE_STATUS_GOOD)
          continue;

      status = (long) (*be->op[OP_GET_DEVS]) (&be_list, local_only);
      if (status != SANE_STATUS_GOOD || !be_list)
        continue;

      /* count the number of devices for this backend: */
      for (num_devs = 0; be_list[num_devs]; ++num_devs);

      ASSERT_SPACE (num_devs);

      for (i = 0; i < num_devs; ++i)
        {
          SANE_Device *dev;
          char *mem;
          struct alias *alias;

          for(alias = first_alias; alias != NULL; alias = alias->next)
            {
              len = strlen(be->name);
              if( strlen(alias->oldname) <= len )
                  continue;
              if( strncmp(alias->oldname, be->name, len) == 0
                  && alias->oldname[len] == ':'
                  && strcmp(&alias->oldname[len+1], be_list[i]->name) == 0 )
                break;
            }

          if( alias )
            {
              if( !alias->newname )   /* hidden device */
                continue;

              len = strlen(alias->newname);
              mem = malloc(sizeof(*dev) + len + 1);
              if( !mem )
                return SANE_STATUS_NO_MEM;

              full_name = mem + sizeof(*dev);
              strcpy(full_name, alias->newname);
            }
          else
            {
              /* create a new device entry with a device name that is the
                 sum of the backend name a colon and the backend's device
                 name: */
              len = strlen (be->name) + 1 + strlen (be_list[i]->name);
              mem = malloc (sizeof (*dev) + len + 1);
              if (!mem)
                return SANE_STATUS_NO_MEM;

              full_name = mem + sizeof (*dev);
              strcpy (full_name, be->name);
              strcat (full_name, ":");
              strcat (full_name, be_list[i]->name);
            }

          dev = (SANE_Device *) mem;
          dev->name = full_name;
          dev->vendor = be_list[i]->vendor;
          dev->model = be_list[i]->model;
          dev->type = be_list[i]->type;

          devlist[devlist_len++] = dev;
        }
    }

  /* terminate device list with NULL entry: */
  ASSERT_SPACE (1);
  devlist[devlist_len++] = 0;

  *device_list = devlist;
  return SANE_STATUS_GOOD;
}

SANE_Status
sane_open (SANE_String_Const full_name, SANE_Handle * meta_handle)
{
  const char *be_name, *dev_name;
  struct meta_scanner *s;
  SANE_Handle *handle;
  struct backend *be;
  SANE_Status status;
  struct alias *alias;

  for( alias = first_alias; alias != NULL; alias = alias->next )
    {
      if( !alias->newname )
        continue;
      if( strcmp(alias->newname, full_name) == 0 )
        {
          full_name = alias->oldname;
          break;
        }
    }

  dev_name = strchr (full_name, ':');
  if (dev_name)
    {
#ifdef strndupa
      be_name = strndupa (full_name, dev_name - full_name);
#else
      char *tmp;

      tmp = alloca (dev_name - full_name + 1);
      memcpy (tmp, full_name, dev_name - full_name);
      tmp[dev_name - full_name] = '\0';
      be_name = tmp;
#endif
      ++dev_name;               /* skip colon */
    }
  else
    {
      /* if no colon interpret full_name as the backend name; an empty
         backend device name will cause us to open the first device of
         that backend.  */
      be_name = full_name;
      dev_name = "";
    }

  if (!be_name[0])
    be = first_backend;
  else
    for (be = first_backend; be; be = be->next)
      if (strcmp (be->name, be_name) == 0)
        break;

  if (!be)
    {
      status = add_backend (be_name, &be);
      if (status != SANE_STATUS_GOOD)
        return status;
    }

  if (!be->inited)
    {
      status = init (be);
      if (status != SANE_STATUS_GOOD)
        return status;
    }

  status = (long) (*be->op[OP_OPEN]) (dev_name, &handle);
  if (status != SANE_STATUS_GOOD)
    return status;

  s = calloc (1, sizeof (*s));
  if (!s)
    return SANE_STATUS_NO_MEM;

  s->be = be;
  s->handle = handle;
  *meta_handle = s;

  return SANE_STATUS_GOOD;
}

void
sane_close (SANE_Handle handle)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_close(handle=%p)\n", handle);
  (*s->be->op[OP_CLOSE]) (s->handle);
  free (s);
}

const SANE_Option_Descriptor *
sane_get_option_descriptor (SANE_Handle handle, SANE_Int option)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_get_option_descriptor(handle=%p,option=%d)\n", handle, option);
  return (*s->be->op[OP_GET_OPTION_DESC]) (s->handle, option);
}

SANE_Status
sane_control_option (SANE_Handle handle, SANE_Int option,
                     SANE_Action action, void *value, SANE_Word * info)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_control_option(handle=%p,option=%d,action=%d,value=%p,info=%p)\n",
      handle, option, action, value, info);
  return (long) (*s->be->op[OP_CTL_OPTION]) (s->handle, option, action,
                                             value, info);
}

SANE_Status
sane_get_parameters (SANE_Handle handle, SANE_Parameters * params)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_get_parameters(handle=%p,params=%p)\n", handle, params);
  return (long) (*s->be->op[OP_GET_PARAMS]) (s->handle, params);
}

SANE_Status
sane_start (SANE_Handle handle)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_start(handle=%p)\n", handle);
  return (long) (*s->be->op[OP_START]) (s->handle);
}

SANE_Status
sane_read (SANE_Handle handle, SANE_Byte * data, SANE_Int max_length,
           SANE_Int * length)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_read(handle=%p,data=%p,maxlen=%d,lenp=%p)\n",
      handle, data, max_length, length);
  return (long) (*s->be->op[OP_READ]) (s->handle, data, max_length, length);
}

void
sane_cancel (SANE_Handle handle)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_cancel(handle=%p)\n", handle);
  (*s->be->op[OP_CANCEL]) (s->handle);
}

SANE_Status
sane_set_io_mode (SANE_Handle handle, SANE_Bool non_blocking)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_set_io_mode(handle=%p,nonblocking=%d)\n", handle, non_blocking);
  return (long) (*s->be->op[OP_SET_IO_MODE]) (s->handle, non_blocking);
}

SANE_Status
sane_get_select_fd (SANE_Handle handle, SANE_Int * fd)
{
  struct meta_scanner *s = handle;

  DBG(3, "sane_get_select_fd(handle=%p,fdp=%p)\n", handle, fd);
  return (long) (*s->be->op[OP_GET_SELECT_FD]) (s->handle, fd);
}
