#include "cpu.h"
#include <stdlib.h>
#include <string.h>

CPU* CPU_Create()
{
	CPU *this = (CPU*)malloc(sizeof(CPU));
	
	this->wr = (Register*)malloc(rCount * sizeof(Register));
	this->br = (byte**)malloc((rCount*2) * sizeof(byte*));
	
	int i, j=0;
	for(i = 0; i < rCount; i++) {
		this->br[j++] = &( this->wr[i].hi);
		this->br[j++] = &( this->wr[i].lo);
	}
	
	CPU_Reset(this);
	
	return this;
}

void CPU_Reset(CPU *this)
{
	memset(this->wr, 0, rCount * sizeof(Register));
	this->sp = 0;
	this->pc = 0;
}


byte CPU_LDrr(CPU *this, int rId, int nrId)
{
	if((7 & rId) == rId && (7 & nrId) == nrId) {
		*(this->br[rId]) = *(this->br[nrId]);
		return 4;
	}
	debug_print("WARNING: `LDrr` Register index out of bounds! rId=%d, nrId=%d\n", rId, nrId);
	return 0;
}

byte CPU_LDrn(CPU *this, int rId, byte n)
{
	if((7 & rId) == rId) {
		*(this->br[rId]) = n;
		return 7;
	}
	debug_print("WARNING: `LDrn` Register index out of bounds! rId=%d\n", rId);
	return 0;
}