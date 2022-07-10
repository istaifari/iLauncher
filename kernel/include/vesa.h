#pragma once
#include <kernel.h>

typedef struct vbe_info
{
	char signature[4];
	uint16_t version;
	uint32_t oem;
	uint32_t capabilities;
	uint32_t video_modes;
	uint16_t video_memory;
	uint16_t software_rev;
	uint32_t vendor;
	uint32_t product_name;
	uint32_t product_rev;
	char reserved[222];
	char oem_data[256];
} __attribute__((packed)) vbe_info_t;

typedef struct mode_info_block
{
	uint16_t attributes;
	uint8_t windowA, windowB;
	uint16_t granularity;
	uint16_t windowSize;
	uint16_t segmentA, segmentB;
	uint32_t winFuncPtr;
	uint16_t pitch;

	uint16_t resolutionX, resolutionY;
	uint8_t wChar, yChar, planes, bpp, banks;
	uint8_t memoryModel, bankSize, imagePages;
	uint8_t reserved0;

	uint8_t readMask, redPosition;
	uint8_t greenMask, greenPosition;
	uint8_t blueMask, bluePosition;
	uint8_t reservedMask, reservedPosition;
	uint8_t directColorAttributes;

	// linear frame buffer
	uint32_t physbase;
	uint32_t offScreenMemOff;
	uint16_t offScreenMemSize;
	uint8_t reserved1[206];
} mode_info_t;

typedef union uint24
{
	struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	} rgb;
	unsigned int integer : 24 __attribute__((packed));
	;
} uint24_t;

void vesa_install();