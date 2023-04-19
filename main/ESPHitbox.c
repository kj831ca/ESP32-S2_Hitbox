#include "ESPHitbox.h"

#define TAG "HITBOX_Debug"

#define bitset(byte, nbit) ((byte) |= (1 << (nbit)))
#define bitclear(byte, nbit) ((byte) &= ~(1 << (nbit)))

#define bit64set(long, nbit) ((long) |= (1ULL << (nbit)))
#define bit64clear(long, nbit) ((long) &= ~(1ULL << (nbit)))

#define UP_DOWN_CHK 5     // Binary 101
#define LEFT_RIGHT_CHK 10 // Binary 1010

// Mapping the IO for UP,RIGHT,DOWN,LEFT
#define DPAD_IO_MAP                                    \
    {                                                  \
        GPIO_NUM_1, GPIO_NUM_2, GPIO_NUM_3, GPIO_NUM_4 \
    }

#define BUTTON_IO_MAP                  \
    {                                  \
        GPIO_NUM_5,      /*Button 1*/  \
            GPIO_NUM_6,  /*Button 2*/  \
            GPIO_NUM_7,  /*Button 3*/  \
            GPIO_NUM_8,  /*Button 4*/  \
            GPIO_NUM_9,  /*Button 5*/  \
            GPIO_NUM_10, /*Button 6*/  \
            GPIO_NUM_11, /*Button 7*/  \
            GPIO_NUM_12, /*Button 8*/  \
            GPIO_NUM_13, /*Button 9*/  \
            GPIO_NUM_14, /*Button 10*/ \
            GPIO_NUM_15                \
    }

const hitbox_dpad_map_t DPAD_MAP = {.gpio_addr = DPAD_IO_MAP, .dpad_value = 0, .dpad_raw = 0};

const int button_io_map[] = BUTTON_IO_MAP;

const hitbox_button_map_t BUTTON_MAP = {.gpio_addr = button_io_map, .buttons_max = (sizeof(button_io_map) / 4)};

static hitbox_gamepad_report_t my_hitbox = {
    .report = {},
    .dpad_gpio = DPAD_IO_MAP,
    .button_handle = &BUTTON_MAP};

esp_err_t InitDPAD(const int gpio_addr[4])
{

    uint64_t gpio_mask = 0;

    for (int i = 0; i < 4; i++)
    {
        bit64set(gpio_mask, gpio_addr[i]);
    }
    // zero-initialize the config structure.
    gpio_config_t io_conf = {};
    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = gpio_mask;
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 1;
    // configure GPIO with the given settings
    return gpio_config(&io_conf);
}

esp_err_t InitButton(const hitbox_button_map_t *button_map)
{
    uint64_t gpio_mask = 0;

    for (int i = 0; i < button_map->buttons_max; i++)
    {
        ESP_LOGI(TAG, "Init button GPIO Address %d", button_map->gpio_addr[i]);
        bit64set(gpio_mask, button_map->gpio_addr[i]);
    }
    // zero-initialize the config structure.
    gpio_config_t io_conf = {};
    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = gpio_mask;
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 1;
    // configure GPIO with the given settings
    return gpio_config(&io_conf);
}
void ScanDPAD(hitbox_gamepad_report_t *hitbox_report)
{
    uint8_t scanValue = 0;
    uint8_t *mapped_dpad = &hitbox_report->report.hat;
    const int *gpio_addr = hitbox_report->dpad_gpio;
    for (int i = 0; i < 4; i++)
    {
        if (gpio_get_level(gpio_addr[i]) != 0)
        {
            bitclear(scanValue, i);
        }
        else
        {
            bitset(scanValue, i);
        }
    }

    if (hitbox_report->dpad_raw_value != scanValue)
    {
        ESP_LOGI(TAG, "DPAD Value = %d", scanValue);
        hitbox_report->dpad_raw_value = scanValue;
        hitbox_report->needUpdate = true;
    }
    else
    {
        // If the DPAD scan value has not been changed from the previous scan then we do nothing.
        return;
    }
    if (scanValue == 0x0f)
    {
        //IF all DPAD buttons pressed then register as natural.
        scanValue = 0;
    }
    else
    {
        // If Left and Right is pressed then register as Natural
        if ((scanValue & LEFT_RIGHT_CHK) == LEFT_RIGHT_CHK)
        {
            scanValue &= ~LEFT_RIGHT_CHK;
        }

        // If Up and down is pressed then register as UP
        if ((scanValue & UP_DOWN_CHK) == UP_DOWN_CHK)
        {
            bitset(scanValue, 0);
            bitclear(scanValue, 2);
        }
    }

    switch (scanValue)
    {
    case 1:
        *mapped_dpad = DPAD_UP;
        break;
    case 3:
        *mapped_dpad = DPAD_UP_RIGHT;
        break;
    case 2:
        *mapped_dpad = DPAD_RIGHT;
        break;
    case 6:
        *mapped_dpad = DPAD_DN_RIGHT;
        break;
    case 4:
        *mapped_dpad = DPAD_DN;
        break;
    case 12:
        *mapped_dpad = DPAD_DN_LEFT;
        break;
    case 8:
        *mapped_dpad = DPAD_LEFT;
        break;
    case 9:
        *mapped_dpad = DPAD_UP_LEFT;
        break;
    default:
        *mapped_dpad = 0; // No pressed if 4 directions are being pressed.
        break;
    }
}

void ScanButton(hitbox_gamepad_report_t *hitbox_report)
{
    uint32_t button_value = 0;
    const hitbox_button_map_t *btn_map = hitbox_report->button_handle;
    for (int i = 0; i < btn_map->buttons_max; i++)
    {
        if (gpio_get_level(btn_map->gpio_addr[i]) != 0)
        {
            bitclear(button_value, i);
        }
        else
        {
            bitset(button_value, i);
        }
    }
    if (hitbox_report->report.buttons != button_value)
    {
        ESP_LOGI(TAG, "Buttons Value = %ld", button_value);
        hitbox_report->needUpdate = 1;
        hitbox_report->report.buttons = button_value;
    }
}

esp_err_t InitHitBox()
{
    esp_err_t status;

    ESP_LOGI(TAG, "Initialize IO for DPAD");
    status = InitDPAD(my_hitbox.dpad_gpio);
    if (status != ESP_OK)
    {
        ESP_LOGE(TAG, "Error init DPAD GPIO %d", status);
        return status;
    }

    status = InitButton(my_hitbox.button_handle);
    if (status != ESP_OK)
    {
        ESP_LOGE(TAG, "Error init Button GPIO %d", status);
        return status;
    }

    return status;
}

void PollingHitBox()
{
    ScanDPAD(&my_hitbox);
    ScanButton(&my_hitbox);
}

bool IsNeedUpdate()
{
    return my_hitbox.needUpdate;
}

bool UpdateReport()
{
    if (my_hitbox.needUpdate)
    {
        my_hitbox.needUpdate = 0;
        return tud_hid_n_report(0, my_hitbox.report_id, &my_hitbox.report, sizeof(hid_gamepad_report_t));
    }
    return 0;
}
