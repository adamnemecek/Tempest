
#include "stdafx.h"

#include "TempestCPU/TempestException.h"
#include "AsteroidsROMs.h"

#include "AsteroidsVectorMemory.h"



AsteroidsVectorMemory::AsteroidsVectorMemory(void)
{
   // clear
   memset(this->vectorRAM, 0, sizeof(this->vectorRAM));

   // implement our logic
   counterClock.OnRisingEdge([this](){
      if (_counterLoad)
         SetCounterValue(counterValue + 1);
   });
   _counterLoad.OnFallingEdge([this](){
      UpdatePC();
   });
   _stackWrite.OnFallingEdge([this](){
      UpdateTopOfStack();
   });
   _stackRead.OnValueChanged([this](bool newValue){
      UpdatePC();
   });
   stackPointerClock.OnRisingEdge([this](){
      if (_stackPointerIncrement)
         --stackPointer;
      else
         ++stackPointer;
      UpdatePC();
   });
   _enableLoadAddress.OnFallingEdge([this](){
      UpdatePC();
   });
   for (int i=0; i<12; ++i)
      loadAddressInputs[i].OnValueChanged([this](bool newValue){
         UpdatePC();
      });
   addressLowBit.OnValueChanged([this](bool newValue){
      UpdateOutputs();
   });
}

void AsteroidsVectorMemory::SetVectorRAM(const void *vectorRAM, int size)
{
   if (size<0 || size>sizeof(this->vectorRAM))
      throw TempestException("AsteroidsVectorMemory::SetVectorRAM: invalid size");
   memcpy(this->vectorRAM, vectorRAM, size);
}


void AsteroidsVectorMemory::SetCounterValue(uint16_t newValue)
{
   if (newValue == counterValue)
      return;
   counterValue = newValue;
   UpdateOutputs();
}


void AsteroidsVectorMemory::UpdatePC(void)
{
   if (!_counterLoad)
   {
      if (!_stackRead)
      {
         SetCounterValue(stack[stackPointer]);
      }
      else
      {
         uint8_t newPC = 0;
         for (int i = 0; i < 12; ++i)
            if (loadAddressInputs[i])
               newPC |= 1 << i;
         SetCounterValue(newPC);
      }
   }
}


void AsteroidsVectorMemory::UpdateTopOfStack(void)
{
   stack[stackPointer] = counterValue;
}


void AsteroidsVectorMemory::UpdateOutputs(void)
{
   // calculate the target address
   uint16_t pc = counterValue * 2;
   if (addressLowBit)
      ++pc;

   // the vector state machine continues to read after HALT is signaled, so
   // it would not be unusual to hit an invalid address; just zero out the
   // high bits in such cases
   pc &= 0x1FFF;

   uint8_t data;
   if (pc >= 0 && pc < 0x1000)
      data = vectorRAM[pc & 0x7FF];
   else if (pc >= 0x1000 && pc<0x2000)
      data = ROM035127_02[pc - 0x1000];

   for (int i = 0; i < 8; ++i)
      outputs[i].Set((data & (1<<i)) != 0);
}

