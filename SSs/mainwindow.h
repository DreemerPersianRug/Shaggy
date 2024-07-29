#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateDataWidgetsSlot();
    void stopUpdateWidgetSlot();

    void acceptButtonSlot();

private:
    Ui::MainWindow *ui__;
    bool startButtonFlag__;

    QString local_ip__;
    QString remote_ip__;
    unsigned short local_port__;
    unsigned short remote_port__;

    QVector<double> magnetic_field_x_proj__;
    QVector<double> magnetic_field_y_proj__;
    QVector<double> magnetic_field_z_proj__;

    QVector<double> acc_x_proj__;
    QVector<double> acc_y_proj__;
    QVector<double> acc_z_proj__;

    QVector<double> angular_velocity_dot_x_proj__;
    QVector<double> angular_velocity_dot_y_proj__;
    QVector<double> angular_velocity_dot_z_proj__;

    QVector<double> angular_velocity_x_proj__;
    QVector<double> angular_velocity_y_proj__;
    QVector<double> angular_velocity_z_proj__;

    QVector<double> t_proj__;
};
#endif // MAINWINDOW_H
