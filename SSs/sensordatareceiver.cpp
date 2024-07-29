#include "sensordatareceiver.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>


SensorDataReceiver::SensorDataReceiver(const std::string& local_ip, unsigned short local_port, const std::string& remote_ip, unsigned short remote_port)
    : local_ip__(local_ip), local_port__(local_port), remote_ip__(remote_ip), remote_port__(remote_port),
      udp_preambule__(0x04030202), sens_cmd_preambule__(0x1), sens_cmd_get_raw__(0x2), udp_pack_cnt__(0x0f), udp_print__(false)
{
    sens_hmc = 0;
    sens_mmc = 1;
    sens_bmi = 2;
    sens_ism_gy = 3;
    sens_ism_xl = 4;
}

std::vector<float> SensorDataReceiver::run(int sensor, int n)
{
    int sockfd_out = socket(AF_INET, SOCK_DGRAM, 0);
    int sockfd_in = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd_out < 0 || sockfd_in < 0) {
        perror("socket creation failed");
        std::exit(1);
    }

    struct sockaddr_in local_addr, remote_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    memset(&remote_addr, 0, sizeof(remote_addr));

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(local_ip__.c_str());
    local_addr.sin_port = htons(local_port__);

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr(remote_ip__.c_str());
    remote_addr.sin_port = htons(remote_port__);

    if (bind(sockfd_in, (const struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind failed");
        close(sockfd_out);
        close(sockfd_in);
        std::exit(1);
    }

    int flags = fcntl(sockfd_in, F_GETFL, 0);
    fcntl(sockfd_in, F_SETFL, flags | O_NONBLOCK);

    std::vector<int> buff = {udp_preambule__, udp_pack_cnt__, sens_cmd_preambule__, sens_cmd_get_raw__, sensor, n};
    ssize_t sent_bytes = sendto(sockfd_out, buff.data(), buff.size() * sizeof(int), 0, (const struct sockaddr *)&remote_addr, sizeof(remote_addr));

    if (sent_bytes < 0) {
        perror("sendto failed");
        close(sockfd_out);
        close(sockfd_in);
        std::exit(1);
    }

    if (udp_print__) {
        std::cout << "pack_cnt = " << udp_pack_cnt__ << std::endl;
    }

    udp_pack_cnt__ += 1;

    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);
    std::vector<float> recv_buffer(1024);

    std::vector<float> data;

    while (true) {
        ssize_t len = recvfrom(sockfd_in, recv_buffer.data(), recv_buffer.size() * sizeof(float), 0, (struct sockaddr *)&sender_addr, &addr_len);

        if (len > 0) {
            data = processData(recv_buffer, n);
            break;
        } else if (len == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(100000);
                continue;
            } else {
                perror("recvfrom failed");
                break;
            }
        }
    }

    close(sockfd_out);
    close(sockfd_in);

    return data;
}

std::vector<float> SensorDataReceiver::processData(const std::vector<float> &data, int n)
{
    if (data.size() < 4) {
        throw std::invalid_argument("Received data is too short");
    }

    int pack_ack_cnt = data[1];
    int pack_ack_result = data[2];
    int pack_ack_length = data[3];

    if (udp_print__) {
        std::cout << "pack_ack_cnt = " << pack_ack_cnt << std::endl;
        std::cout << "pack_ack_length = " << pack_ack_length << std::endl;
        std::cout << "pack_ack_result = " << pack_ack_result << std::endl;
    }

    std::vector<float> data_out(data.begin() + 4, data.begin() + 4 + (n * 3));

    if(udp_print__){
        for (float value : data_out) {
            std::cout << value << " " << std::endl;
        }
        std::cout << std::endl;
    }
    return data_out;
}
