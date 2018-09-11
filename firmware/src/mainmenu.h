static const char itmBack[] = "< Back";
static const char itmOn[] = "On";
static const char itmOff[] = "Off";
static const char itmEnabled[] = "Enabled";
static const char itmDisabled[] = "Disabled";

// mnu* note - first item is menu title and it does not count toward cnt
static const char itmRoot[] = "Spectra 2018 Badge";
static const char itmRootDisplay[] = "Display";
static const char itmRootMesh[] = "Mesh Tools";
static const char itmRootSensors[] = "Sensors";
static const char itmRootMusic[] = "Music";
static const char itmRootGames[] = "Games";
static const char itmRootAnimations[] = "Animations";
static const char itmRootBlinky[] = "Blnky";
static const char itmRootAbout[] = "About";

static const char *mnuRoot[] = {
    itmRoot,
    itmRootDisplay, itmRootMesh, itmRootSensors, itmRootMusic,
    itmRootGames, itmRootAnimations, itmRootBlinky, itmRootAbout};
const int cntRoot = 8;

static const char itmRootDisplay1[] = "Badge Marquee";
static const char itmRootDisplay2[] = "Name";
static const char itmRootDisplay3[] = "Twitter Handle";
static const char itmRootDisplay4[] = "Temp & Humidity";
const char *const mnuDisplay[] = {
    itmRootDisplay,
    itmRootDisplay1, itmRootDisplay2, itmRootDisplay3, itmRootDisplay4, itmBack };
const int cntDisplay = 5;

static const char itmRootMesh1[] = "Mesh Network";
static const char itmRootMesh2[] = "Network Scan";
static const char itmRootMesh3[] = "Hello Ping";
const char *const mnuMesh[] = {
    itmRootMesh,
    itmRootMesh1, itmRootMesh2, itmRootMesh3, itmBack };
const int cntMesh = 4;
