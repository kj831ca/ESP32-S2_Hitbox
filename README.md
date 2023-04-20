| Supported Targets | ESP32-S2 
| ----------------- | -------- 

# Arcade Joystick (Hitbox Style) using ESP32-S2

I build my own Hitbox Joystick and wrote the firmware ESP32 S2. The normal polling rate of Joystick is around 250Hz (4 milli seconds) with ESP32 S2 I can do close to 1000 Hz which is 1 msec. 

I'm using ESP-IDF V5.0 as a compiler. 


│ Function          | GPIO Num    │
| ------------------|:-----------:|
│ UP                │      1      │
│ RIGHT             │      2      │
├───────────────────┼─────────────┤
│DOWN               │ GPIO 3      │
├───────────────────┼─────────────┤
│bMaxPacketSize0    │ 64          │
├───────────────────┼─────────────┤
│idVendor           │ 0x303a      │
├───────────────────┼─────────────┤
│idProduct          │ 0x4004      │
├───────────────────┼─────────────┤
│bcdDevice          │ 0x100       │
├───────────────────┼─────────────┤
│iManufacturer      │ 0x1         │
├───────────────────┼─────────────┤
│iProduct           │ 0x2         │
├───────────────────┼─────────────┤
│iSerialNumber      │ 0x3         │
├───────────────────┼─────────────┤
│bNumConfigurations │ 0x1         │
└───────────────────┴─────────────┘
I (480) TinyUSB: TinyUSB Driver installed
I (480) example: USB initialization DONE
I (2490) example: Sending Keyboard report
I (3040) example: Sending Mouse report
```
