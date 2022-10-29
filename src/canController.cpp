//
// Created by Wojciech Kasperski on 10/29/2022.
//

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

//int CANBusController::readPackets() {
//    // try to parse packet
//    int packetSize = CAN.parsePacket();
//
//    if (packetSize) {
//        // received a packet
//        Serial.print("Received ");
//
//        if (CAN.packetExtended()) {
//            Serial.print("extended ");
//        }
//
//        if (CAN.packetRtr()) {
//            // Remote transmission request, packet contains no data
//            Serial.print("RTR ");
//        }
//
//        Serial.print("packet with id 0x");
//        Serial.print(CAN.packetId(), HEX);
//
//        if (CAN.packetRtr()) {
//            Serial.print(" and requested length ");
//            Serial.println(CAN.packetDlc());
//        } else {
//            Serial.print(" and length ");
//            Serial.println(packetSize);
//
//            // only print packet data for non-RTR packets
//            while (CAN.available()) {
//                Serial.print((char)CAN.read());
//            }
//            Serial.println();
//        }
//        Serial.println();
//    }
//    return 1;
//}
