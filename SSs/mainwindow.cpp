#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "labelupdate.h"
#include "sensordatareceiver.h"
#include "transformvalues.h"
#include "mahonyfilter.h"
#include "quaternion.h"
#include "eulerangles.h"
#include "inertiaestimator.h"

#include "qcustomplot.h"

//#include <QTimer>
#include <unistd.h>

#include <QVector>
#include <QThreadPool>
#include <QRunnable>
#include <QMetaObject>

class UpdateDataWidgetsTask : public QRunnable {
public:
    UpdateDataWidgetsTask(MainWindow* mainWindow) : mainWindow(mainWindow) {}

    void run() override {
        mainWindow->updateDataWidgetsSlot();
    }

private:
    MainWindow* mainWindow;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui__(new Ui::MainWindow), startButtonFlag__(false),
      local_ip__("192.168.0.106"), remote_ip__("192.168.0.102"), local_port__(50052), remote_port__(50051)
{
    ui__->setupUi(this);

    // for graphics
    magnetic_field_x_proj__.append(0);
    magnetic_field_y_proj__.append(0);
    magnetic_field_z_proj__.append(0);

    acc_x_proj__.append(0);
    acc_y_proj__.append(0);
    acc_z_proj__.append(0);

    angular_velocity_x_proj__.append(0);
    angular_velocity_y_proj__.append(0);
    angular_velocity_z_proj__.append(0);

    angular_velocity_dot_x_proj__.append(0);
    angular_velocity_dot_y_proj__.append(0);
    angular_velocity_dot_z_proj__.append(0);

    t_proj__.append(0);

    // events
    connect(ui__->checkBox, &QCheckBox::stateChanged, ui__->openGLWidget, &GLWidget::DisplayModelSlot);
    connect(ui__->checkBox_2, &QCheckBox::stateChanged, ui__->openGLWidget, &GLWidget::DisplayAxesSlot);
    connect(ui__->checkBox_3, &QCheckBox::stateChanged, ui__->openGLWidget, &GLWidget::DisplayBasisSlot);

    connect(ui__->pushButton, &QPushButton::clicked, this, &MainWindow::acceptButtonSlot);

    UpdateDataWidgetsTask* task = new UpdateDataWidgetsTask(this);
    task->setAutoDelete(true);

    connect(ui__->pushButton_2, &QPushButton::clicked, this, [this, task]() {
        QThreadPool::globalInstance()->start(task);
    });

    connect(ui__->pushButton_3, &QPushButton::clicked, this, &MainWindow::stopUpdateWidgetSlot);

    // Magnetic field
    ui__->graph_widget->xAxis->setLabel("Time (sec)");
    ui__->graph_widget->yAxis->setLabel("Value (optionally)");
    ui__->graph_widget->legend->setVisible(true);
    ui__->graph_widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);


    ui__->graph_widget->addGraph();
    QCPGraph* graph = ui__->graph_widget->graph(0);
    graph->setPen(QPen(Qt::blue)); // line color
    graph->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui__->graph_widget->graph(0)->setName("x");
    ui__->graph_widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));

    ui__->graph_widget->addGraph();
    QCPGraph* graph2 = ui__->graph_widget->graph(1);
    graph2->setBrush(QBrush(QColor(255, 0, 0, 20)));
    graph2->setPen(QPen(Qt::red));
    ui__->graph_widget->graph(1)->setName("y");
    ui__->graph_widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 7));

    ui__->graph_widget->addGraph();
    QCPGraph* graph3 = ui__->graph_widget->graph(2);
    graph3->setBrush(QBrush(QColor(0, 255, 0, 20)));
    graph3->setPen(QPen(Qt::green));
    ui__->graph_widget->graph(2)->setName("z");
    ui__->graph_widget->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 7));

    // Angular velocity
    ui__->graph_widget_2->xAxis->setLabel("Time (sec)");
    ui__->graph_widget_2->yAxis->setLabel("Value (optionally)");
    ui__->graph_widget_2->legend->setVisible(true);
    ui__->graph_widget_2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    ui__->graph_widget_2->addGraph();
    QCPGraph* graph4 = ui__->graph_widget_2->graph(0);
    graph4->setPen(QPen(Qt::blue));
    graph4->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui__->graph_widget_2->graph(0)->setName("x");
    ui__->graph_widget_2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangle, 7));

    ui__->graph_widget_2->addGraph();
    QCPGraph* graph5 = ui__->graph_widget_2->graph(1);
    graph5->setBrush(QBrush(QColor(255, 0, 0, 20)));
    graph5->setPen(QPen(Qt::red));
    ui__->graph_widget_2->graph(1)->setName("y");
    ui__->graph_widget_2->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));

    ui__->graph_widget_2->addGraph();
    QCPGraph* graph6 = ui__->graph_widget_2->graph(2);
    graph6->setBrush(QBrush(QColor(0, 255, 0, 20)));
    graph6->setPen(QPen(Qt::green));
    ui__->graph_widget_2->graph(2)->setName("z");
    ui__->graph_widget_2->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPeace, 7));

    // Acceleration
    ui__->graph_widget_3->xAxis->setLabel("Time (sec)");
    ui__->graph_widget_3->yAxis->setLabel("Value (optionally)");
    ui__->graph_widget_3->legend->setVisible(true);
    ui__->graph_widget_3->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    ui__->graph_widget_3->addGraph();
    QCPGraph* graph7 = ui__->graph_widget_3->graph(0);
    graph7->setPen(QPen(Qt::blue));
    graph7->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui__->graph_widget_3->graph(0)->setName("x");
    ui__->graph_widget_3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangleInverted, 7));

    ui__->graph_widget_3->addGraph();
    QCPGraph* graph8 = ui__->graph_widget_3->graph(1);
    graph8->setBrush(QBrush(QColor(255, 0, 0, 20)));
    graph8->setPen(QPen(Qt::red));
    ui__->graph_widget_3->graph(1)->setName("y");
    ui__->graph_widget_3->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossSquare, 7));

    ui__->graph_widget_3->addGraph();
    QCPGraph* graph9 = ui__->graph_widget_3->graph(2);
    graph9->setBrush(QBrush(QColor(0, 255, 0, 20)));
    graph9->setPen(QPen(Qt::green));
    ui__->graph_widget_3->graph(2)->setName("z");
    ui__->graph_widget_3->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle, 7));
}

MainWindow::~MainWindow()
{
    delete ui__;
}

void MainWindow::updateDataWidgetsSlot()
{
    QVector<QLabel*> euler_labels = {ui__->label_30, ui__->label_31, ui__->label_32};
    QVector<QLabel*> quatrenion_labels = {ui__->label_23, ui__->label_24, ui__->label_25, ui__->label_26};

    QVector<QLabel*> omega_dot_labels = {ui__->label_27, ui__->label_28, ui__->label_29};
    QVector<QLabel*> omega_dot_norm_labels = {ui__->label_54};

    QVector<QLabel*> mag_field_labels = {ui__->label_33, ui__->label_34, ui__->label_35};
    QVector<QLabel*> mag_field_norm_label = {ui__->label_55};

    QVector<QLabel*> ang_vel_labels = {ui__->label_36, ui__->label_37, ui__->label_38};
    QVector<QLabel*> ang_vel_norm_label = {ui__->label_59};

    QVector<QLabel*> acc_labels = {ui__->label_39, ui__->label_40, ui__->label_41};
    QVector<QLabel*> acc_norm_label = {ui__->label_57};

    LabelUpdate* update_object_mag = new LabelUpdate(mag_field_labels);
    LabelUpdate* update_object_mag_norm = new LabelUpdate(mag_field_norm_label);

    LabelUpdate* update_object_ang = new LabelUpdate(ang_vel_labels);
    LabelUpdate* update_object_ang_norm = new LabelUpdate(ang_vel_norm_label);

    LabelUpdate* update_object_acc = new LabelUpdate(acc_labels);
    LabelUpdate* update_object_acc_norm = new LabelUpdate(acc_norm_label);

    LabelUpdate* update_object_quat = new LabelUpdate(quatrenion_labels);

    LabelUpdate* update_object_euler = new LabelUpdate(euler_labels);

    LabelUpdate* update_object_omega_dot = new LabelUpdate(omega_dot_labels);
    LabelUpdate* update_object_omega_dot_norm = new LabelUpdate(omega_dot_norm_labels);

    QVector<QString> angular_velocity_norm_str;
    QVector<QString> acceleration_norm_str;
    QVector<QString> magnetic_field_norm_str;
    QVector<QString> omega_dot_norm_str;

    startButtonFlag__ = true;

    int value_for_time_line = 0;
    const int moving_window_size = 25;

    // Mahony filter
    int frequency = ui__->spinBox_5->value();
    double kp = ui__->doubleSpinBox_2->value();
    double ki = ui__->doubleSpinBox_3->value();

    MahonyFilter* filter = new MahonyFilter(frequency, kp, ki);

    // Omega dot data
    QVector<float> prev_angular_velocity = {0.0, 0.0, 0.0};
    QVector<float> dot_angular_velocity = {0.0, 0.0, 0.0};

    //Inertial estimator
    InertiaEstimator* estimator = new InertiaEstimator();

    // Data update loop
    while (startButtonFlag__) {
        SensorDataReceiver* udp_listener = new SensorDataReceiver(
                    local_ip__.toStdString(),
                    local_port__,
                    remote_ip__.toStdString(),
                    remote_port__);

        int round_value = ui__->spinBox_3->value();
        double timeout_state = ui__->doubleSpinBox->value();
        int num_of_values = ui__->spinBox_4->value();

        QVector<float> magnetic_field = QVector<float>::fromStdVector(udp_listener->run(udp_listener->sens_hmc, num_of_values));
        QVector<float> temporary_mag_field = magnetic_field.mid(magnetic_field.size() - 3);

        TransformValues::roundVector(temporary_mag_field, round_value);
        TransformValues::NTToT(temporary_mag_field, ui__->radioButton_6->isChecked());
        TransformValues::TToNT(temporary_mag_field, ui__->radioButton_5->isChecked());

        QVector<QString> magnetic_field_str = TransformValues::convertFloatArrayToStringArray(temporary_mag_field);

        QVector<float> acceleration = QVector<float>::fromStdVector(udp_listener->run(udp_listener->sens_ism_xl, num_of_values));
        QVector<float> temporary_acc = acceleration.mid(acceleration.size() - 3);

        TransformValues::roundVector(temporary_acc, round_value);
        TransformValues::Ms2ToG(temporary_acc, ui__->radioButton_8->isChecked());
        TransformValues::GToMS2(temporary_acc, ui__->radioButton_7->isChecked());

        QVector<QString> acceleration_str = TransformValues::convertFloatArrayToStringArray(temporary_acc);

        QVector<float> angular_velocity = QVector<float>::fromStdVector(udp_listener->run(udp_listener->sens_ism_gy, num_of_values));
        QVector<float> temporary_angular_velocity = angular_velocity.mid(angular_velocity.size() - 3);

        TransformValues::roundVector(temporary_angular_velocity, round_value);
        TransformValues::DegToRad(temporary_angular_velocity, ui__->radioButton_4->isChecked());
        TransformValues::RadToDeg(temporary_angular_velocity, ui__->radioButton_3->isChecked());

        for (int i = 0; i < temporary_angular_velocity.size(); ++i) {
            dot_angular_velocity[i] = (temporary_angular_velocity[i] - prev_angular_velocity[i]) / timeout_state;
            prev_angular_velocity[i] = temporary_angular_velocity[i];
        }
        TransformValues::roundVector(dot_angular_velocity, round_value);

        QVector<QString> angular_velocity_str = TransformValues::convertFloatArrayToStringArray(temporary_angular_velocity);
        QVector<QString> omega_dot_str = TransformValues::convertFloatArrayToStringArray(dot_angular_velocity);

        angular_velocity_norm_str.append(QString::number(TransformValues::norm(temporary_angular_velocity)));
        acceleration_norm_str.append(QString::number(TransformValues::norm(temporary_acc)));
        magnetic_field_norm_str.append(QString::number(TransformValues::norm(temporary_mag_field)));
        omega_dot_norm_str.append(QString::number(TransformValues::norm(dot_angular_velocity)));

        // Update text's widgets
        update_object_acc->update(acceleration_str);
        update_object_acc_norm->update(acceleration_norm_str);

        update_object_ang->update(angular_velocity_str);
        update_object_ang_norm->update(angular_velocity_norm_str);

        update_object_mag->update(magnetic_field_str);
        update_object_mag_norm->update(magnetic_field_norm_str);

        update_object_omega_dot->update(omega_dot_str);
        update_object_omega_dot_norm->update(omega_dot_norm_str);

        if (!angular_velocity_norm_str.isEmpty()) {
            angular_velocity_norm_str.removeLast();
        }

        if (!acceleration_norm_str.isEmpty()) {
            acceleration_norm_str.removeLast();
        }

        if (!magnetic_field_norm_str.isEmpty()) {
            magnetic_field_norm_str.removeLast();
        }
        if (!omega_dot_norm_str.isEmpty()) {
            omega_dot_norm_str.removeLast();
        }

        // Graphics update
        value_for_time_line += timeout_state;
        t_proj__.append(value_for_time_line);

        magnetic_field_x_proj__.append(temporary_mag_field[0]);
        magnetic_field_y_proj__.append(temporary_mag_field[1]);
        magnetic_field_z_proj__.append(temporary_mag_field[2]);

        acc_x_proj__.append(temporary_acc[0]);
        acc_y_proj__.append(temporary_acc[1]);
        acc_z_proj__.append(temporary_acc[2]);

        angular_velocity_x_proj__.append(temporary_angular_velocity[0]);
        angular_velocity_y_proj__.append(temporary_angular_velocity[1]);
        angular_velocity_z_proj__.append(temporary_angular_velocity[2]);

        if (t_proj__.size() > moving_window_size) {
            t_proj__.remove(0, t_proj__.size() - moving_window_size);

            magnetic_field_x_proj__.remove(0, magnetic_field_x_proj__.size() - moving_window_size);
            magnetic_field_y_proj__.remove(0, magnetic_field_y_proj__.size() - moving_window_size);
            magnetic_field_z_proj__.remove(0, magnetic_field_z_proj__.size() - moving_window_size);

            acc_x_proj__.remove(0, acc_x_proj__.size() - moving_window_size);
            acc_y_proj__.remove(0, acc_y_proj__.size() - moving_window_size);
            acc_z_proj__.remove(0, acc_z_proj__.size() - moving_window_size);

            angular_velocity_x_proj__.remove(0, angular_velocity_x_proj__.size() - moving_window_size);
            angular_velocity_y_proj__.remove(0, angular_velocity_y_proj__.size() - moving_window_size);
            angular_velocity_z_proj__.remove(0, angular_velocity_z_proj__.size() - moving_window_size);
        }

        QMetaObject::invokeMethod(this, [this]() {
            ui__->graph_widget->graph(0)->setData(t_proj__, magnetic_field_x_proj__);
            ui__->graph_widget->graph(1)->setData(t_proj__, magnetic_field_y_proj__);
            ui__->graph_widget->graph(2)->setData(t_proj__, magnetic_field_z_proj__);
            ui__->graph_widget->rescaleAxes(true);
            ui__->graph_widget->replot();

            ui__->graph_widget_2->graph(0)->setData(t_proj__, acc_x_proj__);
            ui__->graph_widget_2->graph(1)->setData(t_proj__, acc_y_proj__);
            ui__->graph_widget_2->graph(2)->setData(t_proj__, acc_z_proj__);
            ui__->graph_widget_2->rescaleAxes();
            ui__->graph_widget_2->replot();

            ui__->graph_widget_3->graph(0)->setData(t_proj__, angular_velocity_x_proj__);
            ui__->graph_widget_3->graph(1)->setData(t_proj__, angular_velocity_y_proj__);
            ui__->graph_widget_3->graph(2)->setData(t_proj__, angular_velocity_z_proj__);
            ui__->graph_widget_3->rescaleAxes();
            ui__->graph_widget_3->replot();
        });

        // Mahony filter
        frequency = ui__->spinBox_5->value();
        kp = ui__->doubleSpinBox_2->value();
        ki = ui__->doubleSpinBox_3->value();

        filter->setParam(frequency, kp, ki);

        if (acceleration.size() % 3 != 0 || angular_velocity.size() % 3 != 0 || magnetic_field.size() % 3 != 0) {
            std::cerr << "Vector sizes are not multiples of 3" << std::endl;
            return;
        }

        if(ui__->radioButton_13->isChecked()) {
            // Data format: rad/sec, m/sec^2
            for (int i = 0; i < acceleration.size(); i += 3) {
                QVector<float> current_acceleration = acceleration.mid(i, 3);
                QVector<float> current_angular_velocity = angular_velocity.mid(i, 3);
                filter->updateIMU(current_angular_velocity, current_acceleration);
            }
        }
        else if(ui__->radioButton_14->isChecked()) {
            // Data format: rad/sec, m/sec^2, mT~G
            for (int i = 0; i < acceleration.size(); i += 3) {
                QVector<float> current_acceleration = acceleration.mid(i, 3);
                QVector<float> current_angular_velocity = angular_velocity.mid(i, 3);
                QVector<float> current_magnetic_field = magnetic_field.mid(i, 3);

                filter->update(current_angular_velocity, current_acceleration, current_magnetic_field);
            }
        }

        Quaternion current_quaternion = filter->getQuaternion();
        ui__->openGLWidget->updateQuaternion(current_quaternion);

        QVector<float> quaternion_values;

        quaternion_values.append(static_cast<float>(current_quaternion.w));
        quaternion_values.append(static_cast<float>(current_quaternion.x));
        quaternion_values.append(static_cast<float>(current_quaternion.y));
        quaternion_values.append(static_cast<float>(current_quaternion.z));
        TransformValues::roundVector(quaternion_values, round_value);
        QVector<QString> current_quternion_str = TransformValues::convertFloatArrayToStringArray(quaternion_values);

        update_object_quat->update(current_quternion_str);

        EulerAngles current_euler_angles = current_quaternion.toEulerAngles();
        QVector<float> euler_angles_values;

        euler_angles_values.append(static_cast<float>(current_euler_angles.roll));
        euler_angles_values.append(static_cast<float>(current_euler_angles.pitch));
        euler_angles_values.append(static_cast<float>(current_euler_angles.yaw));

        TransformValues::DegToRad(euler_angles_values, ui__->radioButton->isChecked());
        TransformValues::RadToDeg(euler_angles_values, ui__->radioButton_2->isChecked());
        TransformValues::roundVector(euler_angles_values, round_value);

        QVector<QString> current_euler_str= TransformValues::convertFloatArrayToStringArray(euler_angles_values);

        update_object_euler->update(current_euler_str);

        for(int i = 0; i < acceleration.size(); i += 3) {
            QVector<float> current_acceleration = acceleration.mid(i, 3);
            QVector<float> current_angular_velocity = angular_velocity.mid(i, 3);
            estimator->update(current_angular_velocity, current_acceleration, current_quaternion);
        }

        Eigen::Matrix3f inertia_tensor = estimator->getInertiaTensor();

        std::cout << "Inertia Tensor:\n" << inertia_tensor << std::endl;

        // Timeout
        //QTimer::singleShot(static_cast<int>(timeout_state * 10000), [](){});
        usleep(int(timeout_state * 1000000));
    }
}

void MainWindow::stopUpdateWidgetSlot()
{
    startButtonFlag__ = false;
}

void MainWindow::acceptButtonSlot()
{
    local_ip__ = ui__->lineEdit->text();
    remote_ip__ = ui__->lineEdit_1->text();
    local_port__ = ui__->spinBox->value();
    remote_port__ = ui__->spinBox_2->value();
}
