#include "qserialportinfo.h"
#include "qserialportinfo_p.h"
#ifdef Q_OS_OS2
#include "qserialport_os2.h"
#endif

QT_BEGIN_NAMESPACE

// OS/2 exposes serial devices by name (COM1, COM2, ...), not below /dev.
QList<QSerialPortInfo> availablePortsByFiltersOfDevices(bool &ok)
{
    QList<QSerialPortInfo> serialPortInfoList;

#ifdef Q_OS_OS2
    for (int i = 1; i <= 9; ++i) {
        const QString portName = QStringLiteral("COM%1").arg(i);
        const QByteArray localName = portName.toLocal8Bit();
        HFILE handle = 0;
        ULONG action = 0;
        const APIRET openResult = DosOpen(reinterpret_cast<PCSZ>(localName.constData()),
                                          &handle, &action, 0,
                                          FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS,
                                          OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE, NULL);
        if (openResult != 0)
            continue;

        const APIRET closeResult = DosClose(handle);
        if (closeResult != 0)
            continue;

        QSerialPortInfoPrivate priv;
        priv.portName = portName;
        priv.device = portName;
        serialPortInfoList.append(QSerialPortInfo(priv));
    }
#else
    for (int i = 1; i <= 9; ++i) {
        // Try to open the port first to check if it actually exists
        QSerialPortInfoPrivate priv;
        priv.portName = QStringLiteral("COM%1").arg(i);
        priv.device = priv.portName;
        
        QSerialPort serial;
        serial.setPort(priv.portName);
        bool isOpenable = serial.open(QIODevice::ReadWrite);
        
        // Only add port to list if it can be opened
        if (isOpenable) {
            serialPortInfoList.append(QSerialPortInfo(priv));
            serial.close();
        }
    }
#endif

    ok = true;
    return serialPortInfoList;
}

QList<QSerialPortInfo> QSerialPortInfo::availablePorts()
{
    bool ok = false;
    return availablePortsByFiltersOfDevices(ok);
}

QString QSerialPortInfoPrivate::portNameToSystemLocation(const QString &source)
{
    return source;
}

QString QSerialPortInfoPrivate::portNameFromSystemLocation(const QString &source)
{
    return source;
}

QT_END_NAMESPACE