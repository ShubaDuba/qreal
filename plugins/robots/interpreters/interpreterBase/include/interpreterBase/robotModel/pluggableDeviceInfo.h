#pragma once

#include <QtCore/QString>
#include <QtCore/QMetaObject>

namespace interpreterBase {
namespace robotModel {

/// Describes a type of a pluggable device, not a concrete instance of it.
/// Given a set of PluggableDeviceInfo corresponding to some pluggable devices
/// hierarchy original inheritance relations can be recovered with isA() method.
class PluggableDeviceInfo
{
public:
	/// Creates a new instance of a PluggableDevice descriptor. The resulting object will
	/// correspond to a given type only if Q_OBJECT macro is used inside its declaration.
	template<typename T>
	static PluggableDeviceInfo create(QString const &friendlyName = QString())
	{
		// This line performs Q_OBJECT macro checking in the given type declaration.
		// Without Q_OBJECT macro incorrect metaObject will be passed and it will lead
		// to invalid isA() method work.
		reinterpret_cast<T *>(0)->qt_check_for_QOBJECT_macro(*reinterpret_cast<T *>(0));
		return PluggableDeviceInfo(&T::staticMetaObject, friendlyName);
	}

	/// Returns if the device corresponding to 'this' inherits a 'parent' one or they are the devices of the same type.
	bool isA(PluggableDeviceInfo const &parent) const;

	/// Returns a string that can be displayed to a user as the name of the pluggable device.
	QString friendlyName() const;

private:
	friend bool operator ==(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2);
	friend bool operator !=(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2);

	explicit PluggableDeviceInfo(QMetaObject const *deviceType, QString const &friendlyName);

	QMetaObject const *mDeviceType;
	QString mFriendlyName;
};

inline bool operator ==(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2)
{
	return QString(device1.mDeviceType->className()) == QString(device2.mDeviceType->className());
}

inline bool operator !=(PluggableDeviceInfo const &device1, PluggableDeviceInfo const &device2)
{
	return !(device1 == device2);
}

}
}
