#ifndef hottMessages_h
#define hottMessages_h

const int hottAsciiLength = 173;
// Textpacket
static struct {
  byte startByte; // 0x7B
  byte escape; // 0xE0 D=GAM, E=EAM, A=GPS, 9=Vario
  byte alarm;
  char text[8*21];
  byte endByte; // 0x7D
  byte cksum;
} hottASCII;

const int hottEAMLength = 45;
// EAM
static struct {
  byte startByte; // 0x7C
  byte packetId; // 0x8E
  byte beeps;
  byte idTextMode; // 0xE0
  byte inverse;
  byte inverseStatus;
  byte cell1L;
  byte cell2L;
  byte cell3L;
  byte cell4L;
  byte cell5L;
  byte cell6L;
  byte cell7L;
  byte cell1H;
  byte cell2H;
  byte cell3H;
  byte cell4H;
  byte cell5H;
  byte cell6H;
  byte cell7H;
  unsigned int bat1;
  unsigned int bat2;
  byte temp1;
  byte temp2;
  unsigned int altitude;
  unsigned int current;
  unsigned int powerV;
  unsigned int mAh;
  unsigned int climbrate1;
  byte climbrate3;
  unsigned int rpm;
  byte electricMinutes;
  byte electricSeconds;
  byte speed;
  byte version;
  byte endByte; // 0x7D
  byte cksum;
} hottEAM;

#endif
