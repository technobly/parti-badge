# #PartiBadge Sumo Bot Edition

- Fixed Particle Logo
- Sped up refresh rate of display by 400%
- Sped up boot time with SEMI_AUTOMATIC mode (runs setup() before connected to Particle Cloud) and shortened some things
- Added Sumo Bot Menu
    - Added RC Controller for [Particle Mesh Sumo Bot](https://github.com/technobly/particle-mesh-sumo-bot)
        - Try driving with the joystick and turning at the same time with buttons for smooth figure 8's!
    - Added Logo Turtle OS control system
        - Enter a string of commands `F` `B` `L` `R` and press `>` to send to the Sumo Bot.
        - Use `^` and `v` to adjust the default Command Delay of 500 ms (100 ~ 1000 ms).
- Left rest of PartiBadge functionality in-tact

## Local Compile

```
$ git checkout v0.9.0
$ cd firmware/main
// enter DFU mode
firmware/main $ make clean all -s APPDIR=~/code/debugging/technobly/parti-badge/firmware PLATFORM=xenon COMPILE_LTO=n program-dfu
```

![sumo bot menu](https://raw.github.com/technobly/parti-badge/sumo-bot/images/sumo-bot-menu.png)

(original README.md follows)

---

# #PartiBadge

The #PartiBadge is a Wi-Fi connected, full-featured, Particle-powered conference badge. Because who want to wear a piece of plastic around their neck when they can wear a PCB?

The #PartiBadge was designed by the Particle team and is currently used as an interactive learning platform for conferences and other developer events.

For workshop hands-on labs and other details, click [here](https://part.cl/workshop).

The hardware folder contains Eagle design files for iterations of the badge hardware. Since the original creation of the Electron-based PartiBadge in the Spring of 2018, there have been two additional major iterations to support the Photon (v2) and Mesh-based hardware (v3).

The firmware folder contains firmware for the latest version of the badge, currently v3.

The finished badge includes:

1.  A 1" OLED Screen
2.  An SMD Piezo buzzer
3.  A SPDT Switch
4.  A 5-way joystick
5.  An SMD Si7021 temperature and Humidity sensor
6.  4 Tactile LED Buttons in Red, Blue, Green and Yellow/Orange
7.  An I2C-Compatible breakout for #BadgeLife add-ons
