/*
    Copyright 2014 Alejandro Fiestas Olivares <afiestas@kde.org>

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

#include "fakelogind.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <QDebug>
#include <qtimer.h>

FakeLogind::FakeLogind(QObject* parent) : QObject(parent)
{

}

QDBusUnixFileDescriptor FakeLogind::Inhibit(const QString& what, const QString& who, const QString& why, const QString& mode)
{
    Q_EMIT newInhibition(what, who, why, mode);

//     m_fd = open("/tmp/rolf", O_WRONLY|O_CLOEXEC|O_NDELAY);
//     qDebug() << "Sending" << m_fd;
    QDBusUnixFileDescriptor foo;
    foo.giveFileDescriptor(33);
/*
    auto t = new QTimer();
    t->setInterval(100);
    connect(t, SIGNAL(timeout()), SLOT(checkFd()));
    t->start();*/
    return foo;
}


void FakeLogind::checkFd()
{
    int e = fcntl(m_fd, F_GETFD) != -1 || errno != EBADF;

    if (e == 0) {
        delete sender();
    }
}