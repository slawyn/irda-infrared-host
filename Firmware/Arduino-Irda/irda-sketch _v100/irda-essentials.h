/*

         IrDa Definitions and Functions
         Date: 14.01.2021

*************************************/


#ifndef IRDA_ESSENTIALS
#define IRDA_ESSENTIALS

#include "irda-crc.h"
#include "irda-buffer.h"
#include "irda-devices.h"

// Size
#define SIZEOF(x) sizeof(x) / sizeof(x[0])



/**********************************************

  SIR Wrapper

*******************************************/
#define dXBOF 11
#define dSTF 0x7D
#define dBOF 0xC0
#define dBOFFF 0xFF
#define dEOF 0xC1
#define dSizeOfFcs 2




// ------------ Services ------------------------
#define dServiceDevice                      "Device"
#define dServiceDeviceAttributeName         "DeviceName"
#define dServiceDeviceAttributeSupport      "IrLMPSupport"

// Format specific for Support attribute 1<LmpVersion>n<SupportByte|Extension>m<LM-MuX|Extension>
#define dIasSupportGetInfoBaseDetails             0x01
#define dIasSupportGetObjects                     0x02
#define dIasSupportGetValue                       0x04
#define dIasSupportUnassigned1                    0x08
#define dIasSupportGetObjectInfo                  0x10
#define dIasSupportGetAttributeNames              0x20
#define dIasSupportUnassigned2                    0x40
#define dIasSupportExtension                      0x80

#define dLmMuxSupportExclusiveMode                0x01
#define dLmMuxSupportRoleExchange                 0x02
#define dLmMuxSupportConnectionlessData           0x04
#define dLmMuxSupportUnassigned                   0x78
#define dLmMuxSupportExtension                    0x80


#define dServiceIrDaComm                    "IrDA:IrCOMM"
#define dServiceIrDaCommAttributeParameters "Parameters"

#define dPiIrCommServiceType                     0x00
#define dPlIrCommServiceType                        1
#define dPvIrCommServiceType3WireRaw             0x01
#define dPvIrCommServiceType3Wire                0x02
#define dPvIrCommServiceType9Wire                0x04
#define dPvIrCommServiceTypeCentronics           0x08

#define dPiIrCommPortType                        0x01
#define dPlIrCommPortType                           1
#define dPvIrCommPortTypeSerial                  0x01
#define dPvIrCommPortTypeParallel                0x02

#define dGeneralAttributeTinyTpLsap      "IrDA:TinyTP:LsapSel"
#define dGeneralAttributeInstanceName    "IrDA:LMP:InstanceName"




// Rate in Bits/second
#define dPi3WireParameterDataRate               0x10
#define dPl3WireParameterDataRate                  4
#define dPv3WireParameterDataRate9600           9600
#define dPv3WireParameterDataRate19200         19200
#define dPv3WireParameterDataRate38400         38400
#define dPv3WireParameterDataRate57600         57600
#define dPv3WireParameterDataRate115200       115200
#define dPv3WireParameterDataRate576000       576000
#define dPv3WireParameterDataRate1152000     1152000
#define dPv3WireParameterDataRate576000       576000
// Data format
#define dPi3WireParameterDataFormat            0x11
#define dPl3WireParameterDataFormat               1
#define dPv3WireParameterDataFormatDatabits5   0x00
#define dPv3WireParameterDataFormatDatabits6   0x01
#define dPv3WireParameterDataFormatDatabits7   0x02
#define dPv3WireParameterDataFormatDatabits8   0x03
#define dPv3WireParameterDataFormatStopbits1   0x00
#define dPv3WireParameterDataFormatStopbits2   0x04
#define dPv3WireParameterDataFormatParityOff   0x00
#define dPv3WireParameterDataFormatParityOn    0x08
#define dPv3WireParameterDataFormatParityOdd   0x00
#define dPv3WireParameterDataFormatParityEven  0x10
#define dPv3WireParameterDataFormatParityMark  0x20
#define dPv3WireParameterDataFormatParitySpace 0x30

// state of flow control on input and on output
#define dPi3WireParameterFlowControl            0x12
#define dPl3WireParameterFlowControl               1
#define dPv3WireParameterFlowControlXonXoffIn   0x01
#define dPv3WireParameterFlowControlXonXoffOut  0x02
#define dPv3WireParameterFlowControlRtsCtsIn    0x04
#define dPv3WireParameterFlowControlRtsCtsOut   0x08
#define dPv3WireParameterFlowControlDsrTrIn     0x10
#define dPv3WireParameterFlowControlDsrTrOut    0x20
#define dPv3WireParameterFlowControlEnqAckIn    0x40
#define dPv3WireParameterFlowControlEnqAckOut   0x80

// Xon and Xoff Character definition
#define dPi3WireParameterXonXoff               0x13
#define dPl3WireParameterXonXoff                  2
#define dPv3WireParameterXonCharacter           // <define here as an example
#define dPv3WireParameterXoffCharacter

// Enq and Ack Character definition
#define dPi3WireParameterEnqAck                0x14
#define dPl3WireParameterEnqAck                   2
#define dPv3WireParameterEnqCharacter
#define dPv3WireParameterAckCharacter

// Line status on the other side, 1 = Error
#define dPl3WireParameterLineStatus            0x15
#define dPi3WireParameterLineStatus               1
#define dPv3WireParameterLineStatusReserved    0xF1
#define dPv3WireParameterLineStatusOverrunErr  0x02
#define dPv3WireParameterLineStatusParityErr   0x04
#define dPv3WireParameterLineStatusFramingErr  0x08

// Sender signals break state
#define dPi3WireParameterBreak                0x16
#define dPl3WireParameterBreak                   1
#define dPv3WireParameterBreakSetBreak        0x01

// Deltax 0 = circuit not changed/ Deltax 1 = circuit changed, State 0 = Low/ State 1 = High
#define dPi9WireParameterDteLine            0x20
#define dPl9WireParameterDteLine               1
#define dPv9WireParameterDteLineDeltaDtr    0x01
#define dPv9WireParameterDteLineDeltaRts    0x02
#define dPv9WireParameterDteLineDtrState    0x04
#define dPv9WireParameterDteLineRtsState    0x08

#define dPi9WireParameterDceLine            0x21
#define dPl9WireParameterDceLine               1
#define dPv9WireParameterDceLineDeltaCts    0x01
#define dPv9WireParameterDceLineDeltaDsr    0x02
#define dPv9WireParameterDceLineDeltaRi     0x04
#define dPv9WireParameterDceLineDeltaCd     0x08
#define dPv9WireParameterDceLineCtsState    0x10
#define dPv9WireParameterDceLineDsrState    0x20
#define dPv9WireParameterDceLineRiState     0x40
#define dPv9WireParameterDceLineCdState     0x80

// Setting Polling
#define dPi9WireParameterPollLine           0x22
#define dPl9WireParameterPollLine              0

/*****************************

  Hardware errors

****************************/

#define dSirErrorPreamble         0x0001
#define dSirErrorBofFormat        0x0002
#define dSirErrorFrameFormat      000004
#define dSirErrorFcs              0x0008
#define dSirErrorWrongLength      0x0010
#define dSirErrorFraming          0x0020
#define dSirErrorOverRun          0x0040
#define dSirErrorBufferOverrun    0x0080
#define dSirErrorMediaTimedOut    0x0100

// Software errors
#define dLapErrorUnknownXidFormat 0x0001
#define dLapErrorWrongCmdLength   0x0002
#define dLapErrorCommandFrame     0x0004
#define dLapErrorWrongHintLength  0x0008
#define dLapErrorUnknownCommand   0x0010

typedef struct {
  uint8_t arrui8BaudRate[3];
  uint8_t arrui8MaximumTurnAround[3];
  uint8_t arrui8DataSize[3];
  uint8_t arrui8WindowSize[3];
  uint8_t arrui8AdditionalBofs[3];
  uint8_t arrui8MinimumTurnAroundTime[3];
  uint8_t arrui8LinkDisconnectThreshold[3];
} DeviceParameters_t;

typedef struct {
  uint8_t ui8Type;
} ParameterType0_t;

typedef struct {
  uint8_t ui8Type;
  uint32_t ui32Integer;
} ParameterType1_t __attribute__((packed));

typedef struct {
  uint8_t ui8Type;
  uint16_t ui16Length;
  uint8_t arrui8Sequence[1024];   // max Length is 1024
} ParameterType2_t __attribute__((packed));

typedef struct {
  uint8_t ui8Type;
  uint8_t ui8CharSet;
  uint8_t ui8Length;
  uint8_t arrui8Characters[255];
} ParameterType3_t __attribute__((packed));


// ParameterSet for Configuration
typedef struct {
  uint8_t ui8PI;
  uint8_t ui8PL;
  uint8_t arrui8PV[1024];
} Parameter_t;

typedef struct {
  Parameter_t *pstcParameters;
  uint8_t ui8ParameterSize;
} ParameterList_t;


typedef struct {
  uint32_t ui32TotalTransmitted;
  uint32_t ui32TotalReceived;
  uint32_t ui32TotalEchoed;
  uint32_t ui32UpTime;
  uint8_t ui8DiscoverySequences;
  uint32_t ui32LatchedError;
  uint8_t ui8ErrorCount;
  uint8_t ui8LatchedCommand;
} Statistics_t;

typedef struct {
  uint8_t ui8NameLength;
  char * pcName;
} ClassName_t;

typedef struct {
  char * pcName;
  uint8_t ui8NameLength;
  uint8_t ui8Type;
  Parameter_t stcParameter;
} Attribute_t;

typedef struct {
  ClassName_t stcClassName;
  Attribute_t *pstcAttributes;
  uint8_t ui8NumberOfAttributes;
} Object_t;

typedef struct {
  Object_t *arrstcObjects;
  uint8_t ui8NumberOfObjects;
} IAS_t;

typedef struct {
  PrimaryDevice_t stcPrimaryDevice;
  Statistics_t stcStatistics;
  IAS_t stcIas;
  uint16_t ui16MediaBusyFlagTimeout;
  uint16_t ui16SirErrorFlags;
  uint16_t ui16LapErrorFlags;
  uint8_t ui8JustListen;
  uint8_t ui8NumberOfSlots;
  uint8_t ui8RoleExchange;      // If implemented properly, can be used to switch between Primary and Secondary Roles
  uint8_t ui8xBof;
  uint8_t ui8DiscoverySlots;

  // Lap
  uint8_t ui8LapState;
  uint8_t ui8RemoteBusy;
  uint8_t ui8LocalBusy;
  uint8_t ui8Vr;        // Sequence number of next frame to be received
  uint8_t ui8Vs;        // Sequence number of next frame to be transmitted
  uint8_t ui8As;

} IrDa_t;





/**********************************************

  LAP

**********************************************/
#define dGenerateNewDeviceAddress()      (random(1,0xFFFFFFFE))  // Cannot be 0 or 0xFFFFFFFF, 0xFFFFFFFF is for sniffing
#define dLapGenerateNewConnectAddress() ((random(1,0x7F))<<1) // Address can't be 0, 0 is a NULL connection address?
#define dLapAddressCommandFrame             0x01   // 0 if Response
#define dLapAddressBroadcast                0xFE
#define dLapControlInformationSequenceMax      8
#define dLapControlPollFinal                0x10   // When set Allows the Secondary to start sending, If set by the Secondry it indicates the last Frame
#define dLapControlPollFinalpos                3


//-------------------- Unnumbered Commands --------------------------------
#define dIsLapControlUnnumbered(control)   ((control&0x03) == 0x03)
#define dLapControlResponseUa              0x63  // This response is used for acknowledgement of the command, can be applied to Disc and Snrm

#define dLapControlCommandXid              0x2F  // Id Exchange. Expects XID with C/R Bit Reset
#define dLapControlResponseXid             0xAF

#define dLapControlCommandSnrm             0x83  // Set Normal Response Mode, establish/reset connection. NRM after that. Expects "Ua" as Response.
#define dLapControlResponseRnrm            0x83  // Request Connection, sent from secondary

#define dLapControlCommandTest             0xE3
#define dLapControlResponseTest            0xE3

#define dLapControlResponseFrmr            0x87 // Can be sent by the primary station to initiate a connection

#define dLapControlCommandDisc             0x43  // Disconnect. No Info. Expects "Ua" Response or Rd
#define dLapControlResponseDm              0x0F  // This Response is sent when there is an error during LAP-Connection-Establishment
#define dLapControlResponseRd              0x43  // Can be caused by a timeout and communication problems during NDM

#define dLapControlCommandUi               0x03  // Unnumbered Information. Either ConnectAddress or Broadcast + C/R Bit. NDM max info size 400-16 bytes, NRM negotiated size. Expects "Ui" Response?
#define dLapControlResponseUi              0x03



//-------------------- Supervisory Commands ------------------------------
// Assist in transfer of info. Convey Ready, busy, report sequence errors
#define dIsLapControlSupervisory(control)      ((control & 0x03) == 0x01)
#define dLapControlSupervisoryMask        0x0F
#define dLapControlSupervisoryRr          0x01
#define dLapControlSupervisoryRnr         0x05
#define dLapControlSupervisoryRej         0x09
#define dLapControlSupervisorySrej        0x0D
#define dLapControlSupervisoryNrmask      0xE0
#define dLapControlSupervisoryNrpos          5

// -------------------- Information Commands -------------------------------

// Nr and Ns Counts are transmitted to verify the order and amount of frames, used for frame sequencing. Nr is present in I and S Frames
// if bit 0 is not set, it is an info frame
#define dIsLapControlInfo(control)        ((control&0x01) == 0x00)
#define dLapControlInfo                   0x00
#define dLapControlInfoNrmask             0xE0
#define dLapControlInfoNrpos                 5
#define dLapControlInfoNsmask             0x0E
#define dLapControlInfoNspos                 1
#define dLapMaxSequenceNumber                7

// >Misc.
#define dCompareToControlNs(control, val) (((control&dLapControlInfoNsmask)>>dLapControlInfoNspos) == val)
#define dCompareToControlNr(control, val) (((control&dLapControlInfoNrmask)>>dLapControlInfoNrpos) == val)

typedef struct {
  uint8_t ui8ConnectAddress;
  uint8_t ui8Control;
} LapHeader_t __attribute__((packed));


/************************************************

  Lap Requests and Indication

***********************************************/
#define dRequestSnrm                  0x0001
#define dRequestDisc                  0x0002
#define dRequestUi                    0x0004
#define dRequestXid                   0x0008
#define dRequestTest                  0x0010
#define dRequestSuperRr               0x0020
#define dRequestSuperRnr              0x0040
#define dRequestSuperRej              0x0080
#define dRequestSuperSrej             0x0100
#define dRequestData                  0x0200

#define dIndicSnrm               0x0001
#define dIndicRnrm               0x0002
#define dIndicFrmr               0x0004
#define dIndicDm                 0x0008
#define dIndicRd                 0x0010
#define dIndicUi                 0x0020
#define dIndicXid                0x0040
#define dIndicTest               0x0080
#define dIndicDisc               0x0100
#define dIndicSuperRnr           0x0200
#define dIndicSuperRej           0x0400
#define dIndicSuperSrej          0x0800
#define dIndicSuperRr            0x1000
#define dIndicInfo               0x2000



/*******************************************

  Command XID

*******************************************/
#define dGetMaxSlots(slotflags)            slotflags==dLapXidDiscoveryFlags1Slot?1:(slotflags==dLapXidDiscoveryFlags6Slot?6:(slotflags==dLapXidDiscoveryFlags8Slot?8:16))
#define dDeviceBroadcastAddress            0xFFFFFFFF
#define dLapXidFormatIdentifierv1          0x01
#define dLapXidDiscoveryFlags1Slot         0x00
#define dLapXidDiscoveryFlags6Slot         0x01
#define dLapXidDiscoveryFlags8Slot         0x02
#define dLapXidDiscoveryFlags16Slot        0x03
#define dLapXidDiscoveryFlagsGenNewDevAddr 0x04 // trigger new dev address generation, when set in response means device has generated a new dev. address
#define dLapXidDiscoveryFlagsReserved      0xFE
#define dLapXidSlotNumberInit              0x00
#define dLapXidSlotNumberEnd               0xFF // Discovery is finished when this slot number is used by primary device, secondary device does not use this field
#define dLapXidVersionNumber               0x00 // IrDa 1.1
typedef struct {
  uint8_t ui8FormatIdentifier;  // Is always 1
  uint32_t ui32DeviceSourceAddress;
  uint32_t ui32DeviceDestinationAddress;
  uint8_t ui8DiscoveryFlags;
  uint8_t ui8SlotNumber;
  uint8_t ui8VersionNumber;
} Xid_t __attribute__((packed));


/**********************************************

  Command SNRM

*******************************************/


// Type 1 parameters set "critical" bit, Type 0 don't
// Type 0 parameters need to be negotiated to the same value
#define dIapDataType0               0x00
#define dIapDataType1               0x01
#define dIapDataType2               0x02
#define dIapDataType3               0x03

#define dParameterPiCritical          0x80

#define dPiLapBaud                    0x01
#define dPlLapBaudNormal                 1
#define dPvLapBaud                    0x01
#define dPvLapBaud2400bps             0x01
#define dPvLapBaud9600bps             0x02
#define dPvLapBaud19200bps            0x04
#define dPvrLapBaud38400bps           0x08
#define dPvLapBaud57600bps            0x10
#define dPvLapBaud115200bps           0x20
#define dPvLapBaud576000bps           0x40
#define dPvLapBaud1152000bps          0x80
#define dPlLapBaudExtended               2
#define dPvLapBaudExt4Mbps            0x01

#define dPiLapMaximumTurnAroundTime         0x02|dParameterPiCritical
#define dPlLapMaximumTurnAroundTime            1
#define dPvLapMaximumTurnAroundTime500ms    0x01
#define dPvLapMaximumTurnAroundTime250ms    0x02  // Valid at >=115200
#define dPvLapMaximumTurnAroundTime100ms    0x04  // Valid at >=115200
#define dPvLapMaximumTurnAroundTime50ms     0x08  // Valid at >=115200
#define dPvLapMaximumTurnAroundTimeReserved 0xF0

#define dPiLapDataSize                0x03|dParameterPiCritical
#define dPlLapDataSize                   1
#define dPvLapDataSize64Bytes         0x01
#define dPvLapDataSize128Bytes        0x02
#define dPvLapDataSize256Bytes        0x04
#define dPvLapDataSize512Bytes        0x08
#define dPvLapDataSize1024Bytes       0x10
#define dPvLapDataSize2048Bytes       0x20
#define dPvLapDataSizeReserved        0xC0

#define dPiLapWindowSize                0x04|dParameterPiCritical
#define dPlLapWindowSize                   1
#define dPvLapWindowSize1Frame          0x01
#define dPvLapWindowSize2Frame          0x02
#define dPvLapWindowSize3Frame          0x04
#define dPvLapWindowSize4Frame          0x08
#define dPvLapWindowSize5Frame          0x10
#define dPvLapWindowSize6Frame          0x20
#define dPvLapWindowSize7Frame          0x40
#define dPvLapWindowSizeReserved        0x80

/*
  The Number of BOFs is dependant on the Baud
  Additional BOFs can be calculated as follows:
  2400 bps = Additional BOFs parameter value/48
  9600 bps = Additional BOFs parameter value/12
  19200 bps = Additional BOFs parameter value/6
  38400 bps = Additional BOFs parameter value/3
  57600 bps = Additional BOFs parameter value/2
  115200 bps = Additional BOFs parameter value/1*/
#define dPiLapAdditionalBofs            0x05|dParameterPiCritical
#define dPlLapAdditionalBofs               1
#define dPvLapAdditionalBofs48          0x01
#define dPvLapAdditionalBofs24          0x02
#define dPvLapAdditionalBofs5           0x04
#define dPvLapAdditionalBofs3           0x10
#define dPvLapAdditionalBofs2           0x20
#define dPvLapAdditionalBofs1           0x40
#define dPvLapAdditionalBofs0           0x80

#define dPiLapMinimumTurnAroundTime         0x06|dParameterPiCritical
#define dPlLapMinimumTurnAroundTime            1
#define dPvLapMinimumTurnAroundTime10ms     0x01
#define dPvLapMinimumTurnAroundTime5ms      0x02
#define dPvLapMinimumTurnAroundTime1ms      0x04
#define dPvLapMinimumTurnAroundTime0p5ms    0x08
#define dPvLapMinimumTurnAroundTime0p1ms    0x10
#define dPvLapMinimumTurnAroundTime0p05ms   0x20
#define dPvLapMinimumTurnAroundTime0p01ms   0x40
#define dPvLapMinimumTurnAroundTime0ms      0x80

// Disconnect timeout. If exceeded link disconnects.
// Threshold defines timeout before higher layers are informed that the link has timed out
#define dPiLapLinkDisconnectThresholdTime                 0x08
#define dPlLapLinkDisconnectThresholdTime                    1
#define dPvLapLinkDisconnectThresholdTime3s               0x01 // Threshold 0
#define dPvLapLinkDisconnectThresholdTime8s               0x02 // Threshold 3
#define dPvLapLinkDisconnectThresholdTime12s              0x04 // Threshold 3
#define dPvLapLinkDisconnectThresholdTime16s              0x08 // Threshold 3
#define dPvLapLinkDisconnectThresholdTime20s              0x10 // Threshold 3
#define dPvLapLinkDisconnectThresholdTime25s              0x20 // Threshold 3
#define dPvLapLinkDisconnectThresholdTime30s              0x40 // Threshold 3
#define dPvLapLinkDisconnectThresholdTime40s              0x80 // Threshold 3

typedef struct {
  uint32_t ui32DeviceSourceAddress;
  uint32_t ui32DeviceDestinationAddress;
} Snrm_t __attribute__((packed));

/**********************************************

  Command TEST

**********************************************/
typedef struct {
  uint32_t ui32DeviceSourceAddress;
  uint32_t ui32DeviceDestinationAddress;
} Test_t __attribute__((packed));



/**********************************************

  Command FRMR

**********************************************/
#define dLapFrmrErrorsW                    0x01
#define dLapFrmrErrorsX                    0x02
#define dLapFrmrErrorsY                    0x04
#define dLapFrmrErrorsZ                    0x08
#define dLapFrmrErrorsReserved             0xF0   // Should be 0's
typedef struct {
  uint8_t ui8RejectedFrameControlField;
  uint8_t ui8RejectedFrameExpControlField;
  uint8_t ui8RejectedFrameErrors;
} Frmr_t __attribute__((packed));

/**********************************************

  Command DISC

**********************************************/
typedef struct {
  uint32_t ui32DeviceSourceAddress;
  uint32_t ui32DeviceDestinationAddress;
} Disc_t;


/**********************************************

  Command UI

**********************************************/
typedef struct {
  uint32_t ui32DeviceSourceAddress;
  uint32_t ui32DeviceDestinationAddress;
} UiHeader_t;

#define dLapUiNdmPayload(x) (x-sizeof(LapHeader_t)-sizeof(UiHeader_t)-dSizeOfFcs)
typedef struct {
  LapHeader_t stcLapHeader;
  UiHeader_t stcUiHeader;
  uint8_t arrui8Data[dLapUiNdmPayload(dBufferSize)];
} UiFrameNdm_t;

#define dLapUiNrmPayload(x) (x-sizeof(LapHeader_t)-dSizeOfFcs)
typedef struct {
  LapHeader_t stcLapHeader;
  uint8_t arrui8Data[dLapUiNrmPayload(dBufferSize)];
} UiFrameNrm_t;



/**********************************************

  LMP

**********************************************/
// These Params are used by xid during discovery
#define dLmpNumberOfHintBytes                  2
#define dLmpServiceHint1Pnp                 0x01
#define dLmpServiceHint1Pda                 0x02
#define dLmpServiceHint1Computer            0x04
#define dLmpServiceHint1Printer             0x08
#define dLmpServiceHint1Modem               0x10   // Modem
#define dLmpServiceHint1Fax                 0x20
#define dLmpServiceHint1LanAccess           0x40
#define dLmpServiceHint2Telephony           0x01
#define dLmpServiceHint2FileServer          0x02
#define dLmpServiceHint2IrDaComm            0x04   // IrDaComm
#define dLmpServiceHintExtension            0x80

// 0 = Ascii encoding 1-9 = ISO 0xFF = Unicode
#define dLmpCharEncodingAscii               0x00
#define dLmpCharEncodingISOxx               0x01
#define dLmpCharEncodingUnicode             0xFF

#define dLmpDeviceNicknameCharacterSetAscii dLmpCharEncodingAscii
#define dLmpDeviceNicknameMaxLength           23
#define dLmpDeviceNickname "Alex's IrDa\0"
#define dLmpDeviceUnknownNickname "Unknown\0"

//--------------- I Frame Lmp -------------------------
#define dLmpCommandFrameBit                  0x80
#define dLmpDLsapmask                        0x7F
#define dLmpSLsapmask                        0x7F
#define dLmpSLsapSelRbit                     0x80

// Control
#define dLmpOpcodeABit                      0x80
#define dLmpOpcodeAck                       0x40

// Commands
#define dLmpOpcodeConnectRequest             0x01
#define dLmpOpcodeConnectResponse            (dLmpOpcodeConnectRequest|dLmpOpcodeABit)
#define dLmpOpcodeDisconnectRequest          0x02 // No Confirmation
#define dLmpOpcodeAccessModeRequest          0x03
#define dLmpOpcodeAccessModeResponse         (dLmpOpcodeAccessModeRequest|dLmpOpcodeABit)

// Reasons
#define dLmpDisconnectReasonUserRequest                 0x01
#define dLmpDisconnectReasonUnexpectedLapDisconnect     0x02
#define dLmpDisconnectReasonFailedLapConnect            0x03
#define dLmpDisconnectReasonLapReset                    0x04
#define dLmpDisconnectReasonLmInitiatedDisconnect       0x05
#define dLmpDisconnectReasonDataDeliveredLapDisconnect  0x06
#define dLmpDisconnectReasonNonResponsiveLmMuxClient    0x07
#define dLmpDisconnectReasonNoAvailableLmMuxClient      0x08
#define dLmpDisconnectReasonConnectHalfopen             0x09
#define dLmpDisconnectReasonIlleagalSourceAddress       0x0A
#define dLmpDisconnectReasonUnspecifiedDisconnect       0xFF

// Mode
#define dLmpModeMultiplexed                   0x00
#define dLmpModeExclusive                     0x01

// Status
#define dLmpStatusSuccess                     0x00
#define dLmpStatusFailure                     0x01  // NO such ClassName
#define dLmpStatusFailure2                    0x02  // No such attribute
#define dLmpStatusUnsupported                 0xFF  // Access Mode Confirm Only

// Lmp-Control
typedef struct {
  uint8_t ui8Dlsap;
  uint8_t ui8Slsap;
} LmpHeader_t __attribute__((packed));

typedef struct {
  LmpHeader_t stcLmpHeader;
  uint8_t ui8Opcode;
  union {
    uint8_t ui8Reserved;
    uint8_t ui8Reason;
    uint8_t ui8Status;
  };
} LmpControlHeader_t;

#define dLmpPayloadLegth(x) (x - sizeof(LapHeader_t) - sizeof(LmpHeader_t) - dSizeOfFcs)
typedef struct {
  LmpHeader_t stcLmpHeader;
  uint8_t arrui8Data[dLmpPayloadLegth(dBufferSize)];
} LmpFrame_t;

#define dLmpControlPayloadLegth(x) (x - sizeof(LapHeader_t) - sizeof(LmpControlHeader_t) - dSizeOfFcs)
typedef struct {
  LmpControlHeader_t stcLmpControlHeader;
  uint8_t arrui8Data[dLmpPayloadLegth(dBufferSize)];
} LmpControlFrame_t;


/**********************************************

  Ias

**********************************************/
#define dIapDlsap                         0x00
#define dIapOpcodeLastFrameBit            0x80
#define dIapOpcodeAcknowledgeBit          0x40
#define dIapOpcodemask                    0x3F
#define dIapOpcodeQueryGetInfoBaseDetails 0x01
#define dIapOpcodeQueryGetObjects         0x02
#define dIapOpcodeQueryGetValue           0x03
#define dIapOpcodeQueryGetValueByClass    0x04
#define dIapOpcodeQueryGetObjectInfo      0x05
#define dIapOpcodeQueryGetAttributeNames  0x06

typedef struct {
  uint8_t ui8Opcode;
} IapHeader_t __attribute__((packed));

#define dIapResponseLength(x) (x - sizeof(LapHeader_t) - sizeof(LmpHeader_t) - sizeof(IapHeader_t) - 1 - dSizeOfFcs)
typedef struct {
  uint8_t ui8Status;
  uint8_t arrui8Results[dIapResponseLength(dBufferSize)];
} IapResponse_t;

#define dIapRequestLength(x) (x - sizeof(LapHeader_t) - sizeof(LmpHeader_t) - sizeof(IapHeader_t) - dSizeOfFcs)
typedef struct {
  uint8_t arrui8Request[dIapRequestLength(dBufferSize)];
} IapRequest_t;

#define dIapConnectDataLength(x) (x - sizeof(LapHeader_t) - sizeof(LmpControlHeader_t) - sizeof(IapHeader_t) - dSizeOfFcs)
typedef struct {
  LmpControlHeader_t stcLmpControlHeader;
  IapHeader_t stcIapHeader;
  uint8_t arrui8Data[dIapConnectDataLength(dBufferSize)];
} IapConnectFrame_t;

typedef struct {
  LmpHeader_t stcLmpHeader;
  IapHeader_t stcIapHeader;
  union {
    IapResponse_t stcIapResponse;
    IapRequest_t stcIapRequest;
  };
} IapDataFrame_t;

/**********************************************

  TTP

**********************************************/

// This Parameter defines the max. Sdu distributed over multiple frames
// The Sdu should be reassembled if it does not fit into one frame
#define dTinyTpCreditPflag                                      0x80  // Flag set when parameters are used during connect frame
#define dTinyTpCreditMflag                                      0x80  // This flag is used during data transfer to indicate segmentation
#define dTinyTpCreditDeltamask                                  0x7F  // This determines how many pdus can be sent
typedef struct {
  uint8_t ui8Credit;
} TinyTpHeader_t;

// TinyTP parameters are not used for IrComm connect
typedef struct {
  LmpHeader_t stcLmpHeader;
  TinyTpHeader_t stcTinyTpHeader;
  uint8_t arrui8Data[dBufferSize - sizeof(LapHeader_t) - sizeof(LmpHeader_t) - sizeof(TinyTpHeader_t) - dSizeOfFcs];
} TinyTpDataFrame_t;

#define dTinyTpParameterPiMaxSduSize                            0x00
#define dTinyTpParameterPlMaxSduIllegal                            0
#define dTinyTpParameterPlMaxSduSize1                              1
#define dTinyTpParameterPlMaxSduSize2                              2
#define dTinyTpParameterPlMaxSduSize3                              3
#define dTinyTpParameterPlMaxSduSize4                              4
#define dTinyTpParameterPvMaxSduSizeUnbounded             0xFFFFFFFF
#define dTinyTpParameterPvMaxSduSizeSegmentationProbhibited        0
typedef struct {
  uint8_t ui8PI;
  uint8_t ui8PL;
  uint8_t arrui8PV[4];
} TinyTpParameters_t;

typedef struct {
  LmpControlHeader_t stcLmpControlHeader;
  TinyTpHeader_t stcTinyTpHeader;
  union {
    TinyTpParameters_t stcTinyTpConnectParameters;
    uint8_t arrui8Data[dBufferSize - sizeof(LapHeader_t) - sizeof(LmpControlHeader_t) - sizeof(TinyTpHeader_t) - dSizeOfFcs];
  };
} TinyTpConnectFrame_t;


/********************************************

  Frame Lap with Information field

*********************************************/
#define dSduLength(x) (x - sizeof(LapHeader_t) - dSizeOfFcs)
typedef struct {
  union {
    Test_t stcTest;
    Xid_t stcXid;
    Snrm_t stcSnrm;
    Frmr_t stcFrmr;
    Disc_t stcDisc;
    UiFrameNdm_t stcUiNdmFrame;
    UiFrameNrm_t stcUiNrmFrame;
    LmpFrame_t stcLmpFrame;
    LmpControlFrame_t stcLmpControlFrame;
    TinyTpConnectFrame_t stcTinyTpConnectFrame;
    TinyTpDataFrame_t stcTinyTpDataFrame;
    IapDataFrame_t stcIapDataFrame;
    IapConnectFrame_t stcIapConnectFrame;
    uint8_t arrui8Sdu[dSduLength(dBufferSize)];
  };
} Information_t;

typedef struct {
  LapHeader_t stcLapHeader;
  Information_t stcInformation;
} IrDaFrame_t;


/********************************************

  State machine structures

*********************************************/
typedef struct {
  uint16_t ui16Indication;
  uint8_t ui8InformationLength;

  union {
    uint8_t *pui8Pdu;
    IrDaFrame_t *pstcIrDaFrame;
  };
} LapData_t;

typedef struct {
  uint16_t ui16Indication;
  uint8_t ui8DataLength;
  union {
    ParameterList_t stcReceivedParameterList;
    uint8_t  ui8Data;
    uint16_t ui16Data;
    uint32_t ui32Data;
  };
} IapData_t;


// Configuration
const uint8_t arrui8Configuration[] = { dPiLapBaud, dPlLapBaudNormal, dPvLapBaud9600bps, //| dPvLapBaud19200bps | dPvrLapBaud38400bps | dPvLapBaud57600bps | dPvLapBaud115200bps|dPvLapBaud2400bps |;
                                        dPiLapMaximumTurnAroundTime, dPlLapMaximumTurnAroundTime, dPvLapMaximumTurnAroundTime500ms | dPvLapMaximumTurnAroundTime250ms | dPvLapMaximumTurnAroundTime100ms | dPvLapMaximumTurnAroundTime50ms,
                                        dPiLapDataSize, dPlLapDataSize, dPvLapDataSize64Bytes,//|dPvLapDataSize128Bytes|dPvLapDataSize256Bytes|dPvLapDataSize512Bytes|dPvLapDataSize1024Bytes|dPvLapDataSize2048Bytes;
                                        dPiLapWindowSize, dPlLapWindowSize, dPvLapWindowSize1Frame, //|dPvLapWindowSize2Frame|dPvLapWindowSize3Frame|dPvLapWindowSize4Frame|dPvLapWindowSize5Frame|dPvLapWindowSize6Frame|dPvLapWindowSize7Frame;
                                        dPiLapAdditionalBofs, dPlLapAdditionalBofs, dPvLapAdditionalBofs0,//|dPvLapAdditionalBofs1|dPvLapAdditionalBofs2|dPvLapAdditionalBofs3|dPvLapAdditionalBofs5|dPvLapAdditionalBofs12|dPvLapAdditionalBofs24|dPvLapAdditionalBofs48;
                                        dPiLapMinimumTurnAroundTime, dPlLapMinimumTurnAroundTime, dPvLapMinimumTurnAroundTime1ms | dPvLapMinimumTurnAroundTime10ms, //|dPvLapMinimumTurnAroundTime5ms|dPvLapMinimumTurnAroundTime1ms|dPvLapMinimumTurnAroundTime0p5ms|dPvLapMinimumTurnAroundTime0p1ms|dPvLapMinimumTurnAroundTime0p05ms|dPvLapMinimumTurnAroundTime0p01ms|dPvLapMinimumTurnAroundTime0ms;
                                        dPiLapLinkDisconnectThresholdTime, dPlLapLinkDisconnectThresholdTime, dPvLapLinkDisconnectThresholdTime3s  //|dPvLapLinkDisconnectThresholdTime8s|dPvLapLinkDisconnectThresholdTime12s|dPvLapLinkDisconnectThresholdTime16s|dPvLapLinkDisconnectThresholdTime20s|dPvLapLinkDisconnectThresholdTime25s|dPvLapLinkDisconnectThresholdTime30s|dPvLapLinkDisconnectThresholdTime40s;
                                      };


/*************************************

  Lap and Lmp work only with verified frames

**************************************/
/* Always Verify Frame before parsing, verify after passing it to pc for debugging purposes*/
uint8_t ui8VerifyFrame(IrDa_t * pstcIrDa, Buffer_t * pstcBuffer) {
  uint8_t ui8Status = 0;

  // Check only if the length is valid
  if (pstcBuffer->ui8Length >= 4) {
    if (~ui16CalculateFcs(pstcBuffer->arrui8Buffer, pstcBuffer->ui8Length - 2) != *((uint16_t*) &pstcBuffer->arrui8Buffer[pstcBuffer->ui8Length - 2] )) {
      pstcIrDa->ui16SirErrorFlags |= dSirErrorFcs;
      ui8Status = 1;
    }

  } else {
    pstcIrDa->ui16SirErrorFlags |= dSirErrorWrongLength;
    ui8Status = 1;
  }
  return ui8Status;
}


/*************************************

  Lap I-Frame

**************************************/
uint8_t ui8LapInfo(IrDa_t *pstcIrDa, uint8_t *pui8IrDaFrame, Connection_t *pstcConnection, uint8_t ui8Final, uint8_t ui8SduLength) {
  IrDaFrame_t *pstcIrDaFrame;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;
  pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = pstcConnection->ui8ConnectAddress | dLapAddressCommandFrame;
  pstcIrDaFrame->stcLapHeader.ui8Control = (pstcIrDa->ui8Vr << dLapControlInfoNrpos) | (pstcIrDa->ui8Vs << dLapControlInfoNspos) | (ui8Final << 4);
  ui8SduLength += sizeof(LapHeader_t);   // Add Header bytes
  *(uint16_t*)((pui8IrDaFrame + ui8SduLength)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8SduLength);
  return ui8SduLength + dSizeOfFcs;
}


/*************************************

  Lap Supervisory-Frame RR

**************************************/
uint8_t ui8LapRr(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame,  Device_t *pstDevice) {
  IrDaFrame_t *pstcIrDaFrame;
  uint8_t ui8TempOffset;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;

  pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (pstDevice->ui8ConnectAddress  | dLapAddressCommandFrame);
  pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlSupervisoryRr | dLapControlPollFinal | (pstcIrDa->ui8Vr << dLapControlSupervisoryNrpos));;
  ui8TempOffset = sizeof(LapHeader_t);

  *(uint16_t*)((pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}


/*************************************

  Lap Supervisory-Frame RNR

**************************************/
uint8_t ui8LapRnr(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame, Device_t *pstDevice) {
  IrDaFrame_t *pstcIrDaFrame;
  uint8_t ui8TempOffset;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;

  pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (pstDevice->ui8ConnectAddress  | dLapAddressCommandFrame);
  pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlSupervisoryRnr | dLapControlPollFinal | (pstcIrDa->ui8Vr << dLapControlSupervisoryNrpos));
  ui8TempOffset = sizeof(LapHeader_t);

  *(uint16_t*)((pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}



/*************************************

  Lap Unnumbered-Frame Disc

**************************************/
uint8_t ui8LapDisc(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame, Device_t *pstcSecondaryDevice) {
  IrDaFrame_t *pstcIrDaFrame;
  uint8_t ui8TempOffset;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;

  if (pstcSecondaryDevice->pstcConnection != NULL) {
    pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (pstcSecondaryDevice->pstcConnection->ui8ConnectAddress  | dLapAddressCommandFrame);
    pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlCommandDisc | dLapControlPollFinal);
    ui8TempOffset = sizeof(LapHeader_t);
  } else {
    pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (dLapAddressBroadcast | dLapAddressCommandFrame);
    pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlCommandDisc | dLapControlPollFinal);
    pstcIrDaFrame->stcInformation.stcDisc.ui32DeviceSourceAddress = pstcIrDa->stcPrimaryDevice.ui32DeviceAddress;
    pstcIrDaFrame->stcInformation.stcDisc.ui32DeviceDestinationAddress = pstcSecondaryDevice->ui32DeviceAddress;

    ui8TempOffset = sizeof(LapHeader_t) + sizeof(Disc_t);

  }

  *(uint16_t*)((pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}

uint8_t ui8LapDiscUnknown(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame, uint8_t ui8Address) {
  IrDaFrame_t *pstcIrDaFrame;
  uint8_t ui8TempOffset;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;

  pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (ui8Address | dLapAddressCommandFrame);
  pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlCommandDisc | dLapControlPollFinal);
  ui8TempOffset = sizeof(LapHeader_t);


  *(uint16_t*)((pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}

/*************************************

  Lap Unnumbered-Frame Test

**************************************/
/* Not all devices implement this*/
uint8_t ui8LapTest(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame, Device_t *pstcSecondaryDevice) {
  IrDaFrame_t *pstcIrDaFrame;
  uint8_t ui8TempOffset;
  uint8_t ui8TempCount;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;

  if (pstcSecondaryDevice->pstcConnection == NULL) {
    pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (dLapAddressBroadcast | dLapAddressCommandFrame);
    pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlCommandTest | dLapControlPollFinal);
    pstcIrDaFrame->stcInformation.stcTest.ui32DeviceSourceAddress = pstcIrDa->stcPrimaryDevice.ui32DeviceAddress;
    pstcIrDaFrame->stcInformation.stcTest.ui32DeviceDestinationAddress = pstcSecondaryDevice->ui32DeviceAddress;
    ui8TempOffset = sizeof(LapHeader_t) + sizeof(Test_t);
  } else {
    pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (pstcSecondaryDevice->pstcConnection->ui8ConnectAddress  | dLapAddressCommandFrame);
    pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlCommandTest | dLapControlPollFinal);
    ui8TempOffset = sizeof(LapHeader_t) ;
  }

  pui8IrDaFrame[ui8TempOffset++] = 0x01; // Test payload
  *(uint16_t*)((pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}


/*************************************

  Lap Unnumbered-Frame Test

**************************************/
/* Not all devices implement this*/
uint8_t ui8LapUi(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame, Device_t *pstcSecondaryDevice, uint8_t ui8UserDataLength, uint8_t *pui8UserData) {
  uint8_t ui8TempOffset;
  uint8_t ui8TempCount;

  if (pstcSecondaryDevice->pstcConnection == NULL) {
    UiFrameNdm_t *pstcUiFrame = &(((IrDaFrame_t*) pui8IrDaFrame)->stcInformation.stcUiNdmFrame);
    pstcUiFrame->stcLapHeader.ui8ConnectAddress = (dLapAddressBroadcast | dLapAddressCommandFrame);
    pstcUiFrame->stcLapHeader.ui8Control = (dLapControlCommandTest | dLapControlPollFinal);
    pstcUiFrame->stcUiHeader.ui32DeviceSourceAddress = pstcIrDa->stcPrimaryDevice.ui32DeviceAddress;
    pstcUiFrame->stcUiHeader.ui32DeviceDestinationAddress = pstcSecondaryDevice->ui32DeviceAddress;
    ui8TempOffset = sizeof(LapHeader_t) + sizeof(UiHeader_t);

    if (ui8UserDataLength > 0 && ui8UserDataLength <= dLapUiNdmPayload(dBufferSize)) {
      memcpy(pstcUiFrame->arrui8Data, pui8UserData, ui8UserDataLength);
      ui8TempOffset += ui8UserDataLength;
    }

  } else {
    UiFrameNdm_t *pstcUiFrame = &(((IrDaFrame_t*) pui8IrDaFrame)->stcInformation.stcUiNdmFrame);
    pstcUiFrame->stcLapHeader.ui8ConnectAddress = (pstcSecondaryDevice->pstcConnection->ui8ConnectAddress | dLapAddressCommandFrame);
    pstcUiFrame->stcLapHeader.ui8Control = (dLapControlCommandTest | dLapControlPollFinal);

    if (ui8UserDataLength > 0 && ui8UserDataLength <= dLapUiNrmPayload(dBufferSize)) {
      memcpy(pstcUiFrame->arrui8Data, pui8UserData, ui8UserDataLength);
      ui8TempOffset += ui8UserDataLength;
    }
  }

  *(uint16_t*)((pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}



/***********************************************************************

  Lap Unnumbered-Frame Snrm Parameter Negotiation

************************************************************************/
uint8_t ui8LapSnrm(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame, Device_t *pstcSecondaryDevice) {
  IrDaFrame_t *pstcIrDaFrame;
  uint8_t ui8TempOffset;
  uint8_t ui8TempCount;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;

  pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (dLapAddressBroadcast | dLapAddressCommandFrame);
  pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlCommandSnrm | dLapControlPollFinal);
  pstcIrDaFrame->stcInformation.stcSnrm.ui32DeviceSourceAddress = pstcIrDa->stcPrimaryDevice.ui32DeviceAddress;
  pstcIrDaFrame->stcInformation.stcSnrm.ui32DeviceDestinationAddress = pstcSecondaryDevice->ui32DeviceAddress;

  ui8TempOffset = sizeof(LapHeader_t) + sizeof(Snrm_t);

  // Tell Device what connect address to use
  pui8IrDaFrame[ui8TempOffset++] = pstcSecondaryDevice->ui8ConnectAddress;

  memcpy(&pui8IrDaFrame[ui8TempOffset], arrui8Configuration, sizeof(arrui8Configuration));
  ui8TempOffset += sizeof(arrui8Configuration);

  *((uint16_t*)(pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}

/***********************************************************************

  Lap Unnumbered-Frame Xid Discovery

************************************************************************/
uint8_t ui8LapXid(IrDa_t * pstcIrDa, uint8_t *pui8IrDaFrame, uint8_t ui8Slot ) {
  IrDaFrame_t *pstcIrDaFrame;
  uint8_t ui8TempOffset;

  pstcIrDaFrame = (IrDaFrame_t*) pui8IrDaFrame;
  pstcIrDaFrame->stcLapHeader.ui8ConnectAddress = (dLapAddressBroadcast | dLapAddressCommandFrame);
  pstcIrDaFrame->stcLapHeader.ui8Control = (dLapControlCommandXid | dLapControlPollFinal);
  pstcIrDaFrame->stcInformation.stcXid.ui8FormatIdentifier = dLapXidFormatIdentifierv1;
  pstcIrDaFrame->stcInformation.stcXid.ui32DeviceSourceAddress = pstcIrDa->stcPrimaryDevice.ui32DeviceAddress;
  pstcIrDaFrame->stcInformation.stcXid.ui32DeviceDestinationAddress = dDeviceBroadcastAddress;
  pstcIrDaFrame->stcInformation.stcXid.ui8DiscoveryFlags = pstcIrDa->ui8DiscoverySlots;   // Discovery Flags, 1 Slot, no new address
  pstcIrDaFrame->stcInformation.stcXid.ui8SlotNumber = ui8Slot;
  pstcIrDaFrame->stcInformation.stcXid.ui8VersionNumber = dLapXidVersionNumber;

  ui8TempOffset = sizeof(Xid_t) + sizeof(LapHeader_t);
  if (ui8Slot == dLapXidSlotNumberEnd) {
    memcpy(&pui8IrDaFrame[ui8TempOffset], pstcIrDa->stcPrimaryDevice.pui8ServiceRecord, pstcIrDa->stcPrimaryDevice.ui8ServiceRecordLength);
    ui8TempOffset += pstcIrDa->stcPrimaryDevice.ui8ServiceRecordLength;
  }

  *((uint16_t*)(pui8IrDaFrame + ui8TempOffset)) = ~ui16CalculateFcs(pui8IrDaFrame, ui8TempOffset);
  return ui8TempOffset + dSizeOfFcs;
}


/*************************************

  Lap U,S,I-Frame Response

**************************************/
inline uint8_t ui8LapResponse(IrDa_t *pstcIrDa, Buffer_t *pstcBuffer, LapData_t *pstcLapData) {

  // Check if Frame is okay
  pstcLapData->pui8Pdu = pstcBuffer->arrui8Buffer;
  pstcLapData->ui8InformationLength = dSduLength(pstcBuffer->ui8Length);  // substract lap and fcs


  // Should not be a command frame, because we are a primary station
  if ((pstcLapData->pstcIrDaFrame->stcLapHeader.ui8ConnectAddress & dLapAddressCommandFrame)) {
    pstcIrDa->ui16LapErrorFlags |= dLapErrorCommandFrame;
  } else {
    // lap Unnumbered responses
    if (dIsLapControlUnnumbered(pstcLapData->pstcIrDaFrame->stcLapHeader.ui8Control)) {
      switch (pstcLapData->pstcIrDaFrame->stcLapHeader.ui8Control & (~dLapControlPollFinal)) {
        case dLapControlResponseRnrm:
          pstcLapData->ui16Indication |= dIndicRnrm;
          // No Length Checking here
          break;
        case dLapControlResponseUa:
          // Confirm  Disc: Source and Destination are option
          if (pstcLapData->ui8InformationLength == 0 || pstcLapData->ui8InformationLength == 8) {
            pstcLapData->ui16Indication |= dIndicDisc;
          } else if (pstcLapData->ui8InformationLength > 8) {
            pstcLapData->ui16Indication |= dIndicSnrm;
          } else {
            pstcIrDa->ui16LapErrorFlags |= dLapErrorWrongCmdLength;
          }
          break;
        case dLapControlResponseFrmr:
          pstcLapData->ui16Indication |= dIndicFrmr;
          if (pstcLapData->ui8InformationLength != 3) {
            pstcIrDa->ui16LapErrorFlags |= dLapErrorWrongCmdLength;
          }
          break;
        case dLapControlResponseDm:
          pstcLapData->ui16Indication |= dIndicDm;
          if (!(pstcLapData->ui8InformationLength == (0) || pstcLapData->ui8InformationLength == 8)) {
            pstcIrDa->ui16LapErrorFlags |= dLapErrorWrongCmdLength;
          }
          break;
        case dLapControlResponseRd:
          pstcLapData->ui16Indication |= dIndicRd;
          // No Length Checking here
          break;

        // Unnumbered Information are not really used for anything
        case dLapControlResponseUi:
          pstcLapData->ui16Indication |= dIndicUi;
          break;
        case dLapControlResponseXid:
          if (pstcLapData->ui8InformationLength >= (1 + 8 + 3)) { // Atleast the Xid specifics
            pstcLapData->ui16Indication |= dIndicXid;
            if (pstcLapData->pstcIrDaFrame->stcInformation.stcXid.ui8FormatIdentifier != dLapXidFormatIdentifierv1) {
              pstcIrDa->ui16LapErrorFlags |= dLapErrorUnknownXidFormat;
            }
          } else {
            pstcIrDa->ui16LapErrorFlags |= dLapErrorWrongCmdLength;
          }
          break;
        case dLapControlResponseTest:
          pstcLapData-> ui16Indication |= dIndicTest; // If ConnectAddress is a broadcast address there are no source or device address
          break;
        default:
          pstcIrDa->ui16LapErrorFlags |= dLapErrorUnknownCommand;
          break;

      }
      // Supervisory frame
    } else if (dIsLapControlSupervisory(pstcLapData->pstcIrDaFrame->stcLapHeader.ui8Control)) {
      switch (pstcLapData->pstcIrDaFrame->stcLapHeader.ui8Control & dLapControlSupervisoryMask) {
        case dLapControlSupervisoryRr:
          pstcLapData->ui16Indication |= dIndicSuperRr;
          break;
        case dLapControlSupervisoryRej:
          pstcLapData->ui16Indication |= dIndicSuperRej;
          break;
        case dLapControlSupervisoryRnr:
          pstcLapData->ui16Indication |= dIndicSuperRnr;
          break;
        case dLapControlSupervisorySrej:
          pstcLapData->ui16Indication |= dIndicSuperSrej;
          break;
        default:
          pstcIrDa->ui16LapErrorFlags |= dLapErrorUnknownCommand;
          break;
      }
      // Info Frame
    } else {
      pstcLapData->ui16Indication |= dIndicInfo;
    }
  }

  return pstcLapData->ui16Indication;
}

/*************************************

  Lmp Connect

**************************************/
uint8_t ui8LmpConnect(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection, uint8_t ui8SduLength) {
  uint8_t ui8PduLength;
  LmpControlFrame_t *pstcLmpControlFrame;

  pstcLmpControlFrame = &((IrDaFrame_t*) pui8pstcIrDaFrame)->stcInformation.stcLmpControlFrame;
  pstcLmpControlFrame->stcLmpControlHeader.stcLmpHeader.ui8Dlsap = pstcConnection->ui8Dlsap | dLmpCommandFrameBit;
  pstcLmpControlFrame->stcLmpControlHeader.stcLmpHeader.ui8Slsap = pstcConnection->ui8Slsap;
  pstcLmpControlFrame->stcLmpControlHeader.ui8Opcode = dLmpOpcodeConnectRequest;
  pstcLmpControlFrame->stcLmpControlHeader.ui8Reserved = 0x00;

  ui8PduLength = sizeof(LmpControlHeader_t) + ui8SduLength;
  return  ui8LapInfo(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, 1, ui8PduLength);
}

/*************************************

  Lmp Disconnect

**************************************/
uint8_t ui8LmpDisconnect(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection, uint8_t ui8Reason, uint8_t ui8SduLength) {
  uint8_t ui8PduLength;
  LmpControlFrame_t *pstcLmpControlFrame;

  pstcLmpControlFrame = &((IrDaFrame_t*) pui8pstcIrDaFrame)->stcInformation.stcLmpControlFrame;
  pstcLmpControlFrame->stcLmpControlHeader.stcLmpHeader.ui8Dlsap = pstcConnection->ui8Dlsap | dLmpCommandFrameBit;
  pstcLmpControlFrame->stcLmpControlHeader.stcLmpHeader.ui8Slsap = pstcConnection->ui8Slsap;
  pstcLmpControlFrame->stcLmpControlHeader.ui8Opcode = dLmpOpcodeDisconnectRequest;
  pstcLmpControlFrame->stcLmpControlHeader.ui8Reason = ui8Reason;


  pstcConnection->ui8Dlsap = 0x00;
  pstcConnection->ui8Slsap = 0x00;
  ui8PduLength = sizeof(LmpControlHeader_t)  + ui8SduLength;
  return  ui8LapInfo(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, 1, ui8PduLength);
}

/*************************************

  Lmp Access Mode

**************************************/
uint8_t ui8LmpAccessMode(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection, uint8_t ui8Mode) {
  uint8_t ui8PduLength;
  LmpControlFrame_t *pstcLmpControlFrame;

  pstcLmpControlFrame = &((IrDaFrame_t*) pui8pstcIrDaFrame)->stcInformation.stcLmpControlFrame;
  pstcLmpControlFrame->stcLmpControlHeader.stcLmpHeader.ui8Dlsap = pstcConnection->ui8Dlsap | dLmpCommandFrameBit;
  pstcLmpControlFrame->stcLmpControlHeader.stcLmpHeader.ui8Slsap = pstcConnection->ui8Slsap;
  pstcLmpControlFrame->stcLmpControlHeader.ui8Opcode = dLmpOpcodeAccessModeRequest;
  pstcLmpControlFrame->stcLmpControlHeader.ui8Status = 0x00;
  ui8PduLength = sizeof(LmpControlHeader_t) + 1;

  pstcLmpControlFrame->arrui8Data[0] = ui8Mode;
  return  ui8LapInfo(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, 1, ui8PduLength);
}

/*************************************

  Lmp Data

**************************************/
uint8_t ui8LmpData(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection, uint8_t ui8SduLength) {
  uint8_t ui8PduLength;
  LmpFrame_t *pstcLmpFrame;
  pstcLmpFrame = &((IrDaFrame_t*) pui8pstcIrDaFrame)->stcInformation.stcLmpFrame;
  pstcLmpFrame->stcLmpHeader.ui8Dlsap = pstcConnection->ui8Dlsap; // No Command bit for data
  pstcLmpFrame->stcLmpHeader.ui8Slsap = pstcConnection->ui8Slsap;
  ui8PduLength = sizeof(LmpHeader_t) + ui8SduLength;

  return  ui8LapInfo(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, 1, ui8PduLength);
}


/*************************************

  Lmp Responses

**************************************/
uint8_t ui8LmpResponse() {

  // Check the A bit
  return 0;
}

/*************************************

  Ias Connect

**************************************/
uint8_t ui8IapConnect(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection) {
  return ui8LmpConnect(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, 0);
}

uint8_t ui8IapDisconnect(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection, uint8_t ui8Reason) {
  pstcConnection->ui8Dlsap = dIapDlsap;    // Ias Dlsap
  return ui8LmpDisconnect(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, ui8Reason, 0);
}



/*************************************

  Ias GetInfo

**************************************/
uint8_t ui8IapGetInfoBaseDetails(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection) {
  uint8_t ui8PduLength;
  IapDataFrame_t *pstcIapDataFrame;
  pstcIapDataFrame = &((IrDaFrame_t*) pui8pstcIrDaFrame)->stcInformation.stcIapDataFrame;
  pstcIapDataFrame->stcIapHeader.ui8Opcode = dIapOpcodeLastFrameBit | dIapOpcodeQueryGetInfoBaseDetails;

  ui8PduLength = sizeof(IapHeader_t);
  return ui8LmpData(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, ui8PduLength);
}


/*************************************

  Ias GetObjects

**************************************/
uint8_t ui8IapGetObjects(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection, uint16_t ui16FirstId, uint16_t ui16MaxLength, uint8_t ui8LengthOfClassName, uint8_t *pui8ClassName) {
  uint8_t ui8PduLength;

  IapDataFrame_t *pstcIapDataFrame;
  pstcIapDataFrame =   &(((IrDaFrame_t*)pui8pstcIrDaFrame)->stcInformation.stcIapDataFrame);
  pstcIapDataFrame->stcIapHeader.ui8Opcode = dIapOpcodeLastFrameBit | dIapOpcodeQueryGetValueByClass;

  ui8PduLength = sizeof(IapHeader_t) + 2 + 2 + 1;
  pstcIapDataFrame->stcIapRequest.arrui8Request[0] = ui16FirstId >> 8;
  pstcIapDataFrame->stcIapRequest.arrui8Request[1] = ui16FirstId;
  pstcIapDataFrame->stcIapRequest.arrui8Request[2] = ui16MaxLength >> 8;
  pstcIapDataFrame->stcIapRequest.arrui8Request[3] = ui16MaxLength;
  pstcIapDataFrame->stcIapRequest.arrui8Request[4] = 0x00;

  if (ui8LengthOfClassName > 0 && (2 + 2 + 1 + ui8LengthOfClassName) <= sizeof(pstcIapDataFrame->stcIapRequest.arrui8Request)) {
    pstcIapDataFrame->stcIapRequest.arrui8Request[4] = ui8LengthOfClassName;
    memcpy(&pstcIapDataFrame->stcIapRequest.arrui8Request[5], pui8ClassName, ui8LengthOfClassName);
    ui8PduLength += ui8LengthOfClassName;
  }

  return ui8LmpData(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, ui8PduLength);
}

/*************************************

  Ias GetValueByClass

**************************************/
uint8_t ui8IapGetValueByClass(IrDa_t *pstcIrDa, uint8_t *pui8pstcIrDaFrame, Connection_t *pstcConnection, uint8_t ui8LengthOfClassName, uint8_t *pui8ClassName, uint8_t ui8LengthOfAttributeName, uint8_t *pui8AttributeName) {
  uint8_t ui8PduLength;

  IapDataFrame_t *pstcIapDataFrame;
  pstcIapDataFrame =   &(((IrDaFrame_t*)pui8pstcIrDaFrame)->stcInformation.stcIapDataFrame);
  pstcIapDataFrame->stcIapHeader.ui8Opcode = dIapOpcodeLastFrameBit | dIapOpcodeQueryGetValueByClass;

  ui8PduLength = sizeof(IapHeader_t);

  // Validate
  if (ui8LengthOfClassName > 0 && ui8LengthOfAttributeName > 0 && (ui8LengthOfAttributeName + 2 + ui8LengthOfClassName) <= sizeof(pstcIapDataFrame->stcIapRequest.arrui8Request)) {
    pstcIapDataFrame->stcIapRequest.arrui8Request[0] = ui8LengthOfClassName;
    pstcIapDataFrame->stcIapRequest.arrui8Request[ui8LengthOfClassName] = ui8LengthOfAttributeName;
    memcpy(&pstcIapDataFrame->stcIapRequest.arrui8Request[1], pui8ClassName, ui8LengthOfClassName);
    memcpy(&pstcIapDataFrame->stcIapRequest.arrui8Request[1 + ui8LengthOfClassName], pui8AttributeName, ui8LengthOfAttributeName);

    ui8PduLength = 2 + ui8LengthOfClassName + ui8LengthOfAttributeName;
  }

  return ui8LmpData(pstcIrDa, pui8pstcIrDaFrame, pstcConnection, ui8PduLength);
}
inline uint8_t ui8IasGetValueByClass(IapDataFrame_t *pstcIapDataFrame,  IapData_t *pstcIapData) {
  uint8_t ui8Status = 0;
  uint8_t ui8ParameterSize = 0;
  uint16_t ui16ListLength;
  uint16_t ui16TempLength;
  uint8_t ui8Offset;

  // / Length of entries, aka number of objects with the same name
  ui16ListLength = ((uint16_t)(pstcIapDataFrame->stcIapResponse.arrui8Results[0])) << 8 | pstcIapDataFrame->stcIapResponse.arrui8Results[1];
  ui8Offset = 2;
  while (ui16ListLength > 0 && (ui8Offset < pstcIapData->ui8DataLength)) {
    ui16ListLength--;
    //(uint16_t)(pstcIapFrame->arrui8Results[ui8Offset])) << 8 | pstcIapFrame->arrui8Results[ui8Offset + 1]; // Object identifier
    ui8Offset += 2;


    // Check the type of Data
    switch (pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset++]) {
      case dIapDataType0: //
        if ((1 + ui8Offset) <= pstcIapData->ui8DataLength) {
          pstcIapData->ui8Data = pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset];    // Value here
        } else {
          ui8Status = 1;
        }
        ui8Offset++;
        break;
      case dIapDataType1:
        if ((4 + ui8Offset) <= pstcIapData->ui8DataLength) {
          pstcIapData->ui32Data = ((uint32_t)(pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset])) << 24 | ((uint32_t)(pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset + 1])) << 16 | ((uint32_t)(pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset + 2])) << 8 | pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset + 3]; // 32 bit Value here
        } else {
          ui8Status = 2;
        }
        ui8Offset += 4;
        break;
      case dIapDataType2:
        ui16TempLength = ((uint16_t)(pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset])) << 8 | pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset + 1];
        ui8Offset += 2;
        ui16TempLength += ui8Offset; // Last Index

        if (ui16TempLength <= pstcIapData->ui8DataLength) {
          pstcIapData->stcReceivedParameterList.pstcParameters = (Parameter_t*)&pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset];

          // Validate the length of params here
          while (ui8Offset < ui16TempLength) {
            Parameter_t *pstcParameter = (Parameter_t*)&pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset];
            ui8Offset += (2 + pstcParameter->ui8PL);
            ui8ParameterSize += (2 + pstcParameter->ui8PL);
          }

          // If both are equal, the parameters are okay.
          if (ui16TempLength == ui8Offset) {
            pstcIapData->stcReceivedParameterList.ui8ParameterSize = ui8ParameterSize;
          } else {
            ui8Status = 4;
          }
        } else {
          ui8Offset = (pstcIapData->ui8DataLength);
          ui8Status = 5;
        }

        break;
      case dIapDataType3:   // User String
        ui8Offset += 2;
        ui16TempLength = (pstcIapDataFrame->stcIapResponse.arrui8Results[ui8Offset++]);
        ui8Offset += ui16TempLength;
        break;
      default:
        break;
    }
  }

  return ui8Status;
}


/*************************************

  Ias Response parser

**************************************/
uint8_t ui8IasResponse(IapDataFrame_t *pstcIapDataFrame,  IapData_t *pstcIapData) {
  uint8_t ui8Error = 0;

  if (pstcIapDataFrame->stcIapResponse.ui8Status == dLmpStatusSuccess) {
    switch (pstcIapDataFrame->stcIapHeader.ui8Opcode) {
      case (dIapOpcodeLastFrameBit | dIapOpcodeQueryGetValueByClass):
        ui8Error = ui8IasGetValueByClass(pstcIapDataFrame, pstcIapData);
        break;
      default:
        break;
    }
  }
  return ui8Error;

}

/*************************************

  TinyTP Connect

**************************************/
uint8_t ui8TinyTpConnect(IrDa_t *pstcIrDa, uint8_t *pui8IrDaFrame, Connection_t *pstcConnection, TinyTpParameters_t *pstcTinyTpParameters) {
  uint8_t ui8PduLength;
  TinyTpConnectFrame_t *pstcTinyTpConnectFrame;

  pstcTinyTpConnectFrame = &((IrDaFrame_t*)pui8IrDaFrame)->stcInformation.stcTinyTpConnectFrame;
  pstcTinyTpConnectFrame->stcTinyTpHeader.ui8Credit = pstcConnection->ui8Credit;
  ui8PduLength = sizeof(TinyTpHeader_t);

  if (pstcTinyTpParameters != NULL && pstcTinyTpParameters->ui8PL > dTinyTpParameterPlMaxSduIllegal && pstcTinyTpParameters->ui8PL <= dTinyTpParameterPlMaxSduSize4) {
    pstcTinyTpConnectFrame->stcTinyTpHeader.ui8Credit |= dTinyTpCreditPflag;
    pstcTinyTpConnectFrame->stcTinyTpConnectParameters.ui8PI = pstcTinyTpParameters->ui8PI;
    pstcTinyTpConnectFrame->stcTinyTpConnectParameters.ui8PL = pstcTinyTpParameters->ui8PL;
    memcpy(pstcTinyTpConnectFrame->stcTinyTpConnectParameters.arrui8PV, pstcTinyTpParameters->arrui8PV, pstcTinyTpParameters->ui8PL);
    ui8PduLength += (pstcTinyTpConnectFrame->stcTinyTpConnectParameters.ui8PL + 2);
  }

  return ui8LmpConnect(pstcIrDa, pui8IrDaFrame, pstcConnection, ui8PduLength);
}

/*************************************

  TinyTP Disconnect

**************************************/
uint8_t ui8TinyTpDisconnect(IrDa_t *pstcIrDa, uint8_t *pui8IrDaFrame, Connection_t *pstcConnection,  uint8_t ui8Reason, uint8_t ui8UserDataLength, uint8_t *pui8UserData) {
  uint8_t ui8PduLength;
  TinyTpConnectFrame_t *pstcTinyTpConnectFrame;

  pstcTinyTpConnectFrame = &((IrDaFrame_t*)pui8IrDaFrame)->stcInformation.stcTinyTpConnectFrame;
  pstcTinyTpConnectFrame->stcTinyTpHeader.ui8Credit = pstcConnection->ui8Credit;
  ui8PduLength = sizeof(TinyTpHeader_t);

  if (ui8UserDataLength > 0) {
    memcpy(pstcTinyTpConnectFrame->arrui8Data, pui8UserData, ui8UserDataLength);
    ui8PduLength += ui8UserDataLength;
  }

  return ui8LmpDisconnect(pstcIrDa, pui8IrDaFrame, pstcConnection, ui8Reason, ui8PduLength);
}

/*************************************

  TinyTP Data Frame

**************************************/
uint8_t ui8TinyTpData(IrDa_t *pstcIrDa, uint8_t *pui8IrDaFrame, Connection_t *pstcConnection, uint8_t ui8UserDataLength, uint8_t *pui8UserData, uint8_t ui8Parameters) {
  uint8_t ui8PduLength;
  TinyTpDataFrame_t *pstcTinyTpDataFrame;


  pstcTinyTpDataFrame = &((IrDaFrame_t*)pui8IrDaFrame)->stcInformation.stcTinyTpDataFrame;
  pstcTinyTpDataFrame->stcTinyTpHeader.ui8Credit = pstcConnection->ui8Credit;

  ui8PduLength = sizeof(TinyTpHeader_t);


  if (ui8UserDataLength > 0) {
    if (!ui8Parameters) {
      pstcTinyTpDataFrame->arrui8Data[0] = 0x00;
      //pstcTinyTpDataFrame->arrui8Data[0] = 0x03;
      //pstcTinyTpDataFrame->arrui8Data[1] = dPi9WireParameterDteLine;
      //pstcTinyTpDataFrame->arrui8Data[2] = dPl9WireParameterDteLine;
      //pstcTinyTpDataFrame->arrui8Data[3] = dPv9WireParameterDteLineRtsState|dPv9WireParameterDteLineDeltaRts|dPv9WireParameterDteLineDtrState;
      memcpy(&pstcTinyTpDataFrame->arrui8Data[1], pui8UserData, ui8UserDataLength);
      ui8PduLength += ui8UserDataLength + 1;
    } else {
      memcpy(pstcTinyTpDataFrame->arrui8Data, pui8UserData, ui8UserDataLength);
      ui8PduLength += ui8UserDataLength;
    }
  }

  return ui8LmpData(pstcIrDa, pui8IrDaFrame, pstcConnection, ui8PduLength);
}

/*************************************

  TinyTP Response Parsing

**************************************/
uint8_t ui8TinyTpResponse() {

}

#endif
