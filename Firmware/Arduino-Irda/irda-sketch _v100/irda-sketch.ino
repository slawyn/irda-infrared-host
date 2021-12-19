/*********************************
  Author: am
  Date: 12.10.2020
  Description: IrDa basic stack
  Revision: 0.00
  TODO <Problem with RR, is not recognized as own frame>
***********************************/
#include "irda-devices.h"
#include "irda-buffer.h"
#include "irda-essentials.h"
#include "irda-helpers.h"


/************************************************
  Defines
***********************************************/

// Wrapper states
#define dTxEncoderWait          0
#define dTxEncoderPreamble      1
#define dTxEncoderData          2

#define dRxDecoderWait          0
#define dRxDecoderPreambleFF    1
#define dRxDecoderPreambleC0    2
#define dRxDecoderData          3
#define dRxDecoderUnescape      4

// Time defines
#define dDiscoveryPeriod     3000
#define dPTimerTimeout        500
#define dFTimerTimeout        500
#define dMediaBusyTimeout     550
#define dSlotTimerPeriod       50

// Count defines
#define dMaxRetryCountThreshold    100
#define dMaxRetryCount               3
#define dMaxRetryCount3s             6

// Switches
#define dPcCommTimeout          500
#define dDontCheckPfbit           0
#define dCheckPFbit               1


#define dIapOkay 0
#define dIapTimedOut 1
#define dIapDone 2

/************************************************
  Globals
***********************************************/
FrameStorage_t stcUnackedFrameStorage; // Store unconfirmed I-frames
FrameStorage_t stcReceivedFrameStorage; // Store unconfirmed I-frames

CircularBuffer_t stcPcReceiveCircularBuffer;
CircularBuffer_t stcPcTransmitCircularBuffer;

Buffer_t stcIrDaReceiveBuffer;
Buffer_t stcIrDaTransmitBuffer;

IrDa_t stcIrDa;
uint32_t ui32gCurrentTime;
uint32_t ui32sLastTime;
uint8_t ui8Request = 0;

uint8_t arrui8IrCommPayload[10] = {0};




/********************************************

  Pass Frame to Pc

*********************************************/
void vPassIrDaFrameToPc(Buffer_t * pstcBuffer, uint8_t ui8Error) {
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (pstcBuffer->ui8Length + 8));
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (CMD_READ_FRAME));
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (CMD_READ_FRAME >> 8));
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (ui8Error));
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (stcIrDa.ui16SirErrorFlags >> 8));
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (stcIrDa.ui16SirErrorFlags));
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (pstcBuffer->ui8BytePreamble));
  dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (pstcBuffer->ui8Length));
  dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, pstcBuffer->arrui8Buffer, pstcBuffer->ui8Length); /**/
}


void vPassDebugDwordToPc(uint32_t ui32Data) {
  if (0) {
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (8));
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (CMD_DEBUG));
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (CMD_DEBUG >> 8));
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 0);
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (ui32Data >> 24));
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (ui32Data >> 16));
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (ui32Data >> 8));
    dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, (ui32Data >> 0));
  }
}


/********************************************

  Init IAS and Lap parameters

*********************************************/
void vResetParameters() {}

void vInitParameters(IrDa_t * pstcIrDa) {
  Object_t *pstcObj;
  Attribute_t *pstcAttr;

  uint8_t ui8TempLength;

  // Init IrDa standards
  pstcIrDa->ui8LapState =  LAP_NDM;
  pstcIrDa->ui8DiscoverySlots = dLapXidDiscoveryFlags6Slot;
  pstcIrDa->ui8NumberOfSlots = dGetMaxSlots(stcIrDa.ui8DiscoverySlots);
  pstcIrDa->ui8xBof = dXBOF;

  // ------------- Init Device --------------
  // Get Random Address
  pstcIrDa->stcPrimaryDevice.ui32DeviceAddress = dGenerateNewDeviceAddress();

  pstcIrDa->stcPrimaryDevice.ui8ServiceRecordLength = 3 + strlen(dLmpDeviceNickname);
  pstcIrDa->stcPrimaryDevice.pui8ServiceRecord = malloc(pstcIrDa->stcPrimaryDevice.ui8ServiceRecordLength);

  // Set Hints
  pstcIrDa->stcPrimaryDevice.pui8ServiceRecord[0] = dLmpServiceHint1Modem | dLmpServiceHintExtension;
  pstcIrDa->stcPrimaryDevice.pui8ServiceRecord[1] = dLmpServiceHint2IrDaComm;
  pstcIrDa->stcPrimaryDevice.pui8ServiceRecord[2] = dLmpDeviceNicknameCharacterSetAscii;
  memcpy(&pstcIrDa->stcPrimaryDevice.pui8ServiceRecord[3], dLmpDeviceNickname, strlen(dLmpDeviceNickname));

  vResetParameters();
  // -------- Init Ias Objects if secondary device ---------
  /*
    pstcIrDa->stcIas.ui8NumberOfObjects = 2;
    pstcObj = (Object_t*) malloc(sizeof(Object_t) * pstcIrDa-> stcIas.ui8NumberOfObjects);
    (pstcIrDa->stcIas.arrstcObjects) = pstcObj;

    // First Object
    pstcObj->stcClassName.pcName = dServiceDevice;
    pstcObj->stcClassName.ui8NameLength = strlen(pstcObj->stcClassName.pcName);

    pstcObj->ui8NumberOfAttributes = 2;
    pstcObj->pstcAttributes = (Attribute_t*)malloc(sizeof(Attribute_t) * pstcObj->ui8NumberOfAttributes);

    // --- Attribute ---
    pstcObj->pstcAttributes->pcName = dServiceDeviceAttributeName;
    pstcObj->pstcAttributes->ui8NameLength = strlen(pstcObj->pstcAttributes->pcName);

    // --- Attribute ---
    (pstcObj->pstcAttributes + 1)->pcName = dServiceDeviceAttributeSupport;
    (pstcObj->pstcAttributes + 1)->ui8NameLength = strlen((pstcObj->pstcAttributes + 1)->pcName);

    // Second Object
    pstcObj += 1;
    pstcObj->stcClassName.pcName = dServiceIrDaComm;*/


}

/********************************************

  IrDa Comm Service of IrDa

*********************************************/
uint8_t ui8IrDaCommService(Connection_t *pstcConnection, uint8_t *pui8Status) {
  static uint8_t ui8PollLine = 0;
  uint8_t ui8Length = 0;
  uint8_t *pui8IrCommParameters;

  *pui8Status = 0;
  vPassDebugDwordToPc(pstcConnection->ui8IrCommState);
  switch (pstcConnection->ui8IrCommState) {
    case 0: // Connect and Send Params
      ui8Length = ui8TinyTpConnect(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, NULL);   // Parameterless connect
      pstcConnection->ui8IrCommState = IRCOMM_WAITI;
      break;
    case 1:// Wait for Indication ofr connect confirmation
      pstcConnection->ui8IrCommState = IRCOMM_WAITR;
      break;
    case 2:// Issue Connect Response
      pui8IrCommParameters = malloc(0x13);
      pui8IrCommParameters[0] = 0x12;
      pui8IrCommParameters[1] = dPiIrCommServiceType;
      pui8IrCommParameters[2] = dPlIrCommServiceType;
      pui8IrCommParameters[3] = dPvIrCommServiceType9Wire;
      pui8IrCommParameters[4] = dPi3WireParameterDataRate;
      pui8IrCommParameters[5] = dPl3WireParameterDataRate;
      pui8IrCommParameters[6] = dPv3WireParameterDataRate9600 >> 24;
      pui8IrCommParameters[7] = dPv3WireParameterDataRate9600 >> 16;
      pui8IrCommParameters[8] = dPv3WireParameterDataRate9600 >> 8;
      pui8IrCommParameters[9] = dPv3WireParameterDataRate9600;
      pui8IrCommParameters[10] = dPi3WireParameterDataFormat;
      pui8IrCommParameters[11] = dPl3WireParameterDataFormat;
      pui8IrCommParameters[12] = dPv3WireParameterDataFormatDatabits8 | dPv3WireParameterDataFormatStopbits1;
      pui8IrCommParameters[13] = dPi3WireParameterFlowControl;
      pui8IrCommParameters[14] = dPl3WireParameterFlowControl;
      pui8IrCommParameters[15] = dPv3WireParameterFlowControlRtsCtsOut | dPv3WireParameterFlowControlRtsCtsIn;
      pui8IrCommParameters[16] = dPi9WireParameterDteLine;
      pui8IrCommParameters[17] = dPl9WireParameterDteLine;
      pui8IrCommParameters[18] = dPl9WireParameterDteLine | dPv9WireParameterDteLineDeltaRts;
      pstcConnection->ui8Credit = 0;

      ui8Length = ui8TinyTpData(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, 0x13, pui8IrCommParameters, 1); // Send IrComm Parameters
      free(pui8IrCommParameters);
      pstcConnection->ui8IrCommState = IRCOMM_CONNECTED;
      break;
    case 3:// Send User Data
      ;
      if (arrui8IrCommPayload[0] > 0) {
        pstcConnection->ui8Credit = 0;
        ui8Length = ui8TinyTpData(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, arrui8IrCommPayload[0], arrui8IrCommPayload + 1, 0); // Send data here
        arrui8IrCommPayload[0] = 0;
      } else if (++ui8PollLine >= 6) {
        ui8PollLine = 0;
        pui8IrCommParameters = malloc(3);
        pui8IrCommParameters[0] = 2;
        pui8IrCommParameters[1] = dPi9WireParameterPollLine;
        pui8IrCommParameters[2] = dPl9WireParameterPollLine;
        ui8Length = ui8TinyTpData(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, 3, pui8IrCommParameters, 1); // Send IrComm Parameters
        free(pui8IrCommParameters);
      }
      //ui8Length = ui8TinyTpDisconnect(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, dLmpDisconnectReasonNonResponsiveLmMuxClient, 0, NULL); // Send data here
      //pstcConnection->i8IrCommState = COMM_IDLE;
      // Disconnect when the indication comes
      break;
    default:
      break;
  }
  return ui8Length;
}


uint8_t ui8IrDaIapDiscoveryService(Connection_t *pstcConnection, uint8_t *pui8Status) {
#define dLmpTimeout 5
  static uint8_t ui8Timeout = dLmpTimeout;
  uint8_t ui8Length = 0;
  IapData_t stcIapData;
  *pui8Status = dIapOkay;

  if (ui8Timeout > 0) {
    --ui8Timeout;
  } else {
    pstcConnection->ui8IapState = IAP_DISCONNECT;
  }

  vPassDebugDwordToPc(pstcConnection->ui8IapState);
  *pui8Status = dIapOkay;
  switch (pstcConnection->ui8IapState) {
    case IAP_CONNECT:
      ui8Length = ui8IapConnect(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection);
      pstcConnection->ui8IapState = IAP_DISCOVER_IRCOMM;

      break;
    case IAP_DISCOVER_IRCOMM:
      while (stcReceivedFrameStorage.ui8UnackedFrameCount > 0) {
        --stcReceivedFrameStorage.ui8UnackedFrameCount;
        LmpControlFrame_t *pstcLmpControlFrame = &(((IrDaFrame_t*)&stcReceivedFrameStorage.arrstcBuffers[stcReceivedFrameStorage.ui8UnackedFrameCount].arrui8Buffer)->stcInformation.stcLmpControlFrame);


        vPassDebugDwordToPc(pstcLmpControlFrame->stcLmpControlHeader.ui8Opcode);
        // Connected to IAS
        if (pstcLmpControlFrame->stcLmpControlHeader.ui8Opcode == (dLmpOpcodeConnectResponse)) {
          // Parse connection data if needed <here>
          // Request IrDaComm Parameters by GerByValue
          ui8Length = ui8IapGetValueByClass(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, strlen(dServiceIrDaComm), dServiceIrDaComm, strlen(dServiceIrDaCommAttributeParameters), dServiceIrDaCommAttributeParameters);
          ui8Timeout = dLmpTimeout;
          pstcConnection->ui8IapState = IAP_REQUEST_IRCOMM_LSAP;
          break;
        }
      }
      break;
    case IAP_REQUEST_IRCOMM_LSAP:
      while (stcReceivedFrameStorage.ui8UnackedFrameCount > 0) {
        --stcReceivedFrameStorage.ui8UnackedFrameCount;
        IapDataFrame_t *pstcIapDataFrame =   &(((IrDaFrame_t*)&stcReceivedFrameStorage.arrstcBuffers[stcReceivedFrameStorage.ui8UnackedFrameCount].arrui8Buffer)->stcInformation.stcIapDataFrame);

        memset(&stcIapData, 0, sizeof(IapData_t));
        stcIapData.ui8DataLength = dIapResponseLength(stcReceivedFrameStorage.arrstcBuffers[stcReceivedFrameStorage.ui8UnackedFrameCount].ui8Length);
        ui8Length = ui8IasResponse(pstcIapDataFrame, &stcIapData);


        // If no Error
        if (!ui8Length) {
          ui8Length = 0;
          uint8_t ui8Offset = 0;

          // check if IRCOMM 9 - cooked is there
          while (ui8Offset < stcIapData.stcReceivedParameterList.ui8ParameterSize && !ui8Length) {
            Parameter_t *pstcParameter = (Parameter_t*)(((uint8_t*)stcIapData.stcReceivedParameterList.pstcParameters) + ui8Offset);
            if (pstcParameter->ui8PI == dPiIrCommServiceType && pstcParameter->ui8PL == dPlIrCommServiceType && (pstcParameter->arrui8PV[0]&dPvIrCommServiceType9Wire)) {
              ui8Length = 1;
            }
            ui8Offset += (2 + pstcParameter->ui8PL);
          }

          // If device supports 9-cooked then we request the lsap value
          if (ui8Length) {
            ui8Length = ui8IapGetValueByClass(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, strlen(dServiceIrDaComm), dServiceIrDaComm, strlen(dGeneralAttributeTinyTpLsap), dGeneralAttributeTinyTpLsap);
            ui8Timeout = dLmpTimeout;
            pstcConnection->ui8IapState = IAP_VALIDATE_IRCOMM_LSAP;
          } else {
            ui8Length = ui8IapDisconnect(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, dLmpDisconnectReasonLmInitiatedDisconnect);
            pstcConnection->ui8IapState = IAP_DISCONNECT;
          }
        } else {
          ui8Length = ui8IapDisconnect(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, dLmpDisconnectReasonLmInitiatedDisconnect);
          pstcConnection->ui8IapState = IAP_DISCONNECT;
        }

        ui8Timeout = dLmpTimeout;
      }
      break;
    case IAP_VALIDATE_IRCOMM_LSAP:
      while (stcReceivedFrameStorage.ui8UnackedFrameCount > 0) {
        --stcReceivedFrameStorage.ui8UnackedFrameCount;

        IapDataFrame_t *pstcIapDataFrame =   &(((IrDaFrame_t*)&stcReceivedFrameStorage.arrstcBuffers[stcReceivedFrameStorage.ui8UnackedFrameCount].arrui8Buffer)->stcInformation.stcIapDataFrame);
        memset(&stcIapData, 0, sizeof(IapData_t));
        stcIapData.ui8DataLength = dIapResponseLength(stcReceivedFrameStorage.arrstcBuffers[stcReceivedFrameStorage.ui8UnackedFrameCount].ui8Length);
        ui8Length = ui8IasResponse(pstcIapDataFrame, &stcIapData);

        vPassDebugDwordToPc(stcIapData.ui8Data);
        // Check if lsap has arrived
        if (!ui8Length && stcIapData.ui8Data != 0) {
          ui8Length = ui8IapDisconnect(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, dLmpDisconnectReasonLmInitiatedDisconnect);
          pstcConnection->ui8Result = (uint8_t)(stcIapData.ui8Data);
          pstcConnection->ui8IapState = IAP_DISCONNECT;
          *pui8Status = dIapDone;

        } else {
          ui8Length = ui8IapDisconnect(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcConnection, dLmpDisconnectReasonLmInitiatedDisconnect);
          pstcConnection->ui8IapState = IAP_DISCONNECT;
        }
      }
      break;
    case IAP_DISCONNECT:
      *pui8Status = dIapTimedOut;
      break;
  }

  return ui8Length;
}


/********************************************

  Lmp Service Layer of IrDa, Station Control

*********************************************/
uint8_t ui8IrDaLmMultiplexer(Device_t *pstcDevice) {
  uint8_t ui8Length;
  uint8_t ui8Status;

  ui8Length = 0;

  /* State Machine Lmp */
  switch (pstcDevice->ui8LmMuxState) {
    case LMMUX_INIT:
      if (pstcDevice->ui8IrCommServiceDlsap != 0) {
        pstcCreateConnection(pstcDevice, pstcDevice->ui8IrCommServiceDlsap, ui8IrDaCommService);
      } else {
        pstcCreateConnection(pstcDevice, dIapDlsap, ui8IrDaIapDiscoveryService);
      }
      pstcDevice->ui8LmMuxState = LMMUX_ACTIVE;
    case LMMUX_ACTIVE:
      ui8Length = ((uint8_t (*)(Connection_t*, uint8_t*))pstcDevice->pstcConnection->pvServiceFunction)(pstcDevice->pstcConnection, &ui8Status);
      if (ui8Status == dIapDone) {
        pstcDevice->ui8IrCommServiceDlsap = pstcDevice->pstcConnection->ui8Result;
        ui8DeleteConnection(pstcDevice, pstcDevice->pstcConnection);
        pstcDevice->ui8LmMuxState = LMMUX_INIT;
      }
      break;
    default:
      break;
  }

  return ui8Length;

}
/********************************************

  Lap Service Layer of IrDa

*********************************************/
void vIrDaLap(uint32_t ui32TimeDelta) {
  static LapData_t stcLapData = {0};
  static int32_t i32SpecialTimer = 0;
  static int32_t i32FbitTimer = 0;
  static int32_t i32PbitTimer = 0;
  static uint8_t ui8CurrentXidSlotNumber = 0;
  static uint8_t ui8RetryCount = 0;
  static uint8_t ui8Request = 0;
  static Device_t *pstcSecondaryDevice;
  uint32_t ui32TempDeviceAddress;

  // Stats
  stcIrDa.stcStatistics.ui32UpTime += ui32TimeDelta;

  // Update timers
  i32SpecialTimer = (i32SpecialTimer - (int32_t)ui32TimeDelta) > 0 ? i32SpecialTimer - ui32TimeDelta : 0;
  i32FbitTimer    = (i32FbitTimer -    (int32_t)ui32TimeDelta) > 0 ? i32FbitTimer - ui32TimeDelta : 0;
  i32PbitTimer    = (i32PbitTimer -    (int32_t)ui32TimeDelta) > 0 ? i32PbitTimer - ui32TimeDelta : 0;

  // Receive a frame and generate indication
  // Check if PFbit set where frame indication is parsed
  //----------------------------------------
  if (stcIrDaReceiveBuffer.ui8Length > 0) {
    ui8LapResponse(&stcIrDa, &stcIrDaReceiveBuffer, &stcLapData);
  } else {
    memset((uint8_t *)&stcLapData, 0, sizeof(stcLapData));  // Reset lapData before continuing
  }


  // Lap State machine for primary device
  //-------------------------------------------
  if (!stcIrDa.ui8JustListen) {
    switch (stcIrDa.ui8LapState) {
      case LAP_NDM:
        stcIrDa.ui8xBof = dXBOF;
        if (stcLapData.ui16Indication) {
          if (stcLapData.ui16Indication == dIndicSuperRr || stcLapData.ui16Indication == dIndicSuperRej)
            dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapDiscUnknown(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, stcLapData.pstcIrDaFrame->stcLapHeader.ui8ConnectAddress));
          // Broadcast a Frame
        } else if (!stcIrDa.ui16MediaBusyFlagTimeout) {

          if (ui8GetDevicesCount(&stcIrDa.stcPrimaryDevice) > 0 && !i32FbitTimer) {
            if ((pstcSecondaryDevice = pstcGetDeviceByIndex(&stcIrDa.stcPrimaryDevice, 0)) != NULL) {
              pstcSecondaryDevice->ui8ConnectAddress = dLapGenerateNewConnectAddress();   // Generate new address
              dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapSnrm(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
              i32FbitTimer = dFTimerTimeout; // Set F-Timer
              stcIrDa.ui8LapState = LAP_SETUP;
            } else {
              vRemoveAllDevices(&stcIrDa.stcPrimaryDevice);
            }
          } else if (!i32SpecialTimer) {
            //vPassDebugDwordToPc(ui8GetDevicesCount(&stcIrDa.stcPrimaryDevice));
            i32SpecialTimer = dSlotTimerPeriod;

            // Send the first frame
            ui8CurrentXidSlotNumber = dLapXidSlotNumberInit;
            dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapXid(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, ui8CurrentXidSlotNumber));

            stcIrDa.stcStatistics.ui8DiscoverySequences++;
            stcIrDa.ui8LapState = LAP_QUERY;
          }
        }
        break;
      case LAP_QUERY:
        if (stcLapData.ui16Indication == dIndicXid) {
          // Only do something if the destination matches this device
          if (stcIrDa.stcPrimaryDevice.ui32DeviceAddress == stcLapData.pstcIrDaFrame->stcInformation.stcXid.ui32DeviceDestinationAddress) {
            ui32TempDeviceAddress = stcLapData.pstcIrDaFrame->stcInformation.stcXid.ui32DeviceSourceAddress;
            if ((pstcSecondaryDevice = pstcFindDevice(&stcIrDa.stcPrimaryDevice, ui32TempDeviceAddress)) == NULL) {
              if ((stcLapData.ui8InformationLength - 12) >= 0) {
                pstcSecondaryDevice = pstcAddDevice(&stcIrDa.stcPrimaryDevice, ui32TempDeviceAddress, &stcLapData.pstcIrDaFrame->stcInformation.arrui8Sdu[12], stcLapData.ui8InformationLength - 12);
              }
            }
          }
        } else if (!i32SpecialTimer && !stcIrDa.ui16MediaBusyFlagTimeout) {
          i32SpecialTimer = dSlotTimerPeriod;

          // Final Slot reached
          if (++ui8CurrentXidSlotNumber == stcIrDa.ui8NumberOfSlots) {
            ui8CurrentXidSlotNumber = dLapXidSlotNumberEnd;
            i32FbitTimer = dFTimerTimeout; // Set F-Timer
            i32SpecialTimer = dDiscoveryPeriod;
            stcIrDa.ui8LapState = LAP_NDM;
          }
          dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapXid(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, ui8CurrentXidSlotNumber));
        }
        break;

      // Try to Connect to the first device
      case LAP_SETUP:
        if (stcLapData.ui16Indication) {
          if (pstcSecondaryDevice->ui8ConnectAddress == stcLapData.pstcIrDaFrame->stcLapHeader.ui8ConnectAddress) {
            switch (stcLapData.ui16Indication) {
              case dIndicSnrm:
                // Send Request response to set the Nr and Ns: confirmed with Rr
                dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
                i32FbitTimer = dFTimerTimeout; // Set F-Timer
                stcIrDa.ui8LapState = LAP_NRM;
                break;
              case dIndicRnrm:
                // TODO implement request normal response mode
                break;
            }
          }
          // F-Timer expired, we remove the device and start anew
        } else if (!i32FbitTimer) {
          // Disconnect Indication here
          vRemoveAllDevices(&stcIrDa.stcPrimaryDevice);
          i32FbitTimer = dFTimerTimeout;
          i32SpecialTimer = dDiscoveryPeriod;
          stcIrDa.ui8LapState = LAP_NDM;
        }
        break;
      // Transmit
      case LAP_XMIT:
        // Build Frame here
        if (ui8Request) {
          switch (ui8Request) {
            case dRequestDisc:
              dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapDisc(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
              ui8RetryCount = 0;
              i32FbitTimer = dFTimerTimeout;
              stcIrDa.ui8LapState = LAP_PCLOSE;
              break;
            case dRequestData:
              // Queue Comm Data here

              break;
          }
          ui8Request = 0;
        } else if (!i32PbitTimer) {
          // Send NRM
          if (!stcIrDa.ui8RemoteBusy) {
            if (stcUnackedFrameStorage.ui8UnackedFrameCount > 0) {
              vPassDebugDwordToPc(0xCCCCCCCC);
              memcpy(&stcIrDaTransmitBuffer, &stcUnackedFrameStorage.arrstcBuffers[stcUnackedFrameStorage.ui8UnackedFrameCount - 1], sizeof(Buffer_t));
            } else {

              uint8_t ui8Length  = ui8IrDaLmMultiplexer(pstcSecondaryDevice);

              if (ui8Length != 0) {
                dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8Length);

                stcIrDa.ui8As = (stcIrDa.ui8Vs);
                stcIrDa.ui8Vs = (stcIrDa.ui8Vs + 1)&dLapMaxSequenceNumber;

                // TODO generate error if this exceeds
                // Save Buffer until acknowledged
                if (stcUnackedFrameStorage.ui8UnackedFrameCount < 7) {
                  memcpy(&stcUnackedFrameStorage.arrstcBuffers[stcUnackedFrameStorage.ui8UnackedFrameCount], &stcIrDaTransmitBuffer, sizeof(Buffer_t));

                  ++stcUnackedFrameStorage.ui8UnackedFrameCount;
                }

              } else if (stcIrDa.ui8LocalBusy) {
                // Default response if no cases matched
                dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRnr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
              } else {
                dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
              }
            }
          } else if (stcIrDa.ui8LocalBusy) {
            // Default response if no cases matched
            dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRnr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
          } else {
            vPassDebugDwordToPc(0xFFFF);
            dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
          }

          i32FbitTimer = dFTimerTimeout;
          stcIrDa.ui8LapState = LAP_NRM;
        }
        break;
      case LAP_NRM:
        if (stcLapData.ui16Indication) {
          // Pass Frames to NRM state only if these have correct Connect Address
          if (pstcSecondaryDevice->ui8ConnectAddress == stcLapData.pstcIrDaFrame->stcLapHeader.ui8ConnectAddress) {

            // If last frame is a command frame, disconnect
            if (stcIrDa.ui16LapErrorFlags & dLapErrorCommandFrame) {
              // TODO Apply default connection parameters in the future
              // TODO disconnect indication
              i32FbitTimer = dFTimerTimeout;
              stcUnackedFrameStorage.ui8UnackedFrameCount = 0;
              stcReceivedFrameStorage.ui8UnackedFrameCount = 0;
              i32SpecialTimer = dDiscoveryPeriod;
              stcIrDa.ui8LapState = LAP_NDM;
            } else {

              //vPassDebugDwordToPc(stcLapData.ui16Indication);
              ui8RetryCount = 0;

#define dInvalidNr (!(dCompareToControlNr(stcLapData.pstcIrDaFrame->stcLapHeader.ui8Control, stcIrDa.ui8Vs) || (stcUnackedFrameStorage.ui8UnackedFrameCount && (dCompareToControlNr(stcLapData.pstcIrDaFrame->stcLapHeader.ui8Control,stcIrDa.ui8As)))))

              // TODO handle errors differently in the future. ADD Support for multiple frames
              switch (stcLapData.ui16Indication) {
                case dIndicInfo:
                  // Process Infos using LMP Layer
                  if (!stcIrDa.ui8LocalBusy) {
                    if (dInvalidNr) {
                      //vPassDebugDwordToPc(stcIrDa.ui8Vs);
                      dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapDisc(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
                      i32FbitTimer = dFTimerTimeout;
                      stcIrDa.ui8LapState = LAP_PCLOSE;
                    } else {

                      //vPassDebugDwordToPc(0xDEADBEEF);
                      // Process NR
                      if (dCompareToControlNr(stcLapData.pstcIrDaFrame->stcLapHeader.ui8Control, stcIrDa.ui8Vs) && (stcUnackedFrameStorage.ui8UnackedFrameCount > 0)) {
                        // TODO Put Frame on Front of the queue <here>
                        --stcUnackedFrameStorage.ui8UnackedFrameCount;
                      }

                      if (dCompareToControlNs(stcLapData.pstcIrDaFrame->stcLapHeader.ui8Control, stcIrDa.ui8Vr)) {
                        stcIrDa.ui8Vr = (stcIrDa.ui8Vr + 1) & dLapMaxSequenceNumber;

                        if (stcReceivedFrameStorage.ui8UnackedFrameCount < 7) {
                          memcpy(&stcReceivedFrameStorage.arrstcBuffers[stcReceivedFrameStorage.ui8UnackedFrameCount], &stcIrDaReceiveBuffer, sizeof(Buffer_t));
                          ++stcReceivedFrameStorage.ui8UnackedFrameCount;
                        }
                      }
                    }
                     i32PbitTimer = 0;  // Just received an I-Frame no need to wait. Works only if the frame had the P-flag set
                  }else{
                     i32PbitTimer = dPTimerTimeout;  
                  }

                  //vPassDebugDwordToPc(0xBABA);
            
                  stcIrDa.ui8LapState = LAP_XMIT;
                  break;
                case dIndicSuperRnr:
                case dIndicSuperRr:
                case dIndicSuperRej:
                case dIndicSuperSrej:
                  if (dInvalidNr) {
                    vPassDebugDwordToPc(0xAAAAA);
                    dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapDisc(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
                    i32FbitTimer = dFTimerTimeout;
                    stcIrDa.ui8LapState = LAP_PCLOSE;
                  } else {
                    // handle in xmit
                    // Rnr -> remotebusy
                    // Rr -> !remotebusy
                    // rrej,srej -> retransmit
                    if (stcLapData.ui16Indication == dIndicSuperRnr) {
                      stcIrDa.ui8RemoteBusy = 1;
                    } else {
                      stcIrDa.ui8RemoteBusy = 0;
                    }

                    // Process NR
                    if (dCompareToControlNr(stcLapData.pstcIrDaFrame->stcLapHeader.ui8Control, stcIrDa.ui8Vs) && (stcUnackedFrameStorage.ui8UnackedFrameCount > 0)) {
                      // TODO Put Frame on Front of the queue <here>
                      --stcUnackedFrameStorage.ui8UnackedFrameCount;
                    }

                    //vPassDebugDwordToPc(0xDEAD);
                    i32PbitTimer = dPTimerTimeout;
                    stcIrDa.ui8LapState = LAP_XMIT;
                  }
                  break;
                case dIndicRd:
                  dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapDisc(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
                  i32FbitTimer = dFTimerTimeout;
                  stcIrDa.ui8LapState = LAP_PCLOSE;
                  break;
                // Errors cause device to disconnect, TODO handle this differently in future
                case dIndicFrmr:
                  dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapDisc(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
                  i32FbitTimer = dFTimerTimeout;
                  stcIrDa.ui8LapState = LAP_PCLOSE;
                  break;
                default:
                  if (stcIrDa.ui8LocalBusy) {
                    // Default response if no cases matched
                    dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRnr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
                  } else {
                    dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
                  }
                  i32FbitTimer = dFTimerTimeout;
                  break;
              }
            }
          }
        } else if (!i32FbitTimer) {
          if (++ui8RetryCount >= dMaxRetryCount3s) {
            vRemoveAllDevices(&stcIrDa.stcPrimaryDevice);
            ui8RetryCount = 0;
            i32FbitTimer = dFTimerTimeout;
            i32SpecialTimer = dDiscoveryPeriod;
            stcIrDa.ui8Vs = 0;
            stcIrDa.ui8Vr = 0;
            stcIrDa.ui8LapState = LAP_NDM;
            stcUnackedFrameStorage.ui8UnackedFrameCount = 0;
            stcReceivedFrameStorage.ui8UnackedFrameCount = 0;
          } else {
            //vPassDebugDwordToPc(0xCAFE);
            i32FbitTimer = dFTimerTimeout;
            // TODO Apply Default Parameters
            if (stcIrDa.ui8LocalBusy) {
              // Default response if no cases matched
              dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRnr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
            } else {
              dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapRr(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
            }
          }
        }
        break;

      // Receive Disc
      case LAP_PCLOSE:
        // Wait for Disc confirmation
        if (stcLapData.ui16Indication) {
          switch (stcLapData.ui16Indication) {
            case dIndicDisc:
            case dIndicRd:
              i32SpecialTimer = dDiscoveryPeriod;   // Note: Although in the Standard it says Disc confirmation is a Ua Frame, real devices sometimes use Rd
              i32FbitTimer = dFTimerTimeout;
              stcIrDa.ui8Vs = 0;
              stcIrDa.ui8Vr = 0;
              stcIrDa.ui8LapState = LAP_NDM;
              break;
          }
        } else if (!i32FbitTimer) {
          if (++ui8RetryCount >= dMaxRetryCount) {
            vRemoveAllDevices(&stcIrDa.stcPrimaryDevice);
            ui8RetryCount = 0;
            i32FbitTimer = dFTimerTimeout;
            i32SpecialTimer = dDiscoveryPeriod;
            stcIrDa.ui8Vs = 0;
            stcIrDa.ui8Vr = 0;
            stcIrDa.ui8LapState = LAP_NDM;
            stcUnackedFrameStorage.ui8UnackedFrameCount = 0;
            stcReceivedFrameStorage.ui8UnackedFrameCount = 0;
          } else {
            i32FbitTimer = dFTimerTimeout;
            dMarkBufferForTransmission(stcIrDaTransmitBuffer, ui8LapDisc(&stcIrDa, stcIrDaTransmitBuffer.arrui8Buffer, pstcSecondaryDevice));
          }
        }
        break;
      default:
        break;
    }
  }


  dMarkBufferAsReceived(stcIrDaReceiveBuffer);

  // Error handling
  //-----------------------------
  if (stcIrDa.ui16SirErrorFlags || stcIrDa.ui16LapErrorFlags) {
    stcIrDa.stcStatistics.ui8LatchedCommand = stcIrDaTransmitBuffer.arrui8Buffer[1];
    stcIrDa.stcStatistics.ui32LatchedError = stcIrDa.ui16SirErrorFlags | ((uint32_t)stcIrDa.ui16LapErrorFlags) << 16;
    stcIrDa.stcStatistics.ui8ErrorCount++;
    stcIrDa.ui16SirErrorFlags = 0;
    stcIrDa.ui16LapErrorFlags = 0;
  }
}




/*********************************

  IrDa Wrapper

*********************************/
inline void vIrDaSirWrapper(uint32_t ui32TimeDelta) {
#define dPassDebugErrorFrame() {stcIrDaReceiveBuffer.ui8Length = stcIrDaReceiveBuffer.ui8BytesWritten;vPassIrDaFrameToPc(&stcIrDaReceiveBuffer,1);stcIrDaReceiveBuffer.ui8Length = 0;}

  static uint8_t ui8SirTxState = dTxEncoderWait;
  static uint8_t ui8SirRxState = dRxDecoderWait;
  static uint8_t ui8Echo = 0;
  uint8_t ui8AvailableTx;
  uint8_t ui8AvailableRx;
  uint8_t ui8WritingDone;
  uint8_t ui8ReadingDone;
  uint8_t ui8Data;

  // Media Busy Timeout
  // if it times out in one of the states, then there is an error
  //-----------------------------------------------
  if (stcIrDa.ui16MediaBusyFlagTimeout > ui32TimeDelta) {
    stcIrDa.ui16MediaBusyFlagTimeout -= ui32TimeDelta;
  } else {
    stcIrDa.ui16MediaBusyFlagTimeout = 0;

    // if we are not transmitting, or are done transmitting and RX-Statemachine stuck
    if ((ui8SirRxState != dRxDecoderWait)) {
      stcIrDa.ui16SirErrorFlags |= dSirErrorMediaTimedOut;
      ui8SirRxState = dRxDecoderWait;
      ui8Echo = 0;
    }
  }


  // Do this till data is transfered or buffer is full
  ui8ReadingDone = 0;
  ui8WritingDone = 0;
  do {

    // There is data available. parse it.
    ui8AvailableRx = dIrDaSirDataAvailable();
    if (ui8AvailableRx > 0) {
      ui8Data = dIrDaSirRead();

      //dPcComWrite(ui8Data);
      switch (ui8SirRxState) {
        case dRxDecoderWait:
          if (ui8Data == dBOFFF || ui8Data == dBOF) {
            // Block the transmitter if transmitter is not already transmitting data. which would mean that data either collided or it's own data

            stcIrDa.ui16MediaBusyFlagTimeout  = dMediaBusyTimeout;

            // Check for  Hardware errors only once, before starting. Any other errors will be set in the states
            if (UCSR1A & 0x18) {
              stcIrDa.ui16SirErrorFlags = (UCSR1A & 0x08 != 0) ? (stcIrDa.ui16SirErrorFlags | dSirErrorOverRun) : (UCSR1A & 0x10 != 0) ? (stcIrDa.ui16SirErrorFlags | dSirErrorOverRun) : (stcIrDa.ui16SirErrorFlags);
              cli();
              UCSR1A &= 0xE7;
              sei();
            }

            dResetBuffer(stcIrDaReceiveBuffer);
            if (ui8Data == dBOFFF) {
              ++stcIrDaReceiveBuffer.ui8BytePreamble;    // FF cannot be the last BOF
              ui8SirRxState = dRxDecoderPreambleFF;
            } else {
              ui8SirRxState = dRxDecoderPreambleC0;
            }
          }
          break;
        case dRxDecoderPreambleC0:
          // C0 Bofs, wait for it to be
          if (ui8Data != dBOF) {
            if (ui8Data == dSTF) {
              ui8SirRxState = dRxDecoderUnescape;
            } else if (ui8Data == dEOF) {
              stcIrDa.ui16SirErrorFlags |=  dSirErrorBofFormat;  // what to do here?
              ui8SirRxState = dRxDecoderWait;
              dPassDebugErrorFrame()
            } else {
              stcIrDaReceiveBuffer.arrui8Buffer[stcIrDaReceiveBuffer.ui8BytesWritten++] = ui8Data;
              ui8SirRxState = dRxDecoderData;
            }
          } else {
            ++stcIrDaReceiveBuffer.ui8BytePreamble;
          }

          // Too many bofs
          if (stcIrDaReceiveBuffer.ui8BytePreamble > stcIrDa.ui8xBof) {
            stcIrDa.ui16SirErrorFlags |= dSirErrorPreamble;
            ui8SirRxState = dRxDecoderWait;
            dPassDebugErrorFrame()
          }
          break;
        // Easy with FF filled extras
        case dRxDecoderPreambleFF:
          // Last Bof is a C0. Specification requires 10 Bofs. Old specification required 5 bofs
          if (ui8Data == dBOF) {
            ui8SirRxState = dRxDecoderData;
          } else if (ui8Data != dBOFFF) {
            stcIrDa.ui16SirErrorFlags |=  dSirErrorBofFormat;  // what to do here?
            ui8SirRxState = dRxDecoderWait;
          } else {
            ++stcIrDaReceiveBuffer.ui8BytePreamble;
          }

          // Too many bofs
          if (stcIrDaReceiveBuffer.ui8BytePreamble > stcIrDa.ui8xBof) {
            stcIrDa.ui16SirErrorFlags |= dSirErrorPreamble;
            ui8SirRxState = dRxDecoderWait;
            dPassDebugErrorFrame()
          }
          break;
        case dRxDecoderData:
          if (ui8Data == dSTF) {
            ui8SirRxState = dRxDecoderUnescape;
          } else if (ui8Data == dEOF) {
            stcIrDaReceiveBuffer.ui8Length = stcIrDaReceiveBuffer.ui8BytesWritten;

            ui8Data = ui8VerifyFrame(&stcIrDa, &stcIrDaReceiveBuffer); // Save the verification status here
            vPassIrDaFrameToPc(&stcIrDaReceiveBuffer, 0);                // For debugging, paass the frame with all the errors


            // Verify Frame here. If it does not confirm to minimum requirement of 4 Byte and a valid CRC discard it
            if (ui8Data) {

              stcIrDaReceiveBuffer.ui8Length = 0;
              stcIrDaReceiveBuffer.ui8BytesWritten = 0;
            } else {
              // If Checksums match it is own frame, otherwise the frame will get tossed during Lap reception
              if (ui8Echo) {
                stcIrDa.stcStatistics.ui32TotalEchoed++;
                stcIrDaReceiveBuffer.ui8Length = 0;
                stcIrDaReceiveBuffer.ui8BytesWritten = 0;

              } else {
                stcIrDa.stcStatistics.ui32TotalReceived++;
              }
            }

            if (ui8Echo) {
              --ui8Echo;
            }

            // End reception
            stcIrDa.ui16MediaBusyFlagTimeout = 0;
            ui8SirRxState = dRxDecoderWait;
            ui8ReadingDone = 1;



            // An unescaped character cannot be present
          } else if (ui8Data == dBOF) {
            stcIrDa.ui16SirErrorFlags |= dSirErrorFrameFormat;
            dPassDebugErrorFrame()

            // Check if Buffer is full
          } else if (stcIrDaReceiveBuffer.ui8BytesWritten == dBufferSize) {
            stcIrDa.ui16SirErrorFlags |= dSirErrorBufferOverrun;
            dPassDebugErrorFrame()

          } else {
            stcIrDaReceiveBuffer.arrui8Buffer[stcIrDaReceiveBuffer.ui8BytesWritten++] = ui8Data;
          }
          break;
        case dRxDecoderUnescape:
          stcIrDaReceiveBuffer.arrui8Buffer[stcIrDaReceiveBuffer.ui8BytesWritten++] = ui8Data ^ 0x20;
          ui8SirRxState = dRxDecoderData;
          break;
      }
    } else {
      ui8ReadingDone = 1;
    }

    // TX >> Transmit only if there is no activity on the media
    //----------------------------------------------------+
    ui8AvailableTx = dIrDaSirRoom();
    if (ui8AvailableTx) {
      switch (ui8SirTxState) {
        // Wait for available space to write the bofs and the new frame
        case dTxEncoderWait:
          if (!stcIrDa.ui16MediaBusyFlagTimeout && (stcIrDaTransmitBuffer.ui8Length != stcIrDaTransmitBuffer.ui8BytesWritten) && ui8AvailableTx >= (stcIrDa.ui8xBof)) {
            ui8SirTxState = dTxEncoderPreamble;
          } else {
            ui8WritingDone = 1;
          }
          break;
        case dTxEncoderPreamble:
          // ui8xbof is 10 NDM and negotiated in NRM. Order Little Endian
          for (uint8_t i = 0; i < stcIrDa.ui8xBof - 1; i++) {
            dIrDaSirWrite(dBOFFF); // According to standard, it is better to send 0xFF's
          }
          dIrDaSirWrite(dBOF);    // Final BOF should be 0xC0
          ui8SirTxState = dTxEncoderData;               // All Bofs have been sent, proceed to data
          break;
        case dTxEncoderData:
          if (stcIrDaTransmitBuffer.ui8BytesWritten != stcIrDaTransmitBuffer.ui8Length) {
            ui8Data = stcIrDaTransmitBuffer.arrui8Buffer[stcIrDaTransmitBuffer.ui8BytesWritten];
            if ((ui8Data == dBOF || ui8Data == dEOF || ui8Data == dSTF)) {
              if (ui8AvailableTx < 2) {
                ui8WritingDone = 1;       // Transmitter is waiting for memory space
              } else {
                dIrDaSirWrite(dSTF);
                dIrDaSirWrite(ui8Data ^ 0x20);
                ++stcIrDaTransmitBuffer.ui8BytesWritten;
              }
            } else {
              dIrDaSirWrite(ui8Data);
              ++stcIrDaTransmitBuffer.ui8BytesWritten;
            }
          } else {

            ++ui8Echo;
            dIrDaSirWrite(dEOF);
            ++stcIrDa.stcStatistics.ui32TotalTransmitted;

            // Done
            ui8SirTxState = dTxEncoderWait;
            ui8WritingDone = 1;
          }
          break;
      }
    } else {
      ui8WritingDone  = 1;
    }
  } while (!ui8ReadingDone || !ui8WritingDone); // Loop as long as there is space in the buffer or new data in the buffer


  vIrDaLap(ui32TimeDelta);

}



/*************************
  PC Control
**************************/
inline void vControl(uint32_t ui8TimeDelta) {
  static uint16_t ui16Command = STATE_WAIT;
  static uint16_t ui16Timeout = 0;
  static uint8_t ui8FrameLength = 0;

  uint8_t ui8Temp;
  uint16_t ui16Temp;
  uint8_t *pui8Temp;
  uint8_t ui8Available;

  // Pc Communication: read
  ui8Available = dPcComDataAvailable();
  if (ui8Available > 0) {
    if (ui8Available < dGetCircularBufferCredit(stcPcReceiveCircularBuffer)) {
      dReadIntoCircularBuffer(stcPcReceiveCircularBuffer, ui8Available, dPcComRead);
    } else {
      while (ui8Available--) {
        dPcComRead();
      }
    }
  }

  // Parse Command
  ui8Available = dGetCircularBufferUsedLength(stcPcReceiveCircularBuffer);

  // Timeout conditions. If Length >0 or Command was set
  if ((ui8Available > 0 || ui16Command != STATE_WAIT) && ((ui16Timeout += ui8TimeDelta) >= dPcCommTimeout)) {
    ui16Command = STATE_TIMEOUT;
  }

  // cmd
  switch (ui16Command) {
    case CMD_DEBUG:
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 5);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_DEBUG);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_DEBUG >> 8);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 0);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, stcIrDa.ui8LapState);
      ui16Command = STATE_CLEANUP;
      break;
    case CMD_START_DISCOVERY:
      vRemoveAllDevices(&stcIrDa.stcPrimaryDevice);
      stcIrDa.ui8LapState = LAP_NDM;
      ui16Command = STATE_CLEANUP;
      break;
    case CMD_CALCULATE_CRC:
      if ((ui8FrameLength - 3) <= ui8Available) {
        pui8Temp = malloc(sizeof(ui8FrameLength - 3));
        dGetFromCircularBuffer(stcPcReceiveCircularBuffer, pui8Temp, ui8FrameLength - 3);
        ui16Temp = ui16CalculateFcs(pui8Temp, ui8FrameLength - 3);
        free(pui8Temp);

        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 6);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_CALCULATE_CRC);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_CALCULATE_CRC >> 8);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 0);

        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, ui16Temp);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, ui16Temp >> 8);
        ui16Command = STATE_CLEANUP;
      }
      break;
    case CMD_GET_STATISTIC:
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 27);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_GET_STATISTIC);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_GET_STATISTIC >> 8);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 0);
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui32TotalTransmitted), 4)
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui32TotalReceived), 4)
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui32TotalEchoed), 4)
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui32UpTime), 4)
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui8DiscoverySequences), 1)
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui32LatchedError), 4)
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui8ErrorCount), 1)
      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, ((uint8_t*)&stcIrDa.stcStatistics.ui8LatchedCommand), 1)
      ui16Command = STATE_CLEANUP;
      break;
    case CMD_GET_DEVICES:
      pui8Temp = (uint8_t*)malloc(dCircularBufferSize);
      pui8Temp[0] = 5;
      pui8Temp[1] = CMD_GET_DEVICES;
      pui8Temp[2] = CMD_GET_DEVICES >> 8;
      pui8Temp[3] = 0x00;
      pui8Temp[4] = ui8GetDevicesCount(&stcIrDa.stcPrimaryDevice);

      // Put Names into Buffer
      for (int i = 0; i < pui8Temp[4]; i++) {
        ui8Temp = ui8PutDevicenameIntoBuffer(&stcIrDa.stcPrimaryDevice, pui8Temp + pui8Temp[0], i);
        pui8Temp[0] += ui8Temp;
      }

      dPutIntoCircularBuffer(stcPcTransmitCircularBuffer, pui8Temp, pui8Temp[0])
      free(pui8Temp);
      ui16Command = STATE_CLEANUP;
      break;
    case CMD_RESET_STATS:
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 4);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_RESET_STATS);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_RESET_STATS >> 8);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 0);

      memset(&stcIrDa.stcStatistics, 0, sizeof(Statistics_t));

      UCSR1A = UCSR1A & 0xE7;   // Reset uart errors
      stcIrDa.ui16LapErrorFlags = 0;
      stcIrDa.ui16SirErrorFlags = 0;
      stcIrDa.ui16MediaBusyFlagTimeout = 0;
      ui16Command = STATE_CLEANUP;
      break;
    case CMD_IRDA_PARAM:
      if ((ui8FrameLength - 3) <= ui8Available) {
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 4);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_IRDA_PARAM);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_IRDA_PARAM >> 8);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 0);

        stcIrDa.ui8JustListen = stcPcReceiveCircularBuffer.arrui8Buffer[3];
        ui16Command = STATE_CLEANUP;
      }
      break;
    case CMD_IRDA_DISC:
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 4);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_IRDA_DISC);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_IRDA_DISC >> 8);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 0);

      stcIrDa.ui8JustListen = stcPcReceiveCircularBuffer.arrui8Buffer[3];
      ui16Command = STATE_CLEANUP;
      // TODO add flags here
      break;
    case CMD_IRCOMM_PAYLOAD:
      if ((ui8FrameLength - 3) <= ui8Available) {

        dGetFromCircularBuffer(stcPcReceiveCircularBuffer, arrui8IrCommPayload, ui8FrameLength - 3);

        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 4);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_IRCOMM_PAYLOAD);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_IRCOMM_PAYLOAD >> 8);
        dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, arrui8IrCommPayload[0]);

        ui16Command = STATE_CLEANUP;
      }

      // TODO add flags here
      break;
    case STATE_WAIT:
      if (ui8Available > 1) {
        dGetFromCircularBuffer(stcPcReceiveCircularBuffer, ((uint8_t*)&ui8FrameLength), 1);

        if (ui8FrameLength > dCircularBufferSize) {
          ui16Command  = STATE_CLEANUP;
        } else {
          ui16Command  = STATE_COMMAND;
        }
      }
    case STATE_COMMAND:
      if (ui8Available >= 2) {
        dGetFromCircularBuffer(stcPcReceiveCircularBuffer, ((uint8_t*)&ui16Command), 2)

        // Sanity check
        if (ui16Command >= STATE_WAIT) {
          ui16Command = STATE_CLEANUP;
        }
      }
      break;
    case STATE_TIMEOUT:
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 4);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_STATUS);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, CMD_STATUS >> 8);
      dPutByteIntoCircularBuffer(stcPcTransmitCircularBuffer, 1);
      dClearCircularBuffer(stcPcReceiveCircularBuffer);
    case STATE_CLEANUP:
    default:
      ui16Command = STATE_WAIT;
      ui16Timeout = 0;
      break;
  }

  // Pc Communication. write
  ui8Available = dGetCircularBufferUsedLength(stcPcTransmitCircularBuffer);
  dWriteFromCircularBuffer(stcPcTransmitCircularBuffer, ui8Available, dPcComWrite); /**/
}



/*********************************
  Init

********************************/
void setup() {
  dPcComInit(115200);
  vSetHSDLBaud(dPvLapBaud9600bps);

  // Reset structures
  memset((uint8_t*)&stcPcTransmitCircularBuffer, 0, sizeof(stcPcTransmitCircularBuffer));
  memset((uint8_t*)&stcPcReceiveCircularBuffer, 0, sizeof(stcPcReceiveCircularBuffer));
  memset((uint8_t*)&stcIrDaReceiveBuffer, 0, sizeof(stcIrDaReceiveBuffer));
  memset((uint8_t*)&stcIrDaTransmitBuffer, 0, sizeof(stcIrDaTransmitBuffer));
  memset((uint8_t*)&stcIrDa, 0, sizeof(stcIrDa));

  // Set Random
  ui32gCurrentTime = millis();
  randomSeed(ui32gCurrentTime);

  // Define IAS and Lap Connect Parameters
  vInitParameters(&stcIrDa);
}

/**************************************
  Loop forever. ~1ms per exec.

**************************************/
void loop() {
  ui32sLastTime = ui32gCurrentTime;
  ui32gCurrentTime = millis();

  vControl(ui32gCurrentTime - ui32sLastTime);
  vIrDaSirWrapper(ui32gCurrentTime - ui32sLastTime);
}
