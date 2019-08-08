/*
    Copyright 2010 Michael Zanetti <mzanetti@kde.org>
    Copyright 2010-2012 Lukáš Tinkl <ltinkl@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "udisksstoragedrive.h"

#include "../shared/udevqt.h"

#include <QDebug>

using namespace Solid::Backends::UDisks2;

StorageDrive::StorageDrive(Device *dev)
    : Block(dev)
{
#if UDEV_FOUND
    UdevQt::Client client(this);
    m_udevDevice = client.deviceByDeviceFile(device());
    m_udevDevice.deviceProperties();
#endif
}

StorageDrive::~StorageDrive()
{
}

qulonglong StorageDrive::size() const
{
    return m_device->prop("Size").toULongLong();
}

bool StorageDrive::isHotpluggable() const
{
    const Solid::StorageDrive::Bus _bus = bus();
#if UDEV_FOUND
    return _bus == Solid::StorageDrive::Usb || _bus == Solid::StorageDrive::Ieee1394 ||
           (m_udevDevice.deviceProperty("UDISKS_SYSTEM").isValid() && !m_udevDevice.deviceProperty("UDISKS_SYSTEM").toBool());
#elif defined(Q_OS_FREEBSD)
    return m_device->prop("bsdisks_IsHotpluggable").toBool();
#else
#error Implement this or stub this out for your platform
#endif
}

bool StorageDrive::isRemovable() const
{
    return m_device->prop("MediaRemovable").toBool() || m_device->prop("Removable").toBool();
}

Solid::StorageDrive::DriveType StorageDrive::driveType() const
{
    const QStringList mediaTypes = m_device->prop("MediaCompatibility").toStringList();

    if (m_device->isOpticalDrive()) { // optical disks
        return Solid::StorageDrive::CdromDrive;
    } else if (mediaTypes.contains("floppy")) {
        return Solid::StorageDrive::Floppy;
    }
#if 0 // TODO add to Solid
    else if (mediaTypes.contains("floppy_jaz")) {
        return Solid::StorageDrive::Jaz;
    } else if (mediaTypes.contains("floppy_zip")) {
        return Solid::StorageDrive::Zip;
    } else if (mediaTypes.contains("flash")) {
        return Solid::StorageDrive::Flash;
    }
#endif
    else if (mediaTypes.contains("flash_cf")) {
        return Solid::StorageDrive::CompactFlash;
    } else if (mediaTypes.contains("flash_ms")) {
        return Solid::StorageDrive::MemoryStick;
    } else if (mediaTypes.contains("flash_sm")) {
        return Solid::StorageDrive::SmartMedia;
    } else if (mediaTypes.contains("flash_sd") || mediaTypes.contains("flash_sdhc")
               || mediaTypes.contains("flash_mmc") || mediaTypes.contains("flash_sdxc")) {
        return Solid::StorageDrive::SdMmc;
    }
    // FIXME: udisks2 doesn't know about xD cards
    else {
        return Solid::StorageDrive::HardDisk;
    }
}

Solid::StorageDrive::Bus StorageDrive::bus() const
{
    const QString bus = m_device->prop("ConnectionBus").toString();
    const QString udevBus = 
#if UDEV_FOUND
        m_udevDevice.deviceProperty("ID_BUS").toString();
#elif defined(Q_OS_FREEBSD)
        m_device->prop("bsdisks_ConnectionBus").toString();
#else
#error Implement this or stub this out for your platform
#endif

    //qDebug() << "bus:" << bus << "udev bus:" << udevBus;

    if (udevBus == "ata") {
#if UDEV_FOUND
        if (m_udevDevice.deviceProperty("ID_ATA_SATA").toInt() == 1) { // serial ATA
            return Solid::StorageDrive::Sata;
        } else { // parallel (classical) ATA
            return Solid::StorageDrive::Ide;
        }
#elif defined(Q_OS_FREEBSD)
        if (m_device->prop("bsdisks_AtaSata").toString() == "sata") { // serial ATA
            return Solid::StorageDrive::Sata;
        } else { // parallel (classical) ATA
            return Solid::StorageDrive::Ide;
        }
#else
#error Implement this or stub this out for your platform
#endif
    } else if (bus == "usb") {
        return Solid::StorageDrive::Usb;
    } else if (bus == "ieee1394") {
        return Solid::StorageDrive::Ieee1394;
    } else if (udevBus == "scsi") {
        return Solid::StorageDrive::Scsi;
    }
#if 0  // TODO add these to Solid
    else if (bus == "sdio") {
        return Solid::StorageDrive::SDIO;
    } else if (bus == "virtual") {
        return Solid::StorageDrive::Virtual;
    }
#endif
    else {
        return Solid::StorageDrive::Platform;
    }
}
