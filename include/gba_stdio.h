#ifndef _GBA_STDIO_H_
#define _GBA_STDIO_H_

#if !defined(GBA_STDIO_NEW) && !defined(GBA_STDIO_LEGACY)
#error Need to specify whether to use new GBA Stdio (#define GBA_STDIO_NEW)\
  -- or --  legacy GBA Stdio (#define GBA_STDIO_LEGACY).
#elif defined(GBA_STDIO_NEW)
#include "gba_stdio_new.h"
#else
#include "gba_stdio_legacy.h"
#endif

#endif  /* _GBA_STDIO_H_ */
