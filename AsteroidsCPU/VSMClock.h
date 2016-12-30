
#ifndef VSMCLOCK_H
#define VSMCLOCK_H

#include "LogicSignal.h"

class VSMClock {
public:
   void Tick(void);

   LogicOutput &VGCK(void) { return clock1_5MHz; }
   LogicOutput &C6MHz(void) { return clock6MHz; }
   LogicOutput &C3MHz(void) { return clock3MHz; }

private:
   uint8_t counter = 0;
   LogicSignal clock6MHz;
   LogicSignal clock3MHz;
   LogicSignal clock1_5MHz;
};

#endif
