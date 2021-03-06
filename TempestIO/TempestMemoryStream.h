
#ifndef TEMPESTMEMORYSTREAM_H
#define TEMPESTMEMORYSTREAM_H

#include "AbstractTempestStream.h"


class SimpleMemoryStream
{
public:
   SimpleMemoryStream(void);

   int  Peek(void);
   int  Read(void);
   bool Write(uint8_t b);

private:
	uint8_t buffer[20000];
	int bufferIn;
	int bufferOut;
};


class FullDuplexStream : public AbstractTempestStream
{
public:
   FullDuplexStream(SimpleMemoryStream *readStream, SimpleMemoryStream *writeStream) {
      this->readStream = readStream;
      this->writeStream = writeStream;
   }

   virtual int  Peek(void) { return readStream->Peek(); }
   virtual int  Read(void) { return readStream->Read(); }
   virtual bool Write(uint8_t b) { return writeStream->Write(b); }

private:
   SimpleMemoryStream *readStream;
   SimpleMemoryStream *writeStream;
};

class TempestMemoryStream
{
public:
   TempestMemoryStream(void)
      :
      leftSide(&rightToLeft, &leftToRight),
      rightSide(&leftToRight, &rightToLeft)
         {}
   AbstractTempestStream *GetLeftSide(void) { return &leftSide; }
   AbstractTempestStream *GetRightSide(void) { return &rightSide; }

private:
   SimpleMemoryStream leftToRight;
   SimpleMemoryStream rightToLeft;
   FullDuplexStream leftSide;
   FullDuplexStream rightSide;
};


#endif
