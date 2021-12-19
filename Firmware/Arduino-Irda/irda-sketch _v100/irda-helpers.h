#ifndef IRDA_HELPERS
#define IRDA_HELPERS

#define dPcComInit(x) Serial.end();Serial.begin(x)
#define dPcComPrintFormatedHex(x) {Serial.print(" ");Serial.print(x,HEX);}
#define dPcComPrintDec(x) Serial.print(x,DEC)
#define dPcComPrintHex(x) Serial.print(x,HEX)

#define dPcComPrintString8Hex(string, x) {Serial.print(string);Serial.print(x,HEX);}
#define dPcComPrintlnString8Dec(string, x) {Serial.print(string);Serial.print(x,DEC);}
#define dPcComPrintlnString8Hex(string, x) {Serial.print(string);Serial.println(x,HEX);}
#define dPcComPrintlnString16Hex(string, x) {Serial.print(string);Serial.println(x,HEX);}
#define dPcComPrintBytes(buf, x) Serial.write(buf,x)
#define dPcComPrint(x) Serial.print(x)
#define dPcComPrintln(x) Serial.println(x)
#define dPcComRead()  Serial.read()
#define dPcComWrite(x) Serial.write(x)
#define dPcComDataAvailable() Serial.available()

#define dIrDaSirInit(x) Serial1.begin(x)
#define dIrDaSirWrite(x) Serial1.write(x)
#define dIrDaSirRead() Serial1.read()
#define dIrDaSirDataAvailable() Serial1.available()
#define dIrDaSirRoom() Serial1.availableForWrite()
#define dIrDaSirPeek() Serial1.peek()



/************************************************
        Types
 ***********************************************/

// Pc Commands
enum COMMANDS { CMD_IRDA_PARAM = 10,
                CMD_RESET_STATS = 11,
                CMD_START_DISCOVERY = 12,
                CMD_CALCULATE_CRC = 13,
                CMD_GET_STATISTIC = 14,
                CMD_GET_DEVICES = 15,
                CMD_READ_FRAME = 16,
                CMD_STATUS = 17,
                CMD_DEBUG = 18,
                CMD_IRDA_DISC = 19,
                CMD_IRCOMM_PAYLOAD = 20,
                STATE_WAIT = 0xFFFC,
                STATE_COMMAND = 0xFFFD,
                STATE_TIMEOUT = 0xFFFE,
                STATE_CLEANUP = 0xFFFF
              };




enum IrDaLapStates_e {
  LAP_NDM = 0,
  LAP_QUERY,
  LAP_SETUP,
  LAP_XMIT,
  LAP_NRM,
  LAP_PCLOSE
};

enum IrDaLmMultiplexerStates_e {
  LMMUX_INIT = 0,
  LMMUX_ACTIVE = 1
};

enum IrDaCommStates_e {
  IRCOMM_IDLE = 0,
  IRCOMM_WAITI=1,
  IRCOMM_WAITR=2,
  IRCOMM_CONNECTED=3
};

enum IrDaIapServiceStates{
  IAP_CONNECT,
  IAP_DISCOVER_IRCOMM,
  IAP_REQUEST_IRCOMM_LSAP,
  IAP_VALIDATE_IRCOMM_LSAP,
  IAP_DISCONNECT
};

/************************************************
        Change IrDa Baud
 ***********************************************/
void vSetHSDLBaud(uint32_t ui8Baud) {
  pinMode(21, OUTPUT); //A0
  pinMode(20, OUTPUT); //A2
  pinMode(18, OUTPUT); //A1
  switch (ui8Baud) {
    case dPvLapBaud2400bps:
      digitalWrite(21, LOW);    // A2 A1
      digitalWrite(20, HIGH);
      digitalWrite(18, HIGH);
      dIrDaSirInit(2400);
      break;
    case dPvLapBaud9600bps:
      dIrDaSirInit(9600);
      digitalWrite(21, HIGH);   // A1 A0
      digitalWrite(20, LOW);
      digitalWrite(18, HIGH);
      break;
    case dPvLapBaud19200bps:

      digitalWrite(21, LOW);   // A1
      digitalWrite(20, LOW);
      digitalWrite(18, HIGH);
      dIrDaSirInit(19200);
      break;
    case dPvrLapBaud38400bps:
      digitalWrite(21, LOW);   // A2
      digitalWrite(20, HIGH);
      digitalWrite(18, LOW);

      dIrDaSirInit(38400);
      break;
    case dPvLapBaud57600bps:
      digitalWrite(21, HIGH);   // A0
      digitalWrite(20, LOW);
      digitalWrite(18, LOW);
      dIrDaSirInit(57600);
      break;
    case dPvLapBaud115200bps:
      digitalWrite(21, LOW);
      digitalWrite(20, LOW);
      digitalWrite(18, LOW);
      dIrDaSirInit(115200);
      break;
  }
}

#endif
