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

static const char itmDisplay1[] = "Badge Marquee";
static const char itmDisplay2[] = "Name";
static const char itmDisplay3[] = "Twitter Handle";
static const char itmDisplay4[] = "Temp & Humidity";
const char *const mnuDisplay[] = {
    itmRootDisplay,
    itmDisplay1, itmDisplay2, itmDisplay3, itmDisplay4, itmBack };
const int cntDisplay = 5;

static const char itmMesh1[] = "Mesh Network";
static const char itmMesh2[] = "Network Scan";
static const char itmMesh3[] = "Hello Ping";
const char *const mnuMesh[] = {
    itmRootMesh,
    itmMesh1, itmMesh2, itmMesh3, itmBack };
const int cntMesh = 4;

static const char itmSensors1[] = "Temp & Humidity";
static const char itmSensors2[] = "Battery Status";
const char *const mnuSensors[] = {
    itmRootMesh,
    itmSensors1, itmSensors2, itmBack };
const int cntSensors = 3;

static const char itmMusic1[] = "Mario Start";
static const char itmMusic2[] = "Mario Game Over";
static const char itmMusic3[] = "Never Gonna";
static const char itmMusic4[] = "Bee Gees";
const char *const mnuMusic[] = {
    itmRootMusic,
    itmMusic1, itmMusic2, itmMusic3, itmMusic4, itmBack };
const int cntMusic = 5;

static const char itmGame1[] = "Simon Says";
static const char itmGame2[] = "Etch a Sketch";
static const char itmGame3[] = "Snake";
const char *const mnuGames[] = {
    itmRootGames,
    itmGame1, itmGame2, itmGame3, itmBack };
const int cntGames = 4;

static const char itmAnimation1[] = "All";
static const char itmAnimation2[] = "Snowflakes";
const char *const mnuAnimations[] = {
    itmRootAnimations,
    itmAnimation1, itmAnimation2, itmBack };
const int cntAnimations = 3;

static const char itmBlinky1[] = "Light ring";
static const char itmBlinky2[] = "Pulsing";
static const char itmBlinky3[] = "Random";
static const char itmBlinky4[] = "See-saw";
const char *const mnuBlinky[] = {
    itmRootBlinky,
    itmBlinky4, itmBlinky2, itmBlinky3, itmBlinky4, itmBack };
const int cntBlinky = 5;
