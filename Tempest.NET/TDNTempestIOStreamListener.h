
#ifndef TDNTEMPESTIOSTREAMLISTENER_H
#define TDNTEMPESTIOSTREAMLISTENER_H

using namespace System;
using namespace System::Threading;

#include "TDNMemoryStream.h"
#include "TDNWin32TempestIO.h"

class TempestIOStreamListener;

namespace TempestDotNET {

   public ref class TDNTempestIOStreamListener : public IDisposable {
	public:
		TDNTempestIOStreamListener(TDNMemoryStream ^stream, TDNWin32TempestIO ^tempestIO);
		~TDNTempestIOStreamListener(void);

   private:
      void ThreadEntry();

	private:
		TDNMemoryStream ^stream;
		TDNWin32TempestIO ^tempestIO;

		TempestIOStreamListener *listener;

      bool terminated;
      Thread ^thread;
	};
};


#endif
