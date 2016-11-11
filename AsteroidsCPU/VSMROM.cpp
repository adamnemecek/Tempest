
#include "stdafx.h"
#include "VSMROM.h"

static const uint8_t romData[] = {
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x0F, 0x0C, 0x0A, 0x0E,
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x0F, 0x0C, 0x0A, 0x0E,
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x0D, 0x0F, 0x0C, 0x0B, 0x0E,
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x0B, 0x0D, 0x01, 0x00, 0x08, 0x0C, 0x0A, 0x0E,
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x08, 0x0C, 0x0A, 0x0E,
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x09, 0x0C, 0x0A, 0x0E,
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x09, 0x0C, 0x0A, 0x0E,
      0x00, 0x01, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x0A, 0x0C, 0x0A, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x0F, 0x0C, 0x0A, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x0F, 0x0C, 0x0A, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x0D, 0x0F, 0x0C, 0x0B, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x0B, 0x0D, 0x01, 0x00, 0x08, 0x0C, 0x0A, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x08, 0x0C, 0x0A, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x09, 0x0C, 0x0A, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x09, 0x0C, 0x0A, 0x0E,
      0x09, 0x02, 0x0D, 0x0D, 0x05, 0x06, 0x07, 0x0D, 0x09, 0x0D, 0x01, 0x00, 0x0A, 0x0C, 0x0A, 0x0E
   };


VSMROM::VSMROM(void)
{
   for (int i = 0; i < 8; ++i)
      inputs[0].OnValueChanged([this](bool newValue){ UpdateValue(); });
}

void VSMROM::UpdateValue(void)
{
   int address = 0;
   if (inputs[0])
      address |= 0x01;
   if (inputs[1])
      address |= 0x02;
   if (inputs[2])
      address |= 0x04;
   if (inputs[3])
      address |= 0x08;
   if (inputs[4])
      address |= 0x10;
   if (inputs[5])
      address |= 0x20;
   if (inputs[6])
      address |= 0x40;
   if (inputs[7])
      address |= 0x80;

   uint8_t output = romData[address];
   outputs[0].Set((output & 1) != 0);
   outputs[1].Set((output & 2) != 0);
   outputs[2].Set((output & 4) != 0);
   outputs[3].Set((output & 8) != 0);
}


