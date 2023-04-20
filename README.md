| Supported Targets | ESP32-S2 
| ----------------- | -------- 

# Arcade Joystick (Hitbox Style) using ESP32-S2

I build my own Hitbox Joystick and wrote the firmware ESP32S2 Mini board. The normal polling rate of Joystick is around 250Hz (4 milli seconds) with ESP32 S2 I can do close to 1000 Hz which is 1 msec. 

The compiler for this project is ESP-IDF 5.0. I found that it is a little bit tricky to compile or build the project at first time. There are the BIN files that allow you to flash to ESP32S2 Mini board under /firmware_binary . Here is the command to flash ESP32 S2, you will need to locate the folder where 'esptool.py' is

python.exe $env:IDF_PATH\components\esptool\esptool.py -p (PORT) -b 460800 --before default_reset --after hard_reset --chip esp32s2  write_flash --flash_mode dio --flash_size 2MB --flash_freq 80m 0x1000 .\bootloader.bin 0x8000 .\partition-table.bin 0x10000 .\esp32s2_hitbox.bin


| Button        | GPIO Number   |
| :-----------: |:-------------:|
| UP            | 1             |
| RIGHT         | 2             |
| DOWN          | 3             |
| LEFT          | 4             |
| Button 1      | 5             |
| Button 2      | 6             |
| Button 3      | 7             |
| Button 4      | 8             |
| Button 5      | 9             |
| Button 6      | 10            |
| Button 7      | 11            |
| Button 8      | 12            |
| Button 9      | 13            |
| Button 10     | 14            |
| Button 11     | 15            |

