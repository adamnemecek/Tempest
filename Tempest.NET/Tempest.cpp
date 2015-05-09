

#include "stdafx.h"
#include <msclr\lock.h>

#include "TempestLib/6502/CPU6502.h"
#include "TempestLib/6502/CPU6502Exception.h"
#include "TempestLib/TempestBus.h"
#include "TempestLib/TempestException.h"
#include "TempestLib/TempestRunner.h"
#include "TempestLib/Win32/Win32RealTimeClock.h"
#include "TempestLib/Win32/Win32WaveStreamer.h"


#include "Tempest.h"


using namespace System;

namespace TempestDotNET {
	Tempest::Tempest(void)
	{
		// clear
		synchronizer = gcnew Object();

		realTimeClock = new Win32RealTimeClock();
		tempestRunner = new TempestRunner(realTimeClock);
		vectorData = new VectorData();
		waveStreamer = new Win32WaveStreamer();
	}

	Tempest::~Tempest(void)
	{
		// delete
		delete waveStreamer, waveStreamer = NULL;
		delete vectorData, vectorData = NULL;
		delete tempestRunner, tempestRunner = NULL;
		delete realTimeClock, realTimeClock = NULL;
	}

	String ^Tempest::GetMathBoxStatus(void)
	{
		return gcnew String(tempestRunner->GetMathBoxStatus().c_str());
	}

	String ^Tempest::GetMathBoxLogData(void)
	{
		return gcnew String(tempestRunner->GetMathBoxLogXML().c_str());
	}

	uint64_t Tempest::GetTotalClockCycles(void)
	{
		return tempestRunner->GetTotalClockCycles();
	}

	VectorEnumerator ^Tempest::GetVectorEnumerator(void)
	{
		if (tempestRunner->HaveNewVectorData())
		{
			tempestRunner->PopVectorData(*vectorData);
			return gcnew VectorEnumerator(*vectorData);
		}
		else
		{
			return nullptr;
		}
	}

	void Tempest::LoadROM(array<Byte>^ rom, int address)
	{
		std::vector<uint8_t> romCopy;
		for (int i = 0; i < rom->Length; ++i)
			romCopy.push_back(rom[i]);
		tempestRunner->LoadROM(&romCopy[0], (int)romCopy.size(), address);
	}

	void Tempest::LoadMathBoxROM(array<Byte>^ rom, char slot)
	{
		std::vector<uint8_t> romCopy;
		for (int i = 0; i < rom->Length; ++i)
			romCopy.push_back(rom[i]);
		tempestRunner->LoadMathBoxROM(&romCopy[0], (int)romCopy.size(), slot);
	}

	void Tempest::SetOnePlayerButton(bool pressed)
	{
		tempestRunner->SetButtonState(ONE_PLAYER_BUTTON, pressed);
	}


	void Tempest::Start(void)
	{
		tempestRunner->Start();
	}

	String ^Tempest::GetProcessorStatus(void)
	{
		if (tempestRunner->IsTerminated())
			return gcnew String(tempestRunner->GetProcessorStatus().c_str());
		else
			return gcnew String("OK");
	}
}

