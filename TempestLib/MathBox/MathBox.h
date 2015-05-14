/* 
 * File:   MathBox.h
 * Author: Randy
 *
 * Created on April 27, 2015, 4:22 PM
 */

#ifndef MATHBOX_H
#define	MATHBOX_H

#include "Am2901.h"

class AbstractTempestEnvironment;
class MathBoxLog;


#pragma warning(push)
#pragma warning(disable : 4820)	// padding in structures

class MathBox
{
private:
	enum Bit {
		A10,
		A10STAR,
		A12,
		A18,
		C,
		J,
		LDAB,
		M,
		PCEN,
		Q0,
		R15,
		S,
		S0,
		S1
	};

public:
	MathBox(AbstractTempestEnvironment *_environment);
	virtual ~MathBox(void);

	void LoadROM(const uint8_t *rom, int length, char slot);

	std::string GetErrorString(void) const { return error.size() == 0 ? "OK" : error; }

   uint8_t GetStatus(void);
   uint8_t ReadLow(void);
   uint8_t ReadHigh(void);
   void Write(uint8_t address, uint8_t value);

private:
	Tristate GetTristate(Bit bit);
	void SetALUCarryFlags(void);
	void SetALUInputs(void);
	void SetError(const std::string &_status);

private:
	void HandleRisingClock(void);
	void HandleFallingClock(void);

private:
	// forbidden items
	MathBox(const MathBox &mathBox);
	MathBox &operator=(const MathBox &mathBox);

private:
	// construction parameters
	AbstractTempestEnvironment	*environment;

	// inputs
	NullableByte dataIn;
	NullableByte addressIn;
	Tristate BEGIN;

	// state values that change on rising clock
	NullableByte   PC;
	Tristate  Q0Latch;

	// state values that change on falling clock
	Tristate STOP;
	NullableByte JumpLatch;

	// ALUs
	Am2901	aluK;
	Am2901	aluF;
	Am2901	aluJ;
	Am2901	aluE;

	// ROMs
	std::vector<uint8_t> romA;
	std::vector<uint8_t> romE;
	std::vector<uint8_t> romF;
	std::vector<uint8_t> romH;
	std::vector<uint8_t> romJ;
	std::vector<uint8_t> romK;
	std::vector<uint8_t> romL;

	// profiling
	uint32_t totalMathBoxTime;
	uint32_t totalMathBoxWrites;
	uint32_t totalFallingClockTime;
	uint32_t totalRisingClockTime;
	uint32_t totalGetTristateTime;

	// misc
	std::string	error;

	friend class MathBoxTestInterface;
};

#pragma warning(pop)

#endif	/* MATHBOX_H */

