/*
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>
    Copyright 2012 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2014 Kai Uwe Broulik <kde@privat.broulik.de>

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

#ifndef SOLID_BATTERY_H
#define SOLID_BATTERY_H

#include <solid/solid_export.h>

#include <solid/deviceinterface.h>

namespace Solid
{
class BatteryPrivate;
class Device;

/**
 * This device interface is available on batteries.
 */
class SOLID_EXPORT Battery : public DeviceInterface
{
    Q_OBJECT
    Q_ENUMS(BatteryType ChargeState)
    Q_PROPERTY(bool present READ isPresent NOTIFY presentStateChanged)
    Q_PROPERTY(BatteryType type READ type CONSTANT)
    Q_PROPERTY(int chargePercent READ chargePercent NOTIFY chargePercentChanged)
    Q_PROPERTY(int capacity READ capacity NOTIFY capacityChanged)
    Q_PROPERTY(bool rechargeable READ isRechargeable CONSTANT)
    Q_PROPERTY(bool powerSupply READ isPowerSupply NOTIFY powerSupplyStateChanged)
    Q_PROPERTY(ChargeState chargeState READ chargeState NOTIFY chargeStateChanged)
    Q_PROPERTY(qlonglong timeToEmpty READ timeToEmpty NOTIFY timeToEmptyChanged)
    Q_PROPERTY(qlonglong timeToFull READ timeToFull NOTIFY timeToFullChanged)
    Q_PROPERTY(double energy READ energy NOTIFY energyChanged)
    Q_PROPERTY(double energyRate READ energyRate NOTIFY energyRateChanged)
    Q_PROPERTY(double voltage READ voltage NOTIFY voltageChanged)
    Q_PROPERTY(double temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(bool recalled READ isRecalled)
    Q_PROPERTY(QString recallVendor READ recallVendor)
    Q_PROPERTY(QString recallUrl READ recallUrl)
    Q_PROPERTY(QString serial READ serial)
    Q_DECLARE_PRIVATE(Battery)
    friend class Device;

public:
    /**
     * This enum type defines the type of the device holding the battery
     *
     * - PdaBattery : A battery in a Personal Digital Assistant
     * - UpsBattery : A battery in an Uninterruptible Power Supply
     * - PrimaryBattery : A primary battery for the system (for example laptop battery)
     * - MouseBattery : A battery in a mouse
     * - KeyboardBattery : A battery in a keyboard
     * - KeyboardMouseBattery : A battery in a combined keyboard and mouse
     * - CameraBattery : A battery in a camera
     * - PhoneBattery : A battery in a phone
     * - MonitorBattery : A battery in a monitor
     * - UnknownBattery : A battery in an unknown device
     */
    enum BatteryType { UnknownBattery, PdaBattery, UpsBattery,
                       PrimaryBattery, MouseBattery, KeyboardBattery,
                       KeyboardMouseBattery, CameraBattery,
                       PhoneBattery, MonitorBattery
                     };

    /**
     * This enum type defines charge state of a battery
     *
     * - NoCharge : Battery charge is stable, not charging or discharging or
     *              the state is Unknown
     * - Charging : Battery is charging
     * - Discharging : Battery is discharging
     * - FullyCharged: The battery is fully charged; a battery not neccessarily
     *                 charges up to 100%
     */
    enum ChargeState { NoCharge, Charging, Discharging, FullyCharged };

    /**
      * Technology used in the battery
      *
      * 0: Unknown
      * 1: Lithium ion
      * 2: Lithium polymer
      * 3: Lithium iron phosphate
      * 4: Lead acid
      * 5: Nickel cadmium
      * 6: Nickel metal hydride
      */
    enum Technology { UnknownTechnology = 0, LithiumIon, LithiumPolymer, LithiumIronPhosphate,
                      LeadAcid, NickelCadmium, NickelMetalHydride
                    };

private:
    /**
     * Creates a new Battery object.
     * You generally won't need this. It's created when necessary using
     * Device::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see Solid::Device::as()
     */
    explicit Battery(QObject *backendObject);

public:
    /**
     * Destroys a Battery object.
     */
    virtual ~Battery();

    /**
     * Get the Solid::DeviceInterface::Type of the Battery device interface.
     *
     * @return the Battery device interface type
     * @see Solid::DeviceInterface::Type
     */
    static Type deviceInterfaceType()
    {
        return DeviceInterface::Battery;
    }

    /**
     * Indicates if this battery is currently present in its bay.
     *
     * @return true if the battery is present, false otherwise
     */
    bool isPresent() const;

    /**
     * Retrieves the type of device holding this battery.
     *
     * @return the type of device holding this battery
     * @see Solid::Battery::BatteryType
     */
    Solid::Battery::BatteryType type() const;

    /**
     * Retrieves the current charge level of the battery normalised
     * to percent.
     *
     * @return the current charge level normalised to percent
     */
    int chargePercent() const;

    /**
     * Retrieves the battery capacity normalised to percent,
     * meaning how much energy can it hold compared to what it is designed to.
     * The capacity of the battery will reduce with age.
     * A capacity value less than 75% is usually a sign that you should renew your battery.
     *
     * @since 4.11
     * @return the battery capacity normalised to percent
     */
    int capacity() const;

    /**
     * Indicates if the battery is rechargeable.
     *
     * @return true if the battery is rechargeable, false otherwise (one time usage)
     */
    bool isRechargeable() const;

    /**
     * Indicates if the battery is powering the machine.
     *
     * @return true if the battery is powersupply, false otherwise
     */
    bool isPowerSupply() const;

    /**
     * Retrieves the current charge state of the battery. It can be in a stable
     * state (no charge), charging or discharging.
     *
     * @return the current battery charge state
     * @see Solid::Battery::ChargeState
     */
    Solid::Battery::ChargeState chargeState() const;

    /**
     * Time (in seconds) until the battery is empty.
     *
     * @return time until the battery is empty
     * @since 5.0
     */
    qlonglong timeToEmpty() const;

    /**
     * Time (in seconds) until the battery is full.
     *
     * @return time until the battery is full
     * @since 5.0
     */
    qlonglong timeToFull() const;

    /**
      * Retrieves the technology used to manufacture the battery.
      *
      * @return the battery technology
      * @see Solid::Battery::Technology
      */
    Solid::Battery::Technology technology() const;

    /**
      * Amount of energy (measured in Wh) currently available in the power source.
      *
      * @return amount of battery energy in Wh
      */
    double energy() const;

    /**
      * Amount of energy being drained from the source, measured in W.
      * If positive, the source is being discharged, if negative it's being charged.
      *
      * @return battery rate in Watts
      *
      */
    double energyRate() const;

    /**
      * Voltage in the Cell or being recorded by the meter.
      *
      * @return voltage in Volts
      */
    double voltage() const;

    /**
     * The temperature of the battery in degrees Celsius.
     *
     * @return the battery temperature in degrees Celsius
     * @since 5.0
     */
    double temperature() const;

    /**
     * The battery may have been recalled by the vendor due to a suspected fault.
     *
     * @return true if the battery has been recalled, false otherwise
     * @since 5.0
     */
    bool isRecalled() const;

    /**
     * The vendor that has recalled the battery.
     *
     * @return the vendor name that has recalled the battery
     * @since 5.0
     */
    QString recallVendor() const;

    /**
     * The website URL of the vendor that has recalled the battery.
     *
     * @return the website URL of the vendor that has recalled the battery
     * @since 5.0
     */
    QString recallUrl() const;

    /**
     * The serial number of the battery
     *
     * @return the serial number of the battery
     * @since 5.0
     */
    QString serial() const;

Q_SIGNALS:
    /**
     * This signal is emitted if the battery get plugged in/out of the
     * battery bay.
     *
     * @param newState the new plugging state of the battery, type is boolean
     * @param udi the UDI of the battery with thew new plugging state
     */
    void presentStateChanged(bool newState, const QString &udi);

    /**
     * This signal is emitted when the charge percent value of this
     * battery has changed.
     *
     * @param value the new charge percent value of the battery
     * @param udi the UDI of the battery with the new charge percent
     */
    void chargePercentChanged(int value, const QString &udi);

    /**
     * This signal is emitted when the capacity of this battery has changed.
     *
     * @param value the new capacity of the battery
     * @param udi the UDI of the battery with the new capacity
     * @since 4.11
     */
    void capacityChanged(int value, const QString &udi);

    /**
     * This signal is emitted when the power supply state of the battery
     * changes.
     *
     * @param newState the new power supply state, type is boolean
     * @param udi the UDI of the battery with the new power supply state
     * @since 4.11
     */
    void powerSupplyStateChanged(bool newState, const QString &udi);

    /**
     * This signal is emitted when the charge state of this battery
     * has changed.
     *
     * @param newState the new charge state of the battery, it's one of
     * the type Solid::Battery::ChargeState
     * @see Solid::Battery::ChargeState
     * @param udi the UDI of the battery with the new charge state
     */
    void chargeStateChanged(int newState, const QString &udi = QString());

    /**
     * This signal is emitted when the time until the battery is empty
     * has changed.
     *
     * @param time the new remaining time
     * @param udi the UDI of the battery with the new remaining time
     * @since 5.0
     */
    void timeToEmptyChanged(qlonglong time, const QString &udi);

    /**
     * This signal is emitted when the time until the battery is full
     * has changed.
     *
     * @param time the new remaining time
     * @param udi the UDI of the battery with the new remaining time
     * @since 5.0
     */
    void timeToFullChanged(qlonglong time, const QString &udi);

    /**
     * This signal is emitted when the energy value of this
     * battery has changed.
     *
     * @param energy the new energy value of the battery
     * @param udi the UDI of the battery with the new charge percent
     */
    void energyChanged(double energy, const QString &udi);

    /**
     * This signal is emitted when the energy rate value of this
     * battery has changed.
     *
     * If positive, the source is being discharged, if negative it's being charged.
     *
     * @param energyRate the new energy rate value of the battery
     * @param udi the UDI of the battery with the new charge percent
     */
    void energyRateChanged(double energyRate, const QString &udi);

    /**
     * This signal is emitted when the voltage in the cell has changed.
     *
     * @param voltage the new voltage of the cell
     * @param udi the UDI of the battery with the new voltage
     * @since 5.0
     */
    void voltageChanged(double voltage, const QString &udi);

    /**
     * This signal is emitted when the battery temperature has changed.
     *
     * @param temperature the new temperature of the battery in degrees Celsius
     * @param udi the UDI of the battery with the new temperature
     * @since 5.0
     */
    void temperatureChanged(double temperature, const QString &udi);

};
}

#endif
