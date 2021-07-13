#include <avr_can.h>
#include <avr/sleep.h>
#include <avr/power.h>

/*
 * Application: Alfa Romeo Giulietta OBD Dash Speedo
 * 
 * 
 * Description:
 *  Attaches to OBD-2 port and overwrites radio station name with speed in MPH at 2Hz rate.
 *  Requires 'Radio Repetition' to be ON in dashboard settings for this to work correctly.
 *  Dashboard update speed is slow enough that the time to overwrite station name isn't 
 *  noticable and doesn't cause flickering on display.
 * 
 * Requirements:
 *  - Car
 *  - OBD-II Diagnostics port (B-CAN network is used - pins 1/9)
 *  - Pinout for OBD-II: https://www.alfaobd.com/AlfaOBD_Android_Help.pdf
 * 
 *  
 * Credit:
 *  MCP2515 Library: https://github.com/autowp/arduino-mcp2515
 * 
 * Author: James Fletcher
 * Date: 25/04/2021
 */

#define SPEED_DIVIDER_MPH 23 // Found by trail/error and data-logging.

// Found by CAN bus sniffing
#define CAN_ID_RADIO_STATION 0x0A194005
#define CAN_ID_VEHICLE_SPEED 0x04394000

CAN_FRAME frame;

unsigned long lastPrint = 0;

uint16_t currentSpeed = 0;

bool halfSecond = false;

void setup() {
  // Disable unused peripherals to save power
  ADCSRA = 0;
  power_adc_disable();
  power_spi_disable();
  power_lin_disable();
  power_psc_disable();
  
  // Setup CAN stuff
  Can0.begin(CAN_BPS_50K);
  // 1 box for sending
  Can0.setNumTXBoxes(1);
  // 2 boxes for receiving with no filtering
  int filter;
  for (filter = 0; filter < 2; filter++) {
    Can0.setRXFilter(filter, 0, 0, true);
  }
  // consume remaining boxes.
  while (Can0.setRXFilter(0, 0, false) > 0);

  lastPrint = millis();
}

void send_speed_frame() {
 // Construct our own radio station name CAN frame
 CAN_FRAME frame;
 memset(&frame, 0, sizeof(frame));

 // Currently only supports 2 characters for number.
 uint16_t speed = currentSpeed;
 if (speed > 99) speed = 99;

 char tens = speed / 10;
 char digits = speed % 10;
 // char map has '0' char as value 1
 tens += 1;
 digits += 1;

 // Encoded as 'XX MPH' instead of radio message like 'ROCK FM'
 frame.id = CAN_ID_RADIO_STATION;
 frame.extended = true;
 frame.length = 8;
 // Put speed msg into the frame (0-9 only needs lower 4 bits of char map)
 frame.data.bytes[0] = (tens & 0xF) << 2;
 frame.data.bytes[1] = ( (digits & 0xF) << 4 ) | 0xA;
 // 'MPH' character encoding...
 frame.data.bytes[2] = 0x18;
 frame.data.bytes[3] = 0x6D;
 frame.data.bytes[4] = 0x30;
 frame.data.bytes[5] = 0;
 
 Can0.sendFrame(frame);
}

void loop() {
  CAN_FRAME incoming;
  
  if (Can0.rx_avail()) {
    
    if (Can0.read(incoming)) {
      if (incoming.id == CAN_ID_RADIO_STATION) {
        // send a frame inpersonating the radio station frame to update the 
        // speed of the vehicle, hides the radio frame. Done quickly to prevent flicker.
        send_speed_frame();
        lastPrint = millis();
        halfSecond = false;
      } else if (incoming.id == CAN_ID_VEHICLE_SPEED) { // Same data in ID: 0x84214006
        uint16_t speed = (incoming.data.bytes[0] << 8) + incoming.data.bytes[1];
        speed /= SPEED_DIVIDER_MPH; // Magic value to get to MPH.
        currentSpeed = speed;
      }
    }
  }
  
  // Print every half-second, 1 hz felt too slow.
  if (millis() - lastPrint > 500 && halfSecond == false) {
    halfSecond = true;
    send_speed_frame();
  }
}
