#include "Arduino.h"

static const char itmBack[] = "< Back";
static const char itmOn[] = "On";
static const char itmOff[] = "Off";
static const char itmEnabled[] = "Enabled";
static const char itmDisabled[] = "Disabled";

static const char itmRoot[] = "Header Title";
static const char itmSubmenu1[] = "Submenu 1";
static const char itmSubmenu2[] = "Submenu 2";
static const char itmSubmenu3[] = "Submenu 3";
static const char itmMessageBox[] = "Message box";
static const char itmItem1[] = "Item 1";
static const char itmItem2[] = "Item 2";
static const char itmItem3[] = "Item 3";
static const char itmItem4[] = "Item 4";
static const char itmItem5[] = "Item 5";

////////////////////////////////////////////////////////////////
// menus - first item is menu title and it does not count toward cnt

static const char *mnuRoot[] = {
    itmRoot,
    itmSubmenu1, itmSubmenu2, itmSubmenu3, itmMessageBox};
const int cntRoot = 4;

static const char *mnuSubmenu1[] = {
    itmSubmenu1,
    itmItem1, itmItem2, itmItem3, itmItem4, itmItem5, itmBack};
const int cntSubmenu1 = 6;

static const char *mnuSubmenu2[] = {
    itmSubmenu2,
    itmOn, itmOff, itmBack};
const int cntSubmenu2 = 3;

static const char *mnuSubmenu3[] = {
    itmSubmenu3,
    itmEnabled, itmDisabled, itmBack};
const int cntSubmenu3 = 3;
