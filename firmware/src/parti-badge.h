
/* Piezo Pin */
/* Use C0 for SoftPWM, otherwise, can bodge to WKP/A7 by shorting C0 to A7 */
#define BUZZER_PIN A7

/* Switch Pins for game modes */
#define DISPLAY_MODE_PIN C1
#define GAME_MODE_PIN C2

/* Display Mode macros */
#define STARTUP_MODE 0
#define DISPLAY_MODE 1
#define GAME_MODE 2

/* Startup mode macros */
#define LOGO_DURATION 3000
#define TITLE_SCREEN_DURATION 2000
#define SETUP_DURATION 7000
#define IMAGE_DURATION 2000

/* TFT SPI Pins */
#define TFT_CS A1
#define TFT_DC D2
#define TFT_RST D1
#define TFT_LIGHT D0
#define TFT_SD_CS A2

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

/* Joystick Buttons */
#define JOYSTICK_UP D5
#define JOYSTICK_DOWN D6
#define JOYSTICK_LEFT RX
#define JOYSTICK_RIGHT TX
#define JOYSTICK_CENTER D7

/* Battery Critical Level */
#define BATTERY_CRITICAL 25
#define BATTERY_SHUTOFF 20

/* General Macros */
#define TEMP_CHECK_INTERVAL 120000
#define BATT_CHECK_INTERVAL 300000
