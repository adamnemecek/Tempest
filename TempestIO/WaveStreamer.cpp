
#include "stdafx.h"
#include "WaveStreamer.h"


WaveStreamer::WaveStreamer(void)
{
   queueIn = 0;
   queueOut = 0;
   sampleCounter = 0;
   samplesInInputBuffer = 0;
}

WaveStreamer::~WaveStreamer(void)
{
}

bool WaveStreamer::ProcessNextEvent(void)
{
   // never mind if there are none in the queue
   if (queueIn == queueOut)
      return false;

   // grab from the queue
   WaveStreamEvent event = eventQueue[queueOut];
   if (queueOut == WAVE_STREAM_EVENT_QUEUE_SIZE - 1)
      queueOut = 0;
   else
      queueOut++;

   // process it
   switch (event.eventType)
   {
   case WAVE_EVENT_VOLUME:
      soundGenerator.SetChannelVolume(event.channel, event.value);
      break;

   case WAVE_EVENT_FREQUENCY:
      soundGenerator.SetChannelFrequency(event.channel, event.value);
      break;

   case WAVE_EVENT_WAVEFORM:
      soundGenerator.SetChannelWaveform(event.channel, event.value);
      break;

   case WAVE_EVENT_TICK:
      ProcessTick();
      break;
   }

   return true;
}


void WaveStreamer::FillBuffer(int16_t *buffer, int sampleCount)
{
   // if we don't have enough data in our input buffer to fill an output
   // buffer then we need to process messages
   while (samplesInInputBuffer < WAVE_STREAM_BUFFER_SAMPLE_COUNT)
      if (!ProcessNextEvent())
         break;

   // if we don't have enough data fake the passage of time until we do
   while (samplesInInputBuffer < WAVE_STREAM_BUFFER_SAMPLE_COUNT)
      ProcessTick();

   // copy the data... the Pokey output is very low amplitude... beef it up to the level we like
   for (int i = 0; i < sampleCount; ++i)
      buffer[i] = (int16_t)(inputBuffer[(unsigned)i] * 256);

   // remove the copied data from the input buffer
   samplesInInputBuffer -= sampleCount;
   memcpy(&inputBuffer[0], &inputBuffer[sampleCount], 2 * samplesInInputBuffer);
}


void WaveStreamer::ProcessTick(void)
{
   // figure out how many samples to add for this tick
   sampleCounter += 44100;
   int samplesToAdd = (int)(sampleCounter / 6000);
   sampleCounter -= samplesToAdd * 6000;

   // make sure we have room
   if (samplesInInputBuffer + samplesToAdd > WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT)
      samplesToAdd = WAVE_STREAM_INPUT_BUFFER_SAMPLE_COUNT - samplesInInputBuffer;

   // generate some samples
   soundGenerator.ReadWaveData(&inputBuffer[samplesInInputBuffer], samplesToAdd);
   samplesInInputBuffer += samplesToAdd;
}


void WaveStreamer::QueueEvent(const WaveStreamEvent &event)
{
   // figure out what the index will be after we add the event
   int nextIndex = queueIn + 1;
   if (nextIndex >= WAVE_STREAM_EVENT_QUEUE_SIZE)
      nextIndex = 0;

   // if it would result in a wraparound just drop this event
   if (nextIndex == queueOut)
      return;

   // enqueue and set the event
   eventQueue[queueIn] = event;
   queueIn = nextIndex;
}