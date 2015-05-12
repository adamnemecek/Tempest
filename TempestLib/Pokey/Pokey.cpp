
#include "stdafx.h"

#include "TempestException.h"

#include "Pokey.h"


Pokey::Pokey(void)
{
	ALLPOT = 0;
}

Pokey::~Pokey(void)
{
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