#include <memory.h>
#include <Wire.h>
#include "GlobalData.h"

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  randomSeed(analogRead(0));
  Serial.begin(115200);  // start serial for output

  Wire.begin(43);
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(100+random(1000));

  gStatus++;
  memcpy(&gTotal[(LOC_STATUS-LOC_START)],&gStatus,LEN_STATUS);
  
  gIP+= 101;
  memcpy(&gTotal[(LOC_IP-LOC_START)],&gIP,LEN_IP);

  gDocked--;
  memcpy(&gTotal[(LOC_DOCKED-LOC_START)],&gIP,LEN_DOCKED);

  gXMeters -=121;
  memcpy(&gTotal[(LOC_X_METERS-LOC_START)],&gXMeters,LEN_X_METERS);
  
  gYMeters += 143;
  memcpy(&gTotal[(LOC_Y_METERS-LOC_START)],&gYMeters,LEN_Y_METERS);

  gTheta += 10;
  memcpy(&gTotal[(LOC_THETA-LOC_START)],&gTheta,LEN_THETA);

  gLat +=1.001;
  memcpy(&gTotal[(LOC_LAT-LOC_START)],&gLat,LEN_LAT);

  gLong -=1.001;
  memcpy(&gTotal[(LOC_LONG-LOC_START)],&gLong,LEN_LONG);

  gCharge +=43;
  memcpy(&gTotal[(LOC_CHARGE-LOC_START)],&gCharge,LEN_CHARGE);

  gVolt += 243;
  memcpy(&gTotal[(LOC_VOLT-LOC_START)],&gVolt,LEN_VOLT);

  gCurrent -= 273;
  memcpy(&gTotal[(LOC_CURRENT-LOC_START)],&gCurrent,LEN_CURRENT);

  gTemp -= 12;
  memcpy(&gTotal[(LOC_TEMP-LOC_START)],&gTemp,LEN_TEMP);

  gRSCaution += 2;
  memcpy(&gTotal[(LOC_RS_CAUTION-LOC_START)],&gRSCaution,LEN_RS_CAUTION);
  
  gRSWarning += 4;
  memcpy(&gTotal[(LOC_RS_WARNING-LOC_START)],&gRSWarning,LEN_RS_WARNING);

  gRSStatus += 8;
  memcpy(&gTotal[(LOC_RS_STATUS-LOC_START)],&gRSStatus,LEN_RS_STATUS);

  gRSInfo -= 1;
  memcpy(&gTotal[(LOC_RS_INFO-LOC_START)],&gRSInfo,LEN_RS_INFO);

  for(int i=0;i<LEN_MAP_NAME;i++)
  {
    uint8_t chr = 'A'+i;
    gMapName[i] = (uint8_t)chr;
    int loc = (LOC_MAP_NAME-LOC_START)+i;
    gTotal[loc] = (uint8_t)chr;
  }

  transmit(random(17));
//  transmit(random(2));
  delay(11);
}

void transmit(uint8_t command)
{
  uint8_t tlength = 0;
  uint8_t location = 0;

  switch(command)
  {
    case LOC_IP:
      tlength = LEN_IP;
      break;
    case LOC_DOCKED:
      tlength = LEN_DOCKED;
      break;
    case LOC_X_METERS:
      tlength = LEN_X_METERS;
      break;
    case LOC_Y_METERS:
      tlength = LEN_Y_METERS;
      break;
    case LOC_THETA:
      tlength = LEN_THETA;
      break;
    case LOC_LAT:
      tlength = LEN_LAT;
      break;
    case LOC_LONG:
      tlength = LEN_LONG;
      break;
    case LOC_CHARGE:
      tlength = LEN_CHARGE;
      break;
    case LOC_VOLT:
      tlength = LEN_VOLT;
      break;
    case LOC_CURRENT:
      tlength = LEN_CURRENT;
      break;
    case LOC_TEMP:
      tlength = LEN_TEMP;
      break;
    case LOC_RS_CAUTION:
      tlength = LEN_RS_CAUTION;
      break;
    case LOC_RS_WARNING:
      tlength = LEN_RS_WARNING;
      break;
    case LOC_RS_STATUS:
      tlength = LEN_RS_STATUS;
      break;
    case LOC_RS_INFO:
      tlength = LEN_RS_INFO;
      break;
    case LOC_MAP_NAME:
      tlength = LEN_MAP_NAME;
      break;
    case INDEX_STATUS:
    defualt:
      command = LOC_STATUS;
      tlength = LEN_STATUS;
      break;
  }
  location = command - LOC_START;

  Serial.print((int)command);
  Serial.print(",");    
  Serial.print((int)tlength);
  Serial.print(",");    
  Serial.println((int)location);

  Wire.beginTransmission(43);
  Wire.write(command);
  for(int i=0;i<tlength;i++)
  {
    Wire.write(gTotal[location+i]);
  }
  Wire.endTransmission();
}

void receiveEvent(int howMany)
{
  (void)howMany; // avoid compiler warning about unused parameter
  int command = Wire.read(); // receive byte as a character
  Serial.println(command);         // print the character
  transmit(command);
}
