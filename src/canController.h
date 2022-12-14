#ifndef SUBARU_BRZ_OLED_CANBUS_SNIFFER_CANCONTROLLER_H
#define SUBARU_BRZ_OLED_CANBUS_SNIFFER_CANCONTROLLER_H

#include <Arduino.h>
#include <CAN.h>

class CANBusController {
    public:
        int brakePressure = 0;
        CANBusController();
        bool connect();
        int readPackets();
};


#endif //SUBARU_BRZ_OLED_CANBUS_SNIFFER_CANCONTROLLER_H
