
/* Piezo Pin */
/* Use C0 for SoftPWM, otherwise, can bodge to WKP/A7 by shorting C0 to A7 */
#define BUZZER_PIN A7

/* Switch Pins for game modes */
#define DISPLAY_MODE_PIN C1
#define GAME_MODE_PIN C2

/* Display Mode variables */
#define DISPLAY_MODE 0
#define GAME_MODE 1

/* TFT SPI Pins */
#define TFT_CS A2
#define TFT_DC D2
#define TFT_RST D1
#define TFT_LIGHT D0

/* Tactile LED Button Pins */
/* LEDS */
#define RED_LED B0
#define GREEN_LED B2
#define BLUE_LED B4
#define YELLOW_LED D3
/* Buttons */
#define RED_BUTTON_A B1
#define BLUE_BUTTON_B B5
#define GREEN_BUTTON_C B3
#define YELLOW_BUTTON_D D4

/* Battery Critical Level */
#define BATTERY_CRITICAL 25
#define BATTERY_SHUTOFF 20

/* General Macros */
#define TEMP_CHECK_INTERVAL 120000
#define BATT_CHECK_INTERVAL 300000
