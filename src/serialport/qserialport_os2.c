#include "qserialport_os2.h"

APIRET qt_os2_serial_ioctl(HFILE handle, ULONG function,
                           PVOID params, ULONG params_size,
                           PVOID data, ULONG data_size)
{
    ULONG params_length = params_size;
    ULONG data_length = data_size;
    return DosDevIOCtl(handle, IOCTL_ASYNC, function,
                       params, params_size, &params_length,
                       data, data_size, &data_length);
}
