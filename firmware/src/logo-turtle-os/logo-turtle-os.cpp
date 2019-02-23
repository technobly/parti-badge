/**
 * Particle Mesh Sumo Bot
 * Logo Turtle OS Controller
 * Author: Brett Walach
 * Date: 2019-02-19
 */

#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "macros.h"
#include "display/display.h"
#include "interrupts/interrupts.h"
#include "inputs/inputs.h"
#include "leds/leds.h"
#include "music/music.h"
#include "sumo-common/sumo-common.h"

void updateLogoTurtleOS() {
    setLEDs(CHOICE_RED | CHOICE_BLUE | CHOICE_GREEN | CHOICE_YELLOW);
    int read_control = 0;
    int read_control_last = 0;
    displaySetup("Enter Cmd: & Press >", "Press < to quit");
    String logoCmd = "";
    int cmdDelay = 500;
    uint32_t display_wait = 0;

    // allow previous button activity to settle, and no button to be pressed
    uint32_t start = millis();
    while (millis() - start < (DEBOUNCE_INTERVAL * 2) || read_control != 0) {
        checkInput(read_control);
    }

    while (1) {
        checkInput(read_control);
        // Quit?
        if ((read_control & JOY_LF) == (JOY_LF)) {
            for (int x = 0; x < 3; x++) {
                Mesh.publish("rc-sumo", String(BTN_NONE)); // STOP!!!
                if (x != 3) {
                    delay(50);
                }
            }
            return;
        }
        // Controls changed?
        if (read_control != read_control_last) {

            if (read_control & BTN_UP) {
                logoCmd += "F";
            } else if (read_control & BTN_DN) {
                logoCmd += "B";
            } else if (read_control & BTN_LF) {
                logoCmd += "L";
            } else if (read_control & BTN_RT) {
                logoCmd += "R";
            } else if (read_control & JOY_UP) {
                // logoCmd += "U";
                if (cmdDelay < 1000) {
                    cmdDelay += 100;
                }
                String temp = String::format("Cmd Delay: %d ms", cmdDelay);
                displayHeader(temp.c_str());
                display_wait = millis();
            } else if (read_control & JOY_DN) {
                // logoCmd += "D";
                if (cmdDelay > 100) {
                    cmdDelay -= 100;
                }
                String temp = String::format("Cmd Delay: %d ms", cmdDelay);
                displayHeader(temp.c_str());
                display_wait = millis();
            } else if (read_control & JOY_RT) {
                // TODO: Move this logo CMD parser to the bot
                while (logoCmd.length() > 0) {
                    String cmd = String(logoCmd.charAt(0));
                    int control = 0;

                    if (cmd == "F") {
                        control = BTN_UP;
                    } else if (cmd == "B") {
                        control = BTN_DN;
                    } else if (cmd == "L") {
                        control = BTN_LF;
                    } else if (cmd == "R") {
                        control = BTN_RT;
                    }
                    // } else if (cmd == "U") {
                    //     cmdDelay += 100;
                    // } else if (cmd == "D") {
                    //     cmdDelay -= 100;
                    // }

                    Mesh.publish("rc-sumo", String(control));
                    displayHeader(logoCmd);
                    displayLoop(control);
                    delay(cmdDelay);

                    logoCmd.remove(0, 1);
                }

                // Reset for next command
                Mesh.publish("rc-sumo", BTN_NONE);
                logoCmd = "";
                displayLoop(read_control);
            }

            if (logoCmd.length()) {
                displayHeader(logoCmd.c_str());
                display_wait = 0;
            } else if (display_wait && (millis() - display_wait < 1000UL)) {
                // prevent default splash from overriding this temp command until it times out
            } else {
                displayHeader("Enter Cmd: & Press >");
                display_wait = 0;
            }
            displayLoop(read_control);
            read_control_last = read_control;
        }
        // Restore display after temp command
        if (display_wait && (millis() - display_wait > 1000UL)) {
            display_wait = 0;
            if (logoCmd.length()) {
                displayHeader(logoCmd.c_str());
            } else {
                displayHeader("Enter Cmd: & Press >");
            }
            display.display();
        }
    }
}
