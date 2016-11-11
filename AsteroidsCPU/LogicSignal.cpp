
#include "stdafx.h"
#include "LogicSignal.h"


// =========================================================
// ===================== LogicOutput =======================
// =========================================================

void LogicOutput::Set(bool value)
{
   // never mind no-ops
   if (value == this->value)
      return;

   // set and notify
   this->value = value;
   for (auto callback : changeCallbacks)
      callback(value);
   if (value)
      for (auto callback : risingEdgeCallbacks)
         callback();
}


// =========================================================
// ===================== LogicInput ========================
// =========================================================

void LogicInput::SetSource(LogicOutput &source)
{
   // register for notifications
   source.OnValueChanged([this](bool newValue){ Set(newValue); });

   // and set the initial value
   Set(source.Get());
}



// =========================================================
// ==================== LogicSignal ========================
// =========================================================

void LogicSignal::Set(bool value)
{
   // we have two base classes that declare this, but one is abstract, so
   // just call the one that isn't
   LogicOutput::Set(value);
}

