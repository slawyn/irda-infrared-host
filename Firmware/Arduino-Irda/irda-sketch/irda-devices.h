#ifndef IRDA_DEVICES
#define IRDA_DEVICES


typedef struct Connection_t {
  uint8_t ui8ConnectAddress;
  uint8_t ui8Dlsap;
  uint8_t ui8Slsap;
  uint8_t ui8Credit;
  uint8_t ui8Result;
  union{      // Can only connect to one at a time
      uint8_t ui8IapState;
      uint8_t ui8IrCommState;
  };
  void * pvServiceFunction;
  Connection_t *pstcNextConnection;
} Connection_t __attribute__((packed));

typedef struct Device_t {
  uint32_t ui32DeviceAddress;
  uint8_t ui8ServiceRecordLength;
  uint8_t *pui8ServiceRecord;
  uint8_t ui8ConnectAddress;
  uint8_t ui8ConnectionCount;
  uint8_t ui8IrCommServiceDlsap;
  uint8_t ui8LmMuxState;
  Connection_t *pstcConnection;
  Device_t *pstcNextDevice;
} Device_t __attribute__((packed));

typedef struct {
  uint32_t ui32DeviceAddress;
  uint8_t *pui8ServiceRecord;
  uint8_t ui8ServiceRecordLength;
  Device_t *pstcNextDevice;
} PrimaryDevice_t;

#include "irda-essentials.h"

uint8_t ui8DeleteConnection(Device_t *pstcDevice, Connection_t *pstcConnection){
  uint8_t ui8Index;
  Connection_t *pstcCurrentConnection;
  Connection_t *pstcPreviousConnection;

  ui8Index = pstcDevice->ui8ConnectionCount;
  if(ui8Index == 0 || !pstcDevice->ui8ConnectionCount){
    return 1;
  }else{
    pstcCurrentConnection = pstcDevice->pstcConnection;
    pstcPreviousConnection = NULL;
    while(--ui8Index>0 && pstcCurrentConnection != pstcConnection){
      pstcPreviousConnection = pstcCurrentConnection;
      pstcCurrentConnection = pstcCurrentConnection->pstcNextConnection;
    }

    // Connection found
    if(pstcCurrentConnection == pstcConnection){
        if(pstcPreviousConnection != NULL){
          pstcPreviousConnection->pstcNextConnection = pstcCurrentConnection->pstcNextConnection;
        }
        free(pstcCurrentConnection);
        return 0;
    }else{
      return 2;
    }
  }
}

Connection_t *pstcCreateConnection(Device_t *pstcDevice,uint8_t ui8Dlsap,void *pvServiceFunction){
  uint8_t ui8Slsap;
  uint8_t ui8Index;
  Connection_t *pstcCurrentConnection;

  ui8Index = pstcDevice->ui8ConnectionCount;
  if(ui8Index == 0){
    pstcDevice->pstcConnection = (Connection_t*)malloc(sizeof(Connection_t));
    pstcCurrentConnection = pstcDevice->pstcConnection;
    ui8Slsap = 0;
  }else{
    pstcCurrentConnection = pstcDevice->pstcConnection;
    ui8Slsap = pstcCurrentConnection->ui8Slsap;

    // Find place to insert
    while(--ui8Index>0){
      pstcCurrentConnection = pstcCurrentConnection->pstcNextConnection;
      if((pstcCurrentConnection->ui8Slsap-ui8Slsap)>1){ // Find connections that have a slsap distance of more than 1, meaning some connection was removed between them
        ui8Slsap +=1;
        break;
      }else{
        ui8Slsap = pstcCurrentConnection->ui8Slsap;
      }
    }
    pstcCurrentConnection->pstcNextConnection =  (Connection_t*)malloc(sizeof(Connection_t));
    pstcCurrentConnection = pstcCurrentConnection->pstcNextConnection;
  }

  // Create new Dlsap connection
  ++pstcDevice->ui8ConnectionCount;
  pstcCurrentConnection->ui8Slsap = ui8Slsap; // Take next available
  pstcCurrentConnection->ui8Dlsap = ui8Dlsap;
  pstcCurrentConnection->ui8Credit = 2;
  pstcCurrentConnection->ui8ConnectAddress = pstcDevice->ui8ConnectAddress;
  pstcCurrentConnection->ui8IapState = 0x00;
  pstcCurrentConnection->pvServiceFunction = pvServiceFunction;
  return pstcCurrentConnection;
}


Device_t * pstcGetDeviceByIndex(PrimaryDevice_t *pstcDeviceHead, uint8_t ui8DeviceIndex) {
  uint8_t ui8FlagFound = 0;
  uint8_t ui8LocalIdx = 0;
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  while (pstcTempDevice != NULL) {
    if (ui8LocalIdx == ui8DeviceIndex) {
      ui8FlagFound = 1;
      break;
    }
    pstcTempDevice = pstcTempDevice->pstcNextDevice;
    ++ui8LocalIdx;
  }
  if (ui8FlagFound)
    return pstcTempDevice;
  else
    return NULL;
}

Device_t * pstcAddDevice(PrimaryDevice_t *pstcDeviceHead, uint32_t ui32DeviceAddress, uint8_t *pui8ServiceRecord , uint8_t ui8ServiceRecordLength) {
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;

  if (pstcTempDevice != NULL) {
    while (pstcTempDevice->pstcNextDevice != NULL) {
      pstcTempDevice = pstcTempDevice->pstcNextDevice;
    }

    pstcTempDevice = pstcTempDevice->pstcNextDevice;
    pstcTempDevice = (Device_t*)malloc(sizeof(Device_t));
  } else {
    pstcDeviceHead->pstcNextDevice = (Device_t*)malloc(sizeof(Device_t));
    pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  }


  // Init Device here
  memset(pstcTempDevice,0,sizeof(Device_t));
  pstcTempDevice->ui32DeviceAddress = ui32DeviceAddress;

  // Save serivce record
  if (ui8ServiceRecordLength > 0) {
    if (ui8ServiceRecordLength > dLmpDeviceNicknameMaxLength)
      ui8ServiceRecordLength = dLmpDeviceNicknameMaxLength;

    pstcTempDevice->pui8ServiceRecord = (uint8_t*)malloc(ui8ServiceRecordLength);
    memcpy(pstcTempDevice->pui8ServiceRecord, pui8ServiceRecord, ui8ServiceRecordLength);
  } else {
    ui8ServiceRecordLength = strlen(dLmpDeviceUnknownNickname);
    pstcTempDevice->pui8ServiceRecord = (uint8_t*)malloc(ui8ServiceRecordLength + 1);
    memset(pstcTempDevice->pui8ServiceRecord, 0, ui8ServiceRecordLength);
    memcpy((pstcTempDevice->pui8ServiceRecord + 1), dLmpDeviceUnknownNickname, ui8ServiceRecordLength);
  }

  pstcTempDevice->ui8ServiceRecordLength = ui8ServiceRecordLength;
  return pstcTempDevice;
}

Device_t *pstcFindDevice(PrimaryDevice_t *pstcDeviceHead, uint32_t ui32DeviceAddress) {
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  while (pstcTempDevice != NULL && pstcTempDevice->ui32DeviceAddress != ui32DeviceAddress) {
    pstcTempDevice = pstcTempDevice->pstcNextDevice;
  }
  return pstcTempDevice;
}

/*Remove device from list*/
void vRemoveDevice(PrimaryDevice_t *pstcDeviceHead, uint32_t ui32DeviceAddress) {
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  Device_t *pstcPreviousDevice = NULL;
  while (pstcTempDevice != NULL) {
    if (pstcTempDevice->ui32DeviceAddress == ui32DeviceAddress) {

      // If it has a previous device
      if (pstcPreviousDevice != NULL) {
        pstcPreviousDevice->pstcNextDevice = pstcTempDevice->pstcNextDevice;
      } else {
        pstcDeviceHead->pstcNextDevice = pstcTempDevice->pstcNextDevice;
      }

      if (pstcTempDevice->pui8ServiceRecord != NULL) {
        free(pstcTempDevice->pui8ServiceRecord);
        pstcTempDevice->pui8ServiceRecord = NULL;
      }

      if (pstcTempDevice->pstcConnection != NULL) {
        free(pstcTempDevice->pstcConnection);
        pstcTempDevice->pstcConnection = NULL;
      }

      free(pstcTempDevice);
      break;
    }
    pstcPreviousDevice = pstcTempDevice;
    pstcTempDevice = pstcTempDevice->pstcNextDevice;
  }
}

void vDeleteAllDevices(PrimaryDevice_t *pstcDeviceHead) {
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  Device_t *pstcNextDevice = NULL;

  while (pstcTempDevice != NULL) {
    pstcNextDevice = pstcTempDevice->pstcNextDevice;
    if (pstcTempDevice->pui8ServiceRecord != NULL){
      free(pstcTempDevice->pui8ServiceRecord);
      pstcTempDevice->pui8ServiceRecord = NULL;
    }
    if (pstcTempDevice->pstcConnection != NULL){
      free(pstcTempDevice->pstcConnection);
      pstcTempDevice->pstcConnection = NULL;
    }
    free(pstcTempDevice);

    pstcTempDevice = pstcNextDevice;
  }

  pstcDeviceHead->pstcNextDevice = NULL;
}

uint8_t ui8GetDevicesCount(PrimaryDevice_t *pstcDeviceHead) {
  uint8_t ui8Count = 0;
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  while (pstcTempDevice != NULL) {
    pstcTempDevice = pstcTempDevice->pstcNextDevice;
    ++ui8Count;
  }

  return ui8Count;
}

uint8_t ui8GetTotalDevicenamesLength(PrimaryDevice_t *pstcDeviceHead){
  uint8_t ui8Size = 0;
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  while (pstcTempDevice != NULL) {
    pstcTempDevice = pstcTempDevice->pstcNextDevice;
    ui8Size += pstcTempDevice->ui8ServiceRecordLength;
  }
  return ui8Size;
}

void vPutDevicenamesIntoBuffer(PrimaryDevice_t *pstcDeviceHead, uint8_t* pui8Buf) {
  uint8_t ui8Offset = 0;
  Device_t *pstcTempDevice = pstcDeviceHead->pstcNextDevice;
  while (pstcTempDevice != NULL) {
    pstcTempDevice = pstcTempDevice->pstcNextDevice;
    memcpy(&pui8Buf[ui8Offset], pstcTempDevice->pui8ServiceRecord, pstcTempDevice->ui8ServiceRecordLength);
    ui8Offset += pstcTempDevice->ui8ServiceRecordLength;
  }
}

#endif
