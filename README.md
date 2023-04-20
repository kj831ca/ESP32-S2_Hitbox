| Supported Targets | ESP32-S2 
| ----------------- | -------- 

# Arcade Joystick (Hitbox Style) using ESP32-S2

I build my own Hitbox Joystick and wrote the firmware ESP32S2 Mini board. The normal polling rate of Joystick is around 250Hz (4 milli seconds) with ESP32 S2 I can do close to 1000 Hz which is 1 msec. 

The compiler for this project is ESP-IDF 5.0. I found that it is a little bit tricky to compile or build the project at first time. There are the BIN files that allow you to flash to ESP32S2 Mini board under /firmware_binary . Here are the steps to flash the ROM image to the ESP32S2 Mini.

1. Install Visual Studio Code with Espressif IDF plug in.
2. Open the project folder.
3. Run the ESP-IDF Terminal locate on the bottom status bar of Visual Studio code.
4. Connect ESP32S2 Mini into the computer. For Windows, open Device manager, hold ESP32S2 Mini both RST and 0 buttons, then release RST while you still pressing 0 buttons then release 0 button. If you do correctly there should be another  Serial Port show up on your Windows. 
5. Go to firmware_binary and run the below command. Replace the (PORT) with the COM port name shown on your PC.

python.exe $env:IDF_PATH\components\esptool_py\esptool\esptool.py -p (PORT) -b 460800 --before default_reset --after hard_reset --chip esp32s2  write_flash --flash_mode dio --flash_size 2MB --flash_freq 80m 0x1000 .\bootloader.bin 0x8000 .\partition-table.bin 0x10000 .\esp32s2_hitbox.bin

6. Reset the ESP32S2 by press and release RST button. Run "Set up USB Game Controller", there should be 'SOI14 USB Hitbox" controller show up on your Installed game controller.

# SOCD Function
The firmware handle the SOCD as follow:

| SOCD                               |
| Combination         | Output       |
| :------------:      | ------------ |
| LEFT + RIGHT        | Natural      |
| UP + DOWN           | UP           |
| UP,DOWN,LEFT,RIGHT  | Natural      |

# Connection PINs

The push button on of the pins are connected to the ESP32S2 GPIO as the table below and the other pin will shared all the same GND pin of ESP32S2 Mini. 

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

