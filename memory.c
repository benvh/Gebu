#include "memory.h"

#include <stdlib.h>

void Memory_HandleBankChange(Memory*, ushort, byte);

Memory* Memory_Create()
{
	Memory *this = (Memory*)malloc(sizeof(Memory));
	
	this->inbios = 1;
	this->bios = (byte*)malloc(0x0100 * sizeof(byte));
	this->mem = (byte*)malloc(0x8000 * sizeof(byte));
	
	this->cartridge = NULL;
	this->extRAM = NULL;
	
	return this;
}

void Memory_Destroy(Memory *this)
{
	free(this->bios);
	free(this->mem);
	free(this->cartridge);
	free(this->extRAM);
	
	free(this);
}

void Memory_LoadCartridge(Memory *this, const char *filename)
{
	FILE *f = fopen(filename, "rb");
	if(f != NULL) {
		
		fseek(f, 0, SEEK_END);
		long int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		this->cartridge = (byte*)malloc(size * sizeof(byte));
		fread(this->cartridge, sizeof(byte), size, f);
		fclose(f);
		
		//Check the cartridge type (rom/ram banking)
		switch(this->cartridge[0x147]) {
			case 0: this->mbcType = MBC0; break;
			case 1: this->mbcType = MBC1; break;
			case 2: this->mbcType = MBC1R; break;
			case 3: this->mbcType = MBC1RB; break;
			case 5: this->mbcType = MBC2; break;
			case 6: this->mbcType = MBC2B; break;
		}
		this->currROMBank = 1;
		
		//Check the cartridge ram size (this is the external ram used for saving stuff... NOT the gameboy working ram)
		switch(this->cartridge[0x149]){
			case 1:	this->extRAMSize = 0x0800; break;
			case 2: this->extRAMSize = 0x2000; break;
			case 3: this->extRAMSize = 0x8000; break;
			default: this->extRAMSize = 0;
		}
		if(this->extRAMSize > 0) this->extRAM = (byte*)malloc(this->extRAMSize * sizeof(byte));
		this->currRAMBank = 0;
		
		//Load a saved external ram file for this cartridge (if any)
		if(this->mbcType == MBC1RB || this->mbcType == MBC2B) {
			//TODO: Load saved external RAM
		}
		
	} else {
		debug_print("ERROR: `Memory_LoadCartridge` Cartridge file with name `%s` not found!\n", filename);
	}
}

const char* Memory_GetCartridgeName(Memory *this)
{
	//TODO: (fix)String might not be 0 terminated
	if(this->cartridge) return &(this->cartridge[0x134]);
	return NULL;
}

byte Memory_ReadByte(Memory *this, const ushort addr)
{
	if(addr < 0x8000) { //Reading ROM...
		
		if(addr < 0x4000) { //Reading from ROM Bank 0
			if(addr < 0x0100 && this->inbios) return this->bios[addr];
			return this->cartridge[addr];
		} else { //Reading from ROM Bank (this->currROMBank)
			ushort naddr = (addr - 0x4000) + (this->currROMBank * 0x4000);
			return this->cartridge[naddr];
		}
		
	} else if(addr >= 0xA000 && addr<= 0xBFFF) { //Reading ext RAM
		
		ushort naddr = (addr - 0xA000) + (this->currRAMBank * 0x2000);
		if(naddr < this->extRAMSize) {
			return this->extRAM[naddr];
		} else {
			debug_print("ERROR: `Memory_ReadByte (extRAM)` memory address out of bounds! addr=0x%x\n", naddr);
		}
		
	} else { //Reading other memory (VRAM, STACK, ...)
		return this->mem[addr - 0x8000];
	}
	
	return 0;
}

ushort Memory_ReadWord(Memory *this, const ushort addr)
{
	return ( Memory_ReadByte(this, addr) + (Memory_ReadByte(this, addr+1)<<8) );
}

void Memory_WriteByte(Memory *this, const ushort addr, const byte val) 
{
	if(addr < 0x8000) { //Trying to write in ROM...
		Memory_HandleBankChange(this, addr, val);
	}
	//TODO: Complete implementation
	//TODO: Write to external RAM => also make persistent in extRam save file
}

void Memory_HandleBankChange(Memory *this, ushort addr, byte val)
{
	switch(this->mbcType) {
		
		case MBC1:
		case MBC1R:
		case MBC1RB:
			//TODO: Implement MBC1 bank change
			break;
			
		case MBC2:
		case MBC2B:
			//TODO: Implement MBC2 bank change
			break;
	}
}

