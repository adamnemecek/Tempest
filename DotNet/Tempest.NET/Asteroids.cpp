

#include "stdafx.h"
#include <msclr\lock.h>

#include "AsteroidsCPU/AsteroidsRunner.h"

#include "TempestCPU/6502/CPU6502.h"
#include "TempestCPU/6502/CPU6502Exception.h"
#include "TempestCPU/TempestBus.h"
#include "TempestCPU/TempestException.h"
#include "TempestCPU/Win32/Win32TempestEnvironment.h"

#include "TempestIO/Vector/SimpleVectorDataInterpreter.h"

#include "TDNWin32TempestIO.h"

#include "Asteroids.h"


using namespace System;

namespace TempestDotNET {
   Asteroids::Asteroids(TDNIOStreamProxy ^soundIO)
   {
      // create objects
      environment = new Win32TempestEnvironment();
      tempestSoundIO = soundIO->GetIOObject();
      asteroidsRunner = new AsteroidsRunner(environment);

      // hook objects together
      asteroidsRunner->SetTempestIO(tempestSoundIO);
   }

   Asteroids::Asteroids(TDNWin32TempestSoundIO ^soundIO)
   {
      // create objects
      environment = new Win32TempestEnvironment();
      tempestSoundIO = soundIO->GetSoundIOObject();
      asteroidsRunner = new AsteroidsRunner(environment);

      // hook objects together
      asteroidsRunner->SetTempestIO(tempestSoundIO);
   }

   Asteroids::~Asteroids(void)
	{
		// delete
      delete asteroidsRunner, asteroidsRunner = NULL;
      delete environment, environment = NULL;
	}

   uint64_t Asteroids::GetTotalClockCycles(void)
	{
      return asteroidsRunner->GetTotalClockCycles();
	}

   VectorEnumerator ^Asteroids::GetVectorEnumerator(void)
   {
      // get the latest vector data
      VectorData  vectorData;
      asteroidsRunner->GetVectorData(vectorData);

      // interpret it
      SimpleVectorDataInterpreter vectorInterpretor;
      vectorInterpretor.SetVectorData(vectorData);
      vectorInterpretor.Interpret();

      // return the result
      std::vector<SimpleVector> vectors;
      vectorInterpretor.GetAllVectors(vectors);
      return gcnew VectorEnumerator(vectors);
   }
   
   bool Asteroids::IsStopped(void)
	{
      return asteroidsRunner->IsStopped();
	}

   void Asteroids::SetBreakpoint(int address, bool set)
	{
      asteroidsRunner->SetBreakpoint(address, set);
	}

   void Asteroids::Start(void)
	{
      asteroidsRunner->Start();
	}

   String ^Asteroids::GetProcessorStatus(void)
	{
      if (asteroidsRunner->IsTerminated())
         return gcnew String(asteroidsRunner->GetProcessorStatus().c_str());
		else
			return gcnew String("OK");
	}
}
