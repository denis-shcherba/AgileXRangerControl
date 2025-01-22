#include <iostream>
#include <cstring>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ioctl.h>  // Include for ioctl and SIOCGIFINDEX

int main() {
    // Set up the CAN interface
    const char* can_interface = "can0";  // Change if your interface is different
    int socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return 1;
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, can_interface, IFNAMSIZ - 1);
    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        std::cerr << "Error getting interface index: " << strerror(errno) << std::endl;
        close(socket_fd);
        return 1;
    }

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
        close(socket_fd);
        return 1;
    }

    try {
        // Step 1: Send control mode message
        uint32_t id_control_mode = 0x421; 
        struct can_frame frame_control_mode;
        frame_control_mode.can_id = id_control_mode;
        frame_control_mode.can_dlc = 8; // Data length code (number of bytes)
        frame_control_mode.data[0] = 0x01; // Byte 0 for control mode
        std::memset(frame_control_mode.data + 1, 0, 7); // Remaining bytes = 0

        // Send the control mode message
        if (write(socket_fd, &frame_control_mode, sizeof(frame_control_mode)) != sizeof(frame_control_mode)) {
            std::cerr << "Error sending control mode message: " << strerror(errno) << std::endl;
            close(socket_fd);
            return 1;
        }
        std::cout << "Control mode message sent: ID = " << std::hex << id_control_mode << std::endl;

        // Step 2: Define motion control command
        uint32_t id_motion = 0x111; 
        int16_t linear_speed = -100; // Linear speed in mm/s (1 cm/s)
        int16_t spin_speed = 0;       // Spin speed in 0.001 rad/s
        uint8_t reserved = 0;         // Reserved byte

        // Create the data payload
        struct can_frame frame_motion;
        frame_motion.can_id = id_motion;
        frame_motion.can_dlc = 8; // Data length code (number of bytes)

        // Convert linear speed and spin speed to bytes
        frame_motion.data[0] = (linear_speed >> 8) & 0xFF; // High byte of linear speed
        frame_motion.data[1] = linear_speed & 0xFF;        // Low byte of linear speed
        frame_motion.data[2] = (spin_speed >> 8) & 0xFF;   // High byte of spin speed
        frame_motion.data[3] = spin_speed & 0xFF;          // Low byte of spin speed
        frame_motion.data[4] = reserved;                    // Reserved byte
        std::memset(frame_motion.data + 5, 0, 3);          // Remaining reserved bytes

        // Send the motion command message
        if (write(socket_fd, &frame_motion, sizeof(frame_motion)) != sizeof(frame_motion)) {
            std::cerr << "Error sending motion command message: " << strerror(errno) << std::endl;
            close(socket_fd);
            return 1;
        }
        std::cout << "Motion command message sent: ID = " << std::hex << id_motion << std::endl;

        // (Optional) Receive CAN messages (implement if needed)
        // while (true) {
        //     struct can_frame recv_frame;
        //     int nbytes = read(socket_fd, &recv_frame, sizeof(recv_frame));
        //     if (nbytes < 0) {
        //         std::cerr << "Error reading message: " << strerror(errno) << std::endl;
        //         break; // Exit on error
        //     }

        //     if (nbytes < sizeof(struct can_frame)) {
        //         std::cerr << "Incomplete CAN frame received" << std::endl;
        //         continue;
        //     }

        //     // Process the received message
        //     std::cout << "Received message: ID = " << std::hex << recv_frame.can_id
        //               << ", Data[0] = " << std::hex << static_cast<int>(recv_frame.data[0]) << std::endl;
        // }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    // Cleanup
    close(socket_fd);
    return 0;
}
