#ifndef _SANEI_DEBUG_H
#define _SANEI_DEBUG_H

#include <sane/sanei.h>

/* this header file defines:
 *
 * DBG_INIT() 		 - should be called before any other debug function
 * DBG(level, fmt, ...)	 - prints a message at debug level `level' or higher
 * 			   using a printf-like function
 * IF_DBG(x)             - expands to x if debug support is enabled at
 * 			   compile-time, if NDEBUG is defined at compile-time
 * 			   this macro expands to nothing
 *
 * ENTRY(name)		 - expands to sane_BACKEND_NAME_name
 *
 * before you include sanei_debug.h, you'll have to define
 *
 * BACKEND_NAME		 - the name of your backend without double-quotes
 * STUBS                 - if this is defined, nothing will happen
 * DEBUG_DECLARE_ONLY    - generate prototypes instead of functions
 * DEBUG_NOT_STATIC	 - doesn't generate static functions
 *
 */

#define ENTRY(name)     PASTE(PASTE(PASTE(sane_,BACKEND_NAME),_),name)

#ifdef NDEBUG
  
extern void sanei_debug_ndebug (int level, const char *msg, ...);
	
# define DBG_LEVEL	(0)
# define DBG_INIT()
# define DBG		sanei_debug_ndebug
# define IF_DBG(x)
	
#else /* !NDEBUG */
	
# define DBG_LEVEL      PASTE(sanei_debug_,BACKEND_NAME)

# if defined(BACKEND_NAME) && !defined(STUBS)
#  ifdef DEBUG_DECLARE_ONLY
extern int DBG_LEVEL;
#  else /* !DEBUG_DECLARE_ONLY */
int DBG_LEVEL = 0;
#  endif /* DEBUG_DECLARE_ONLY */
# endif /* BACKEND_NAME && !STUBS */

# define DBG_INIT()                                     \
  sanei_init_debug (STRINGIFY(BACKEND_NAME), &DBG_LEVEL)

# define DBG_LOCAL	PASTE(DBG_LEVEL,_call)


# ifndef STUBS

#  ifdef DEBUG_DECLARE_ONLY

extern void DBG_LOCAL (int level, const char *msg, ...);

#  else /* !DEBUG_DECLARE_ONLY */

#   include <stdarg.h>
	
extern void sanei_debug_msg 
  (int level, int max_level, const char *be, const char *fmt, va_list ap);

#   ifndef DEBUG_NOT_STATIC
static
#   endif /* !DEBUG_NOT_STATIC */
void
DBG_LOCAL (int level, const char *msg, ...)
{
  va_list ap;

  va_start (ap, msg);
  sanei_debug_msg (level, DBG_LEVEL, STRINGIFY(BACKEND_NAME), msg, ap);
  va_end (ap);
}

#  endif /* DEBUG_DECLARE_ONLY */

# endif /* !STUBS */

# define DBG            DBG_LOCAL

extern void sanei_init_debug (const char * backend, int * debug_level_var);
  
# define IF_DBG(x)      x

#endif /* NDEBUG */

#endif /* _SANEI_DEBUG_H */
