/*
 * Gearboy - Nintendo Game Boy Emulator
 * Copyright (C) 2012  Ignacio Sanchez

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/ 
 * 
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <iostream>
#include <fstream>

//#define DEBUG_GEARBOY 1

#ifndef NULL
#define NULL 0
#endif

#ifdef _WIN32
#define BLARGG_USE_NAMESPACE 1
#endif

#define SAVE_FILE_SIGNATURE "GearboySaveFile"
#define SAVE_FILE_VERSION 5

#define SafeDelete(pointer) if(pointer != NULL) {delete pointer; pointer = NULL;}
#define SafeDeleteArray(pointer) if(pointer != NULL) {delete [] pointer; pointer = NULL;}

#define InitPointer(pointer) ((pointer) = NULL)
#define IsValidPointer(pointer) ((pointer) != NULL)

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef void (*RamChangedCallback) (void);

#define FLAG_ZERO 0x80
#define FLAG_SUB 0x40
#define FLAG_HALF 0x20
#define FLAG_CARRY 0x10
#define FLAG_NONE 0

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

struct GB_Color
{
	// bits are in RGB5A1 format
	u16 bits;

	inline u8 gblow() const { 
            return ((bits >> 11) & 0x1F) | ((bits >> 1) & 0xE0);
	}

	inline u8 gbhigh() const { 
            return  0x80 | ((bits << 1) & 0x7C) | ((bits >> 6) & 0x03);
	}

	inline void gblow(u16 l) { 
		bits &= 0x063F;
		bits |= (l << 11);
		bits |= (l << 1) & 0x01C0;
	}

	inline void gbhigh(u16 h) { 
		bits &= 0xF9C0;
		bits |= ((h >> 1) & 0x3E) | 1;
        bits |= (h << 9) & 0x0600;
	}

    GB_Color (u16 red, u16 green, u16 blue) {
	    float to5bit = 31.0 / 255.0;
	    red *= to5bit;
	    green *= to5bit;
	    blue *= to5bit;
        bits = (red << 11) | (green << 6) | (blue << 1) | 1;
	}

    GB_Color()
    {
        bits = 0;
    }
};


enum Gameboy_Keys
{
    Right_Key = 0,
    Left_Key = 1,
    Up_Key = 2,
    Down_Key = 3,
    A_Key = 4,
    B_Key = 5,
    Select_Key = 6,
    Start_Key = 7
};

#ifdef DEBUG_GEARBOY
#define Log(msg, ...) (Log_func(msg, ##__VA_ARGS__))
#else
#define Log(msg, ...)  
#endif

inline void Log_func(const char* const msg, ...)
{
    static int count = 1;
    char szBuf[512];

    va_list args;
    va_start(args, msg);
    vsprintf(szBuf, msg, args);
    va_end(args);

    printf("%d: %s\n", count, szBuf);

    count++;
}

inline u8 SetBit(const u8 value, const u8 bit)
{
    return value | (0x01 << bit);
}

inline u8 UnsetBit(const u8 value, const u8 bit)
{
    return value & (~(0x01 << bit));
}

inline bool IsSetBit(const u8 value, const u8 bit)
{
    return (value & (0x01 << bit)) != 0;
}

#endif	/* DEFINITIONS_H */

