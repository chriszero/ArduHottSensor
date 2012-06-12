#include "HottMessages.h"

#define HottStart 0;
#define AsciiPacket 1
#define BinaryPacket 2;
static int state = 0;

static char sendBuffer[180];

void loopHott() {
  if (hottSerial.available() > 1) {
    
    for(int i = 0; i < 2; i++) {
      uint8_t c = hottSerial.read();
      c = (uint8_t)(c & 0xFF); // nur das lowest byte
      
      switch (state) {
       case 0: // Check first byte
         if(c == 0x7F) { 
           state = AsciiPacket;
           inAscii = true;
         }
         else if(c == 0x80) { 
           state = BinaryPacket; 
           inAscii = false;
         }
         break;
       case 1 : // Ascii
         if((c & 0xF0) == 0xE0) { // Ascii EAM, compare just highest byte
           hottBuildAscii(c);
         }
         state = HottStart;
         // ...
         break;
       case 2 : // Binary
         if(c == 0x8E) { // BinaryEAM;
           hottBuildEAM();
         }
         state = HottStart;
         // ... weitere
         break;
       default:
         state = HottStart;
      }
    }
  }
  
}

void clearLine(int line) {
  memset(&hottASCII.text[line*21], ' ', 21);
}

void clearAll() {
  memset(&hottASCII.text[0], ' ', 21*8);
}

/*
 * Prepares text for sending it to the HoTT display
 * returns the position of the last char printed
 */
int hottPrint(int line, int col, char* text, boolean inv=false) {
  int i = 0;
  while(text[i] != '\0' && i < 21 - col) {
   hottASCII.text[(line * 21) + col + i] = inv ? text[i] | 0x80 : text[i];
   i++;
  }
  return i + col;
}

int curserPos = 1;
boolean voltageAlarmEdit = false;
float voltageAlarmBackupValue = 0;

void hottBuildAscii(byte button) {
  uint8_t key = (uint8_t)(button & 0x0F); // get pressed button
  int voltageposition = 0;
  
  hottASCII.startByte = 0x7B; // 0x7B
  hottASCII.escape = 0xE0; // 0xE0 D=GAM, E=EAM, A=GPS, 9=Vario
  hottASCII.alarm = 0;
  
  // Textmenü generieren
  clearAll();
  
  hottASCII.endByte = 0x7D; // 0x7D
  
  
  hottPrint(0, 0, "Custom HOTTv7");
  
  hottPrint(1, 1, "EAM Emu");
  hottPrint(2, 1, "Reset Vario");
  hottPrint(3, 1, "Reset Capacity");
  
  voltageposition = hottPrint(4, 1, "Lipo Alarm bei: ");
  
  switch(key) {
   case  0x07: 
     // ESC
     if(voltageAlarmEdit) {
       voltageAlarmEdit = false;
       VoltageAlarm = voltageAlarmBackupValue;
     }
     hottASCII.escape = 0x01;
     break;
   case  0x0D: 
     // INC
     if(voltageAlarmEdit) {
       VoltageAlarm += 0.1;
     }
     else if(curserPos < 8) curserPos++;
     break;
   case  0x0B: 
     // DEC
     if(voltageAlarmEdit) {
       VoltageAlarm -= 0.1;
     }
     else if(curserPos > 1) curserPos--;
     break;
   case  0x0E: 
     // ENTER
     break;
   case  0x09: 
     // SET
     if(voltageAlarmEdit) {
       voltageAlarmEdit = false;
       break;
     }
     if(curserPos = 2) GroundAltitude = AbsoluteAltitude;
     if(curserPos = 3) Capacity = 0;
     if(curserPos = 4) {
       voltageAlarmEdit = true;
       voltageAlarmBackupValue = VoltageAlarm;
     }
     break;
   case  0x0F:
   default:
     // NONE
     break;
  }
  
  hottPrint(curserPos, 0, ">");
  
  // print Voltage alarm level
  char buffer[4];
  dtostrf(VoltageAlarm, 1, 1, buffer);
  hottPrint(4, voltageposition, buffer, voltageAlarmEdit);
  
  memcpy(&sendBuffer, &hottASCII, hottAsciiLength);
  
  delay(5);
  if(hottSerial.available() > 0) return; // Data on the line... return
  
  sendHottPacket(hottAsciiLength);
}

void hottBuildEAM() {
  hottEAM.startByte = 0x7C; // 0x7C
  hottEAM.packetId = 0x8E; // 0x8E
  hottEAM.beeps = 0;
  hottEAM.idTextMode = 0xE0; // 0xE0
  hottEAM.inverse = 0;
  hottEAM.inverseStatus = 0;
  
  // Packet befüllen
  hottEAM.cell1L = 210;
  hottEAM.cell2L = 110;
  
  hottEAM.powerV = Voltage;
  hottEAM.current = Current;
  hottEAM.mAh = Capacity;
  
  hottEAM.temp1 = (byte)Temperature + 20; // byte!! 46 = 25C, 20 = 0C
  
  hottEAM.altitude = 500 + ((AbsoluteAltitude - GroundAltitude));
  
  hottEAM.climbrate1 = 30000 + (Climbrate1*100);
  hottEAM.climbrate3 = 120;
  
  hottEAM.endByte = 0x7D; // 0x7D
  
  // memcpy ( void * destination, const void * source, size_t num );
  memcpy(&sendBuffer, &hottEAM, hottEAMLength);
  
  delay(5);
  if(hottSerial.available() > 0) return; // Data on the line... return
  
  sendHottPacket(hottEAMLength);
}

void sendHottPacket(int length) {
  pinMode(HOTTRX, OUTPUT); // switchoff RX-Line
  
  int ck = 0;
  for(int i = 0; i < length - 1; i++) {
   ck += sendBuffer[i];
   hottSerial.write(sendBuffer[i]);
  }
  hottSerial.write((byte)ck); // write Checksum
  
  delayMicroseconds(2000); // 2ms
  pinMode(HOTTRX, INPUT);
}
