#ifndef CPU_H_JW212KF4
#define CPU_H_JW212KF4

#include "global.h"

#define B2WORD(h,l) ((h<<8)+l)

typedef union
{
	ushort reg;
	struct
	{
		byte lo, hi;
	};
	
}Register;

enum 
{
	rBC, rDE, rHL, rAF, rCount
};

enum
{
	rB, rC, rD, rE, rH, rL, rA=7
	//6 == rHL
};


typedef struct
{	
	Register *wr; //Register pairs BC, DE, HL, AF
	byte **br; //Pointers to the seperate bytes of the register pairs, e.g., the value of B in register pair BC
	
	ushort sp, pc;
	
}CPU;


CPU* 		CPU_Create();
void		CPU_Reset(CPU *this);

byte		CPU_LDrr(CPU *this, int rId, int nrId);
byte		CPU_LDrn(CPU *this, int rId, byte n);


#endif /* end of include guard: CPU_H_JW212KF4 */
