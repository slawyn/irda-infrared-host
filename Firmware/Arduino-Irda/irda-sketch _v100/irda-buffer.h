#ifndef IRDA_BUFFER
#define IRDA_BUFFER


/************************************************
 *      Types
 ***********************************************/
#define dCircularBufferSize 128
typedef struct{
  uint8_t arrui8Buffer[dCircularBufferSize];
  uint8_t ui8Head;
  uint8_t ui8Tail;
}CircularBuffer_t;

#define dBufferSize 64
typedef struct{
  uint8_t arrui8Buffer[dBufferSize];
  uint8_t ui8BytePreamble;
  uint8_t ui8BytesWritten;
  uint8_t ui8Length;
}Buffer_t;


typedef struct{
  Buffer_t arrstcBuffers[4];
  uint8_t ui8UnackedFrameCount;
}FrameStorage_t;


/************************************************
 *      Defines
 ***********************************************/
#define dPutByteIntoCircularBuffer(circularbuf, octet) {circularbuf.arrui8Buffer[circularbuf.ui8Head] = octet; circularbuf.ui8Head = (circularbuf.ui8Head + 1)&(dCircularBufferSize-1);}
#define dPutIntoCircularBuffer(circularbuf, buf, len)   for(uint8_t i=0;i<len;i++){ circularbuf.arrui8Buffer[circularbuf.ui8Head] = buf[i]; circularbuf.ui8Head = (circularbuf.ui8Head + 1)&(dCircularBufferSize-1);}
#define dGetFromCircularBuffer(circularbuf, buf, len)   for(uint8_t i=0;i<len;i++){ buf[i] = circularbuf.arrui8Buffer[circularbuf.ui8Tail]; circularbuf.ui8Tail = (circularbuf.ui8Tail + 1)&(dCircularBufferSize-1);}
#define dReadIntoCircularBuffer(circularbuf, len, readfunc)  for(uint8_t i=0;i<len;i++){ circularbuf.arrui8Buffer[circularbuf.ui8Head] = readfunc(); circularbuf.ui8Head = (circularbuf.ui8Head + 1)&(dCircularBufferSize-1);}
#define dWriteFromCircularBuffer(circularbuf, len, writefunc) for(uint8_t i=0;i<len;i++){ writefunc(circularbuf.arrui8Buffer[circularbuf.ui8Tail]); circularbuf.ui8Tail = (circularbuf.ui8Tail + 1)&(dCircularBufferSize-1);}
#define dClearCircularBuffer(circularbuf) {circularbuf.ui8Head = 0; circularbuf.ui8Tail = 0;}

#define dGetCircularBufferCredit(circularbuf)             (circularbuf.ui8Head>=circularbuf.ui8Tail?(dCircularBufferSize-(circularbuf.ui8Head-circularbuf.ui8Tail)):circularbuf.ui8Tail-circularbuf.ui8Head)
#define dGetCircularBufferUsedLength(circularbuf)         (dCircularBufferSize-dGetCircularBufferCredit(circularbuf))

#define dResetBuffer(buffer) { buffer.ui8Length = 0; buffer.ui8BytesWritten = 0; buffer.ui8BytePreamble = 0;}
#define dMarkBufferAsReceived(buffer) {buffer.ui8Length = 0;}
#define dMarkBufferForTransmission(buffer, func) {buffer.ui8BytesWritten = 0; buffer.ui8Length = func;}

#endif
