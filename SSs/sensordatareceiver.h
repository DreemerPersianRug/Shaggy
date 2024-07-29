#ifndef SENSORDATARECEIVER_H
#define SENSORDATARECEIVER_H

#include <string>
#include <vector>

#include <QObject>

class SensorDataReceiver
{
public:
    SensorDataReceiver(const std::string& local_ip, unsigned short local_port, const std::string& remote_ip, unsigned short remote_port);
    std::vector<float> run(int sensor, int n);

    int sens_hmc;
    int sens_mmc;
    int sens_bmi;
    int sens_ism_gy;
    int sens_ism_xl;

private:
    std::vector<float> processData(const std::vector<float>& data, int n);

    std::string local_ip__;
    unsigned short local_port__;
    std::string remote_ip__;
    unsigned short remote_port__;

    int udp_preambule__;
    int sens_cmd_preambule__;
    int sens_cmd_get_raw__;

    int udp_pack_cnt__;
    bool udp_print__;
};

#endif // SENSORDATARECEIVER_H
