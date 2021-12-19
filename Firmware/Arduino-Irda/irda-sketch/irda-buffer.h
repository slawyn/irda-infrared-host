#ifndef IRDA_BUFFER
#define IRDA_BUFFER

#define ASSERT(condition) {while(!condition){};}
/************************************************
        Types
 ***********************************************/

#define dBufferSize 64
typedef struct {
  uint8_t arrui8Buffer[dBufferSize];
  uint8_t ui8BytePreamble;
  uint8_t ui8BytesWritten;
  uint8_t ui8Length;
} Buffer_t;

#define dHeapBufferMaxMemory 128
typedef struct HeapBuffer_t{
  HeapBuffer_t *pstcHeapBufferNext;
  uint8_t ui8Length;
  uint8_t arrui8Buffer[dHeapBufferMaxMemory];
} HeapBuffer_t;

typedef struct {
  HeapBuffer_t *pstcHeapBufferHead;
  HeapBuffer_t *pstcHeapBufferEnd;
  uint8_t ui8WrittenLength;
} PowerBuffer_t;


typedef struct {
  Buffer_t arrstcBuffers[4];
  uint8_t ui8UnackedFrameCount;
} FrameStorage_t;

// Allocate buffer to head or end
HeapBuffer_t *pstcAllocateNextHeapBuffer(PowerBuffer_t *pstcPowerBuffer, uint8_t ui8Length) {
  HeapBuffer_t *pstcHeapBuffer;

  ASSERT((ui8Length <=dHeapBufferMaxMemory));
  pstcHeapBuffer = (HeapBuffer_t*)malloc(sizeof(pstcHeapBuffer) + 1 + ui8Length);
  pstcHeapBuffer->ui8Length = ui8Length;
  pstcHeapBuffer->pstcHeapBufferNext = NULL;
  if (pstcPowerBuffer->pstcHeapBufferEnd != NULL) {         // if end exists hang it on there
    pstcPowerBuffer->pstcHeapBufferEnd->pstcHeapBufferNext = pstcHeapBuffer;
    pstcPowerBuffer->pstcHeapBufferEnd = pstcHeapBuffer;
  } else if (pstcPowerBuffer->pstcHeapBufferHead != NULL) { // if head exists create end
    pstcPowerBuffer->pstcHeapBufferEnd = pstcHeapBuffer;
    pstcPowerBuffer->pstcHeapBufferHead->pstcHeapBufferNext = pstcHeapBuffer;
  } else {
    pstcPowerBuffer->pstcHeapBufferHead = pstcHeapBuffer;   // create head
  }
  return pstcHeapBuffer;
}

// Always deallocate head
void vDeallocateHeapBufferHead(PowerBuffer_t *pstcPowerBuffer) {
  HeapBuffer_t *pstcHeapBuffer;

  pstcHeapBuffer = pstcPowerBuffer->pstcHeapBufferHead;
  if (pstcHeapBuffer != NULL) {
    if (pstcHeapBuffer->pstcHeapBufferNext != NULL) {
      pstcPowerBuffer->pstcHeapBufferHead = pstcHeapBuffer->pstcHeapBufferNext;                       // replace head if there is a next
      if (pstcPowerBuffer->pstcHeapBufferHead == pstcPowerBuffer->pstcHeapBufferEnd);              // set end to zero if no next
      pstcPowerBuffer->pstcHeapBufferEnd = NULL;
    } else {
      pstcPowerBuffer->pstcHeapBufferHead = NULL;
    }
    free(pstcHeapBuffer);
  }

  pstcPowerBuffer->ui8WrittenLength = 0;  // Reset Length
}

/*Get Next Buffer Data*/
HeapBuffer_t* pstcHandleHextHeapBuffer(PowerBuffer_t *pstcPowerBuffer){
  HeapBuffer_t *pstcHeapBuffer = pstcPowerBuffer->pstcHeapBufferHead;
  if(pstcHeapBuffer != NULL && pstcPowerBuffer->ui8WrittenLength == pstcPowerBuffer->pstcHeapBufferHead->ui8Length){
    vDeallocateHeapBufferHead(pstcPowerBuffer);
    pstcHeapBuffer = pstcPowerBuffer->pstcHeapBufferHead;
    pstcPowerBuffer->ui8WrittenLength = 0;
  }

  return pstcHeapBuffer;
}


/************************************************
        Defines
 ***********************************************/

#define dResetBuffer(buffer) { buffer.ui8Length = 0; buffer.ui8BytesWritten = 0; buffer.ui8BytePreamble = 0;}
#define dMarkBufferAsReceived(buffer) {buffer.ui8Length = 0;}
#define dMarkBufferForTransmission(buffer, func) {buffer.ui8BytesWritten = 0; buffer.ui8Length = func;}

#endif
