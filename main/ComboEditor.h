#pragma once
#include <stdint.h>
#include "C:\Kris Projects\ESP32S2Hitbox2\main\HitboxMacro.h"
//#include "C:\Projects\esp32\ESP32S2\ESP32S2Hitbox2\main\HitboxMacro.h"
#define HEADER 0x55FF;
typedef enum {
	READ_COMBO_CNT,
	READ_COMBO,
	READ_COMBO_FRAME_CNT,
	READ_FRAME,
	CREATE_COMBO,
	CREATE_FRAME,
	INSERT_FRAME,
	INSERT_FRAME_AT,
	REMOVE_COMBO,
	CLEAR_ALL_COMBO,
	REMOVE_FRAME_AT
}COMBO_COMMAND_ENUM;

typedef struct COMBO_PROTOCOL
{
	uint16_t header;
	uint8_t address;
	uint8_t cmd;
	uint8_t length;
	uint8_t* data;
	uint16_t chksum;
}combo_protocol_t;


