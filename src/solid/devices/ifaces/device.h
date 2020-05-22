/*
    SPDX-FileCopyrightText: 2005 Kevin Ottens <ervin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SOLID_IFACES_DEVICE_H
#define SOLID_IFACES_DEVICE_H

#include <QObject>
#include <QVariant>

#include <QMap>

#include <solid/deviceinterface.h>
#include <solid/device.h>
#include <solid/solidnamespace.h>

namespace Solid
{
namespace Ifaces
{
/**
 * This class specifies the interface a device will have to comply to in order to be used in the system.
 *
 * Backends will have to implement it to gather and modify data in the underlying system.
 * Each device has a set of key/values pair describing its properties. It has also a list of interfaces
 * describing what the device actually is (a cdrom drive, a portable media player, etc.)
 *
 * @author Kevin Ottens <ervin@kde.org>
 */
class Device : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructs a Device
     */
    Device(QObject *parent = nullptr);
    /**
     * Destruct the Device object
     */
    virtual ~Device();

    /**
     * Retrieves the Universal Device Identifier (UDI) of the Device.
     * This identifier is unique for each device in the system.
     *
     * @returns the Universal Device Identifier of the current device
     */
    virtual QString udi() const = 0;

    /**
     * Retrieves the Universal Device Identifier (UDI) of the Device's
     * parent.
     *
     * @returns the Universal Device Identifier of the parent device
     */
    virtual QString parentUdi() const;

    /**
     * Retrieves the name of the device vendor.
     *
     * @return the vendor name
     */
    virtual QString vendor() const = 0;

    /**
     * Retrieves the name of the product corresponding to this device.
     *
     * @return the product name
     */
    virtual QString product() const = 0;

    /**
     * Retrieves the name of the icon representing this device.
     * The naming follows the freedesktop.org specification.
     *
     * @return the icon name
     */
    virtual QString icon() const = 0;

    /**
     * Retrieves the name of the emblems representing the state of this device.
     * The naming follows the freedesktop.org specification.
     *
     * @return the emblem names
     */
    virtual QStringList emblems() const = 0;

    /**
      * Retrieves the display name to use for this device.
      * Same as description when not defined.
      *
      * @return the display name
      * @since 5.71
      */
    QString displayName() const;

    /**
      * Retrieves the description of device.
      *
      * @return the description
      */
    virtual QString description() const = 0;

    /**
     * Tests if a property exist.
     *
     * @param type the device interface type
     * @returns true if the device interface is provided by this device, false otherwise
     */
    virtual bool queryDeviceInterface(const Solid::DeviceInterface::Type &type) const = 0;

    /**
     * Create a specialized interface to interact with the device corresponding to
     * a particular device interface.
     *
     * @param type the device interface type
     * @returns a pointer to the device interface if supported by the device, 0 otherwise
     */
    virtual QObject *createDeviceInterface(const Solid::DeviceInterface::Type &type) = 0;

    /**
     * Register an action for the given device. Each time the same device in another process
     * broadcast the begin or the end of such action, the corresponding slots will be called
     * in the current process.
     *
     * @param actionName name of the action to register
     * @param dest the object receiving the messages when the action begins and ends
     * @param requestSlot the slot processing the message when the action begins
     * @param doneSlot the slot processing the message when the action ends
     */
    void registerAction(const QString &actionName, QObject *dest, const char *requestSlot, const char *doneSlot) const;

    /**
     * Allows to broadcast that an action just got requested on a device to all
     * the corresponding devices in other processes.
     *
     * @param actionName name of the action which just completed
     */
    void broadcastActionRequested(const QString &actionName) const;

    /**
     * Allows to broadcast that an action just completed in a device to all
     * the corresponding devices in other processes.
     *
     * @param actionName name of the action which just completed
     * @param error error code if the action failed
     * @param errorString message describing a potential error
     */
    void broadcastActionDone(const QString &actionName,
                             int error = Solid::NoError,
                             const QString &errorString = QString()) const;

private:
    QString deviceDBusPath() const;
};
}
}

#endif
