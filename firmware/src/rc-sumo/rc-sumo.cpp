/**
 * Particle Mesh Sumo Bot
 * RC Controller
 * Author: Brett Walach
 * Date: 2019-02-08
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

void updateRcSumo() {
    setLEDs(CHOICE_RED | CHOICE_BLUE | CHOICE_GREEN | CHOICE_YELLOW);
    int read_control = 0;
    int read_control_last = 0;
    displaySetup("Particle Sumo Bot", "Press < & (A) to quit");

    // allow previous button activity to settle, and no button to be pressed
    uint32_t start = millis();
    while (millis() - start < (DEBOUNCE_INTERVAL * 2) || read_control != 0) {
        checkInput(read_control);
    }

    while (1) {
        checkInput(read_control);
        // Quit?
        if ((read_control & (JOY_LF | BTN_LF)) == (JOY_LF | BTN_LF)) {
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
            Mesh.publish("rc-sumo", String(read_control));
            displayLoop(read_control);
            read_control_last = read_control;
        }
    }
}
