
#include "stdafx.h"

#include "TempestException.h"

#include "SoundChannelStatus.h"

#include "Pokey.h"


bool Pokey::noiseWaveformsInitialized = false;
bool Pokey::poly17[128 * 1024];
bool Pokey::poly5[32];


Pokey::Pokey(void)
{
	// clear
	ALLPOT = 0;

	// initialize our noise buffers
	InitializeNoiseBuffers();
}

Pokey::~Pokey(void)
{
}


SoundChannelStatus Pokey::GetSoundChannelStatus(int channel)
{
	switch (channel)
	{
	case 0:
		return sound1.GetStatus();
	case 1:
		return sound2.GetStatus();
	case 2:
		return sound3.GetStatus();
	case 3:
		return sound4.GetStatus();
	default:
		throw TempestException("Pokey::GetSoundChannelStatus: Invalid channel number");
	}
}

uint8_t Pokey::ReadByte(uint16_t address)
{
	switch (address)
	{
	case 0x8:
		return ALLPOT;

	case 0x0A:
		// random number
		return (uint8_t)rand();

	default:
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey read: %02X", address);
			throw TempestException(buf);
		}
	}
}

void Pokey::WriteByte(uint16_t address, uint8_t value)
{
	switch (address)
	{
	case 0x00:
		sound1.SetFrequency(value);
		break;

	case 0x01:
		sound1.SetVolume(value & 0x0F);
		sound1.SetOutputWave(value >> 4);
		break;

	case 0x02:
		sound2.SetFrequency(value);
		break;

	case 0x03:
		sound2.SetVolume(value & 0x0F);
		sound2.SetOutputWave(value >> 4);
		break;

	case 0x04:
		sound3.SetFrequency(value);
		break;

	case 0x05:
		sound3.SetVolume(value & 0x0F);
		sound3.SetOutputWave(value >> 4);
		break;

	case 0x06:
		sound4.SetFrequency(value);
		break;

	case 0x07:
		sound4.SetVolume(value & 0x0F);
		sound4.SetOutputWave(value >> 4);
		break;

	case 0x8:
		// audio control... for the moment I only support the simplest mode of
		// operation until I'm told I need to support more
		if (value != 0)
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey write: %02X, %02X", address, value);
			throw TempestException(buf);
		}
		break;

	case 0xB:
		// this just latches the "pot port state", which is basically just
		// a bunch of bit inputs
		ALLPOT = GetALLPOT();
		break;

	case 0xF:
		// serial port control... not supported
		break;

	default:
		{
			char buf[200];
			sprintf_s(buf, "Invalid Pokey write: %02X, %02X", address, value);
			throw TempestException(buf);
		}
	}
}


void Pokey::AddWaveData(int16_t *buffer, int count)
{
	sound1.AddWaveData(buffer, count);
	sound2.AddWaveData(buffer, count);
	sound3.AddWaveData(buffer, count);
	sound4.AddWaveData(buffer, count);
}


void Pokey::InitializeNoiseBuffers(void)
{
	// never mind if we've already been here
	if (noiseWaveformsInitialized)
		return;

	// make our noise waveforms
	MakeNoise(poly17, 128 * 1024);
	MakeNoise(poly5, 32);

	// note that we're initialized
	noiseWaveformsInitialized = true;
}

void Pokey::MakeNoise(bool *buffer, int count)
{
	// just for good form I make sure there's no DC
	int	onesLeft = count / 2;
	int   zerosLeft = count - onesLeft;
	while (onesLeft>0 && zerosLeft>0)
	{
		int random = rand() % (onesLeft + zerosLeft);
		if (random > onesLeft)
		{
			buffer[onesLeft + zerosLeft - 1] = false;
			--zerosLeft;
		}
		else
		{
			buffer[onesLeft + zerosLeft - 1] = true;
			--onesLeft;
		}
	}

}
