#ifndef QSERIALPORT_OS2_H
#define QSERIALPORT_OS2_H

#ifndef NO_INCL_SAFE_HIMEM_WRAPPERS
#  define NO_INCL_SAFE_HIMEM_WRAPPERS
#endif
#ifndef INCL_DOSDEVICES
#  define INCL_DOSDEVICES
#endif
#ifndef INCL_DOSFILEMGR
#  define INCL_DOSFILEMGR
#endif
#ifndef INCL_DOSDEVIOCTL
#  define INCL_DOSDEVIOCTL
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#include <os2.h>

APIRET qt_os2_serial_ioctl(HFILE handle, ULONG function,
                           PVOID params, ULONG params_size,
                           PVOID data, ULONG data_size);

#if defined(__cplusplus)
}
#endif

#endif
