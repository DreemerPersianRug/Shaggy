#include "connectionupdate.h"
#include <QMainWindow>

ConnectionUpdate::ConnectionUpdate(QMainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow), local_ip("192.168.41.119"), remote_ip("192.168.41.100"), local_port(50006), remote_port(50005)
{
}

void ConnectionUpdate::updateConnectionData()
{
    local_ip = mainWindow->lineEdit->text();
}
