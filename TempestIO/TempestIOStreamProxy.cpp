// =========================================================================
//    TempestIOStreamProxy.cpp
//    Author: Randy Rasmussen
//    Copyright: none, use at your own risk and to your heart's content
//
//		This is an implementation of TempestIO that connects to the actual
//    IO device via a stream.  My plan is to use a USB-to-serial connection,
//    but for testing it's most convenient to leave this as generic as
//    possible.
// =========================================================================

#include "stdafx.h"

#include "../TempestCPU/TempestException.h"
#include "AbstractTempestStream.h"

#include "TempestIOStreamProxy.h"


TempestIOStreamProxy::TempestIOStreamProxy(AbstractTempestStream *_stream)
   :
      stream(_stream)
{
   // copy parameters
   this->stream = stream;

   // clear
   buttons = 0;
   encoder = 0;
   cpuTime = 0;
   lastSendTime = 0;
   leds = 0;

   // start the first packet
   StartPacket();
}


void TempestIOStreamProxy::SetSoundChannelState(int channel, SoundChannelState state)
{
   // copy the value
   channelState[channel] = state;

   // if this is still the first tick of the packet this will get reported
   // as the channel's initial state, and we don't report it here
   int elapsedTicks = (int)((cpuTime - lastSendTime) / SoundIOPacketReader::ClockCyclesPerTick);
   if (elapsedTicks == 0)
      return;

   // add the value to the current packet... the first byte is:
   //  - channel number (high 3 bits)
   //  - number of ticks to delay (low 5 bits)
   stream.Write((channel<<5) | (elapsedTicks - ticksThisPacket));
   ticksThisPacket = elapsedTicks;

   // then we write the channel data
   stream.Write(channelState[channel].GetFrequency());
   stream.Write(channelState[channel].GetVolumeAndWaveform());
}

void TempestIOStreamProxy::SetTime(uint64_t clockCycles)
{
   // update our clock
   cpuTime = clockCycles;

   // figure out how much time has elapsed since packet start
   int elapsedTicks = (int)((clockCycles - lastSendTime) / SoundIOPacketReader::ClockCyclesPerTick);

   // if we are no longer in the first tick of the packet it's time to send
   // the packet's initial state
   if (elapsedTicks != 0 && !initialStatesSent)
   {
      // send the mask of channels that are currently on
      uint8_t channelMask = 0;
      for (int i = 0; i < 8; ++i)
         if (channelState[i].GetVolume() != 0)
            channelMask |= (1 << i);
      stream.Write(channelMask);

      // for each channel in the channel mask send its data
      for (int i = 0; i < 8; ++i)
      {
         if (channelMask & (1 << i))
         {
            stream.Write(channelState[i].GetFrequency());
            stream.Write(channelState[i].GetVolumeAndWaveform());
         }
      }

      initialStatesSent = true;
   }

   // send a packet if it's time
   if ((cpuTime - lastSendTime) > SoundIOPacketReader::ClockCyclesPerPacket)
   {
      lastSendTime += SoundIOPacketReader::ClockCyclesPerPacket;

      // end the packet
      stream.EndPacket();

      // start the next packet
      StartPacket();
   }

   // and check the return stream
   TempestInPacket packet;
   while (packet.ReadFromStream(&stream))
   {
      this->buttons = packet.GetButtons();
      this->encoder = packet.GetEncoder();
   }
}


void TempestIOStreamProxy::SetButtonLED(ButtonFlag button, bool value)
{
   if (value)
      leds |= button;
   else
      leds &= ~button;
}


void TempestIOStreamProxy::StartPacket(void)
{
   // clear
   ticksThisPacket = 0;
   initialStatesSent = false;

   // send the start packet
   stream.StartPacket();

   // send the flags byte
   stream.Write(leds);
}

