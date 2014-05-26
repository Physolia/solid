/*
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>

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

#ifndef SOLID_DEVICEINTERFACE_H
#define SOLID_DEVICEINTERFACE_H

#include <QtCore/QObject>

#include <solid/solid_export.h>

namespace Solid
{
class Device;
class DevicePrivate;
class Predicate;
class DeviceInterfacePrivate;

/**
 * Base class of all the device interfaces.
 *
 * A device interface describes what a device can do. A device generally has
 * a set of device interfaces.
 */
class SOLID_EXPORT DeviceInterface : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_DECLARE_PRIVATE(DeviceInterface)

    Q_PROPERTY(QString udi READ udi CONSTANT)
    Q_PROPERTY(QString parentUdi READ parentUdi CONSTANT)
    Q_PROPERTY(QString vendor READ vendor CONSTANT)
    Q_PROPERTY(QString product READ product CONSTANT)
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QStringList emblems READ emblems CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)

public:
    /**
     * This enum type defines the type of device interface that a Device can have.
     *
     * - Unknown : An undetermined device interface
     * - Processor : A processor
     * - Block : A block device
     * - StorageAccess : A mechanism to access data on a storage device
     * - StorageDrive : A storage drive
     * - OpticalDrive : An optical drive (CD-ROM, DVD, ...)
     * - StorageVolume : A volume
     * - OpticalDisc : An optical disc
     * - Camera : A digital camera
     * - PortableMediaPlayer: A portable media player
     * - NetworkShare: A network share interface
     */
    enum Type { Unknown = 0, GenericInterface = 1, Processor = 2,
                Block = 3, StorageAccess = 4, StorageDrive = 5,
                OpticalDrive = 6, StorageVolume = 7, OpticalDisc = 8,
                Camera = 9, PortableMediaPlayer = 10,
                Battery = 12, NetworkShare = 14, Last = 0xffff
              };

    /**
     * Destroys a DeviceInterface object.
     */
    virtual ~DeviceInterface();

    /**
     * Indicates if this device interface is valid.
     * A device interface is considered valid if the device it is referring is available in the system.
     *
     * @return true if this device interface's device is available, false otherwise
     */
    bool isValid() const;

    /**
     *
     * @return the class name of the device interface type
     */
    static QString typeToString(Type type);

    /**
     *
     * @return the device interface type for the given class name
     */
    static Type stringToType(const QString &type);

    /**
     *
     * @return a description suitable to display in the UI of the device interface type
     * @since 4.4
     */
    static QString typeDescription(Type type);

    /**
     * Retrieves the Universal Device Identifier (UDI).
     *
     * \warning Don't use the UDI for anything except communication with Solid. Also don't store
     * UDIs as there's no guarantee that the UDI stays the same when the hardware setup changed.
     * The UDI is a unique identifier that is local to the computer in question and for the
     * current boot session. The UDIs may change after a reboot.
     * Similar hardware in other computers may have different values; different
     * hardware could have the same UDI.
     *
     * @since 5.0
     * @return the udi of the device
     */
    QString udi() const;

    /**
     * Retrieves the Universal Device Identifier (UDI)
     * of the Device's parent.
     *
     * @return the udi of the device's parent
     * @since 5.0
     */
    QString parentUdi() const;

    /**
     * Retrieves the name of the device vendor.
     *
     * @return the vendor name
     * @since 5.0
     */
    QString vendor() const;

    /**
     * Retrieves the name of the product corresponding to this device.
     *
     * @return the product name
     * @since 5.0
     */
    QString product() const;

    /**
     * Retrieves the name of the icon representing this device.
     * The naming follows the freedesktop.org specification.
     *
     * @return the icon name
     * @since 5.0
     */
    QString icon() const;

    /**
     * Retrieves the names of the emblems representing the state of this device.
     * The naming follows the freedesktop.org specification.
     *
     * @return the emblem names
     * @since 5.0
     */
    QStringList emblems() const;

    /**
     * Retrieves the description of device.
     *
     * @return the description
     * @since 5.0
     */
    QString description() const;

protected:
    /**
     * @internal
     * Creates a new DeviceInterface object.
     *
     * @param dd the private d member. It will take care of deleting it upon destruction.
     * @param backendObject the device interface object provided by the backend
     */
    DeviceInterface(DeviceInterfacePrivate &dd, QObject *backendObject);

    DeviceInterfacePrivate *d_ptr;

private:
    friend class Device;
    friend class DevicePrivate;
};
}

#endif
