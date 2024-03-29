#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

#define DPAD_UP 1
#define DPAD_UP_RIGHT 2
#define DPAD_RIGHT 3
#define DPAD_DN_RIGHT 4
#define DPAD_DN 5
#define DPAD_DN_LEFT 6
#define DPAD_LEFT 7
#define DPAD_UP_LEFT 8


typedef struct DPAD
{
    const int gpio_addr[4]; //{Left,Right,Up,Down}
    int8_t dpad_value;
    uint8_t dpad_raw;

}hitbox_dpad_map_t;

typedef struct BUTTONS_MAP
{
    uint8_t buttons_max ;// Max is 32
    const int* gpio_addr; //mapped to {UP,RIGHT,DOWN.LEFT}
}hitbox_button_map_t;

typedef struct HITBOX_BUTTONS
{
    hid_gamepad_report_t report;

    const int dpad_gpio[4];
    const hitbox_button_map_t* button_handle;

    uint32_t button_value;
    uint8_t dpad_raw_value;
    
    int8_t report_id;
    uint8_t needUpdate;
    
}hitbox_gamepad_report_t;
/*
bool update_gamepad_report(hitbox_gamepad_report_t * hitbox_report)
{
    //Copy the current report to prev_report
    if(hitbox_report->needUpdate)
    {
        //memcpy(&hitbox_report->report,&hitbox_report->prev_report, sizeof(hitbox_gamepad_report_t));
        hitbox_report->needUpdate = 0;
        return tud_hid_n_report(0,hitbox_report->report_id,&hitbox_report->report,sizeof(hitbox_gamepad_report_t));
    }
    return 0; 

}*/

esp_err_t InitHitBox();
void PollingHitBox();
bool UpdateReport();
bool IsNeedUpdate();

