
#include "Stdafx.h"
#include "Win32TempestIO.h"


Win32TempestSoundIO::Win32TempestSoundIO(void)
{
   buttons = 0;
   encoder = 0;
   leds = 0;
   cpuTime = 0;
}

Win32TempestSoundIO::~Win32TempestSoundIO(void)
{
}

void Win32TempestSoundIO::SetSoundChannelState(int channel, SoundChannelState state)
{
	waveStreamer.SetChannelState(channel, state);
}

void Win32TempestSoundIO::SetTime(uint64_t clockCycles)
{
	waveStreamer.Delay((int)(clockCycles - cpuTime));
   cpuTime = clockCycles;
}


void Win32TempestSoundIO::Fire(bool state)
{
   if (state)
      buttons |= FIRE_BUTTON;
   else
      buttons &= ~FIRE_BUTTON;
}


void Win32TempestSoundIO::OnePlayer(bool state)
{
   if (state)
      buttons |= ONE_PLAYER_BUTTON;
   else
      buttons &= ~ONE_PLAYER_BUTTON;
}


void Win32TempestSoundIO::TwoPlayer(bool state)
{
   if (state)
      buttons |= TWO_PLAYER_BUTTON;
   else
      buttons &= ~TWO_PLAYER_BUTTON;
}


bool Win32TempestSoundIO::OnePlayerLED(void)
{
   return (leds & ONE_PLAYER_BUTTON) != 0;
}

bool Win32TempestSoundIO::TwoPlayerLED(void)
{
   return (leds & TWO_PLAYER_BUTTON) != 0;
}


void Win32TempestSoundIO::Zap(bool state)
{
   if (state)
      buttons |= ZAPPER_BUTTON;
   else
      buttons &= ~ZAPPER_BUTTON;
}


void Win32TempestSoundIO::SetButtonLED(ButtonFlag button, bool value)
{
   if (value)
      leds |= button;
   else
      leds &= ~button;
}


