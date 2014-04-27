/*
 *   Copyright (C) 2013 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLID_DECALARATIVE_DEVICES_H
#define SOLID_DECALARATIVE_DEVICES_H

#include <QObject>
#include <QList>

#include <solid/predicate.h>
#include <solid/deviceinterface.h>

#include <QSharedPointer>

namespace Solid
{

class DeviceNotifier;
class DevicesQueryPrivate;

/**
 * A class that watches the devices known to the solid system.
 *
 * It behaves similarly to Solid::DeviceNotifier, but
 * adds some convenience methods which allow it to
 * watch only the devices matching a specified query
 * (formatted for Solid::Predicate).
 *
 * It is intended to be used from QML like this:
 *
 * <code>
 *    Solid.Devices {
 *        id: allDevices
 *    }
 *
 *    Solid.Devices {
 *        id: networkShares
 *        query: "IS NetworkShare"
 *    }
 *
 *    Solid.Devices {
 *        id: mice
 *        query: "PointingDevice.type == 'Mouse'"
 *    }
 *
 *    Text {
 *        text: "Total number of devices: " + allDevices.count
 *    }
 *
 *    Text {
 *        text: "NFS url: " + networkShares.device(
 *            networkShares.devices[0], "NetworkShare"
 *        ).url
 *    }
 * </code>
 */
class DeclarativeDevices: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)
    Q_PROPERTY(QStringList deviceList READ deviceList NOTIFY deviceListChanged)
    Q_PROPERTY(QList<QObject *> devices READ devices NOTIFY devicesChanged)

public:
    explicit DeclarativeDevices(QObject *parent = Q_NULLPTR);
    ~DeclarativeDevices();

Q_SIGNALS:
    /**
     * Emitted when a new device matching the specified
     * query arrives
     * @param udi UDI of the new device
     */
    void deviceAdded(const QString &udi) const;

    /**
     * Emitted when a device matching the specified
     * query disappears
     * @param udi UDI of the device
     */
    void deviceRemoved(const QString &udi) const;

    /**
     * Emitted when the number of devices that
     * match the specified query has changed
     * @param count new device count
     */
    void countChanged(int count) const;

    /**
     * Emitted when the list of device UDIs that
     * match the specified query has changed
     * @param devices list of UDIs
     */
    void deviceListChanged(const QStringList &deviceList) const;

    /**
     * Emitted when the list of devices that
     * match the specified query has changed
     * @param devices list
     */
    void devicesChanged(const QList<QObject *> &devices) const;

    /**
     * Emitted when the query has changed
     * @param query new query
     */
    void queryChanged(const QString &query) const;

    /**
     * Emitted when the empty property changes
     * @param empty is the device list empty
     */
    void emptyChanged(bool empty) const;

public:
    /**
     * Retrieves the number of the devices that
     * match the specified query
     * @return device count
     */
    int count() const;

    /**
     * Retrieves whether there are devices matching
     * the specified query
     * @return true if there are no matching devices
     */
    bool isEmpty() const;

    /**
     * Retrieves the list of UDIs of the devices that
     * match the specified query
     */
    QStringList deviceList() const;

    /**
     * Retrieves the list of devices that match the
     * specified query
     */
    QList<QObject *> devices() const;

    /**
     * Query to check the devices against. It needs
     * to be formatted for Solid::Predicate.
     * @see Solid::Predicate
     */
    QString query() const;

    /**
     * Sets the query to filter the devices.
     * @param query new query
     */
    void setQuery(const QString &query);

public Q_SLOTS:
    /**
     * Retrieves an interface object to the specified device
     * @param udi udi of the desired device
     * @param type how to interpret the device
     * @see Solid::Device::asDeviceInterface
     */
    QObject *device(const QString &udi, const QString &type);

private Q_SLOTS:
    void addDevice(const QString &udi);
    void removeDevice(const QString &udi);

    /**
     * Initializes the backend object
     */
    void initialize() const;

    /**
     * Frees up the backend and sends the appropriate events
     */
    void reset();

private:
    QString m_query;

    mutable QSharedPointer<DevicesQueryPrivate> m_backend;
};

} // namespace Solid

#endif

