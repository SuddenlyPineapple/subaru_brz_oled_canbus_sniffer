#include "canController.h"
#include <CAN.h>

// Connections:
//  MCP | BOARD
//  INT | Not used, can connect to Pin 9
//  SCK | SCK
//   SI | MO
//   SO | MI
//   CS | Pin 7
//  GND | GND
//  VCC | 3.3V

const int CS_PIN = 7;
const int IRQ_PIN = 9;
const int QUARTZ_MHZ = 16;  // Some MCP2515 boards have 8 MHz quartz.
const int SPI_MHZ = 8;

CANBusController::CANBusController() {
    CAN.setClockFrequency(QUARTZ_MHZ * 1E6);
    CAN.setSPIFrequency(SPI_MHZ * 1E6);
    CAN.setPins(CS_PIN, IRQ_PIN);
}

bool CANBusController::connect() {
    // Subaru BRZ uses a 500k baud rate.
    while (!CAN.begin(500000)) {
        Serial.println("Failed to connect to the CAN controller!");
        delay(1000);
    }
    Serial.println("CAN controller connected");
    return true;
}

// returns false if packet is not mapped
bool CANBusController::mapPacketData(long packetID, uint8_t *packetData) {
    switch (packetID) {
        case 0xD1: // Speed & Brakes
            // [0]? const 0
            // [1]? const 0
            // [2] 0 to FF probably break pressure but not just it
            // [3]? const 0
            brakePressure = (int) packetData[2] * 128;
//            Serial.print(" - break pressure is ");
//            Serial.print(brakePressure);
            return true;
        case 0x140: // Clutch, Acceleration, RPM
            clutchDown = packetData[1] == 0x00 ? true : false;
//            Serial.print(" - clutch is ");
//            Serial.print(clutchDown);
            return true;
        case 0x360: // Temperatures, Battery voltage -
            // [0]? some low values
            // [1]? some low values
            // [2]-40=oil temp
            // [3]-40=coolant temp
            // [4]-40=(probably) outdoor temp
            // [5]? constant 0
            // [6] is 145 (probably) it's battery voltage without dot 14.5
            // [7]? constant 0
            oilTemperature = (int) packetData[2] - 40;
            coolantTemperature = (int) packetData[3] - 40;
//            Serial.print(" - oli temp is ");
//            Serial.print(oilTemperature);
//            Serial.print(" - colant temp is ");
//            Serial.print(coolantTemperature);
            return true;
        default:
            return false;
    }

};

int CANBusController::readPackets() {
    // try to parse packet
    int packetSize = CAN.parsePacket();

    if (packetSize) {
        // received a packet
//        Serial.print("Received ");
        long packetID = CAN.packetId();
        Serial.print(packetID, HEX);

        if (CAN.packetRtr()) {
            // Remote transmission request, packet contains no data
//            Serial.print("RTR ");
            Serial.print(",01");
        } else {
            Serial.print(",00");
        }

        if (CAN.packetExtended()) {
//            Serial.print("extended ");
            Serial.print(",01,");
        } else {
            Serial.print(",00,");
        }

//        Serial.print("packet with id 0x");
//        long packetID = CAN.packetId();
//        Serial.print(packetID, HEX);

        if (CAN.packetRtr()) {
//            Serial.print(" and requested length ");
            Serial.println(CAN.packetDlc());
        } else {
//            Serial.print(" and length ");
            Serial.print(packetSize);
//            Serial.print(" - value is: ");

            uint8_t data[8];
            int data_length = 0;
            while (data_length < packetSize && data_length < sizeof(data)) {
                int byte_read = CAN.read();
                Serial.print((char)byte_read, HEX);
                Serial.print(" ");
                if (byte_read == -1) {
                    break;
                }
                data[data_length++] = byte_read;
            }
            mapPacketData(packetID, data);

//            // only print packet data for non-RTR packets
//            while (CAN.available()) {
//                Serial.print((char)CAN.read(), HEX);
//            }
        }
        Serial.println();
    }
    return 1;
}
