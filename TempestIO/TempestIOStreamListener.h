
#ifndef TEMPESTIOSTREAMLISTENER_H
#define TEMPESTIOSTREAMLISTENER_H

class AbstractTempestSoundIO;
class AbstractTempestStream;

class TempestIOStreamListener {
public:
   TempestIOStreamListener(AbstractTempestStream *stream, AbstractTempestSoundIO *tempestIO);

   void Service(void);

private:
   enum State {
      IDLE,
      BUTTON_LEDS,
      SOUND_VOLUME,
      SOUND_WAVE,
      SOUND_FREQUENCY
   };

private:
   AbstractTempestStream *stream;
   AbstractTempestSoundIO *tempestIO;

   State state;
   uint8_t soundChannel;
   uint8_t encoder;
};

#endif
