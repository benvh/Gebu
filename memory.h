#ifndef MEMORY_H_SCFM0ISU
#define MEMORY_H_SCFM0ISU

#include "global.h"

typedef enum
{
	MBC0, MBC1, MBC1R, MBC1RB, MBC2, MBC2B
}MBCMode;

typedef struct _MEMORY
{
	byte inbios;		//flag to check if bios is wired or not
	
	MBCMode mbcMode;	//Memory Bank Controller 'mode'
	byte currROMBank;	//currently selected ROM bank -> 1..nn  (rom bank 0 is always loaded in 0x0000 - 0x3FFF)
	byte currRAMBank;	//currently select external cartridge RAM bank -> 0..4
	
	byte *bios;			//256 bytes for the BIOS
	byte *mem;			//the gameboy memory (we will only allocate 32k instead of 64k, we will read the rom banks (first 32k) directly from the cartridge data)
	
	byte *cartridge;	//Cartridge data
	byte *extRAM;		//Cartridge RAM
	ushort extRAMSize;
	
}Memory;


Memory*			Memory_Create();
void			Memory_Destroy(Memory *this);

void			Memory_LoadCartridge(Memory *this, const char *filename);
const char*		Memory_GetCartridgeName(Memory *this);

byte			Memory_ReadByte(Memory *this, const ushort addr);
ushort			Memory_ReadWord(Memory *this, const ushort addr);
void			Memory_WriteByte(Memory *this, const ushort addr, const byte data);
void			Memory_WriteWord(Memory *this, const ushort addr, const ushort data);

#endif /* end of include guard: MEMORY_H_SCFM0ISU */
