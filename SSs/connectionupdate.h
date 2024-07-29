#ifndef CONNECTIONUPDATE_H
#define CONNECTIONUPDATE_H

#include <QObject>
#include <QString>
#include <QMainWindow>

class ConnectionUpdate : public QObject
{
    Q_OBJECT

public:
    ConnectionUpdate(QMainWindow *mainWindow, QObject *parent = nullptr);

    QString local_ip;
    QString remote_ip;

    unsigned short local_port;
    unsigned short remote_port;

public slots:
    void updateConnectionData();

private:
    QMainWindow *mainWindow;
};

#endif // CONNECTIONUPDATE_H
