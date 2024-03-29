/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"
#include "ESPHitbox.h"


static const char *TAG = "example";

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID report descriptor
 *
 * In this example we implement Keyboard + Mouse HID device,
 * so we must define both report descriptors
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_GAMEPAD()
};

/**
 * @brief Configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and 1 HID interface
 */
static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(0, 0, false, sizeof(hid_report_descriptor), 0x81, 16, 1),
};

/********* TinyUSB HID callbacks ***************/

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    ESP_LOGI(TAG, "GET HID Report CB");
    return hid_report_descriptor;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
}

/********* Application ***************/


static tusb_desc_device_t descriptor_config = {
    .bLength = sizeof(descriptor_config),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A,
    .idProduct = 0x4004,
    .bcdDevice = 0x100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

static char const *string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 },  // 0: is supported language is English (0x0409)
    "Kris SOI14",                      // 1: Manufacturer
    "SOI14 USB Hitbox",               // 2: Product
    "00001",                       // 3: Serials
    "Home made HITBOX USB",                  // 4. MSC
};
static void JoyPadTask(void *pvParameter)
{
    uint64_t task_cycle = 0;
    #ifdef MEASURE_SPD
    InitSpeedMeasurePIN();
    #endif
    TickType_t prevTick = xTaskGetTickCount();
    while (1)
    {
       
        #ifdef MEASURE_SPD
        TURN_ON(POLLING_PIN);
        #endif
        task_cycle++;
        if (tud_mounted())
        {
          
           PollingHitBox();
           #ifdef MEASURE_SPD
           if(IsNeedUpdate())
           {
                TURN_ON(USB_REPORT_PIN);
           } 
           #endif
           UpdateReport();
        }
        #ifdef MEASURE_SPD
        TURN_OFF(POLLING_PIN);
        TURN_OFF(USB_REPORT_PIN);
        #endif
        
        //We set the Free RTOS tick to 1000 Hz in menu config.
        vTaskDelayUntil(&prevTick,pdMS_TO_TICKS(1)); 
    }
}
void app_main(void)
{
    // Initialize button that will trigger HID reports

    ESP_LOGI(TAG, "Initialize ESP32S2 Hitbox GPIO");
    ESP_ERROR_CHECK(InitHitBox());

    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &descriptor_config,
        .string_descriptor = string_desc_arr,
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    ESP_LOGI(TAG, "Size of Descriptor %d", sizeof(hid_report_descriptor));

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");

    xTaskCreate(JoyPadTask,"Joy Stick Polling",4096,NULL,5,NULL);

}
