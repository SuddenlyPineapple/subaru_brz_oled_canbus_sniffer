#ifndef SUBARU_BRZ_OLED_CANBUS_SNIFFER_CANCONTROLLER_H
#define SUBARU_BRZ_OLED_CANBUS_SNIFFER_CANCONTROLLER_H

#include <Arduino.h>
#include <CAN.h>

class CANBusController {
    public:
        int brakePressure = 0;
        int oilTemperature = 0;
        int coolantTemperature = 0;
        bool clutchDown = false;

        CANBusController();
        bool connect();
        int readPackets();

    private:
        bool mapPacketData(long packetID,uint8_t* packetData);
};


#endif //SUBARU_BRZ_OLED_CANBUS_SNIFFER_CANCONTROLLER_H
