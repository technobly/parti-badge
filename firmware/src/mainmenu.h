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
static const char itmRootGraphics[] = "Graphics";
static const char itmRootAnimations[] = "Animations";
static const char itmRootBlinky[] = "Blinky";
static const char itmRootAbout[] = "About";

static const char *mnuRoot[] = {
    itmRoot,
    itmRootDisplay, itmRootMesh, itmRootSensors, itmRootMusic,
    itmRootGames, itmRootGraphics, itmRootAnimations, itmRootBlinky,
    itmRootAbout};
const int cntRoot = 9;

static const char itmDisplay1[] = "Badge Marquee";
static const char itmDisplay2[] = "Name";
static const char itmDisplay3[] = "Twitter Handle";
static const char itmDisplay4[] = "Temp & Humidity";
static const char itmDisplay5[] = "Carousel";
const char *const mnuDisplay[] = {
    itmRootDisplay,
    itmDisplay1, itmDisplay2, itmDisplay3, itmDisplay4, itmDisplay5, itmBack};
const int cntDisplay = 6;

static const char itmMesh1[] = "Mesh Network";
static const char itmMesh2[] = "Network Scan";
static const char itmMesh3[] = "Hello Ping";
const char *const mnuMesh[] = {
    itmRootMesh,
    itmMesh1, itmMesh2, itmMesh3, itmBack};
const int cntMesh = 4;

static const char itmSensors1[] = "Temp & Humidity";
static const char itmSensors2[] = "Battery Status";
const char *const mnuSensors[] = {
    itmRootMesh,
    itmSensors1, itmSensors2, itmBack};
const int cntSensors = 3;

static const char itmMusic1[] = "Mario Start";
static const char itmMusic2[] = "Mario Game Over";
static const char itmMusic3[] = "Never Gonna";
static const char itmMusic4[] = "Bee Gees";
const char *const mnuMusic[] = {
    itmRootMusic,
    itmMusic1, itmMusic2, itmMusic3, itmMusic4, itmBack};
const int cntMusic = 5;

static const char itmGame1[] = "Simon Says";
static const char itmGame2[] = "Etch a Sketch";
static const char itmGame3[] = "Snake";
const char *const mnuGames[] = {
    itmRootGames,
    itmGame1, itmGame2, itmGame3, itmBack};
const int cntGames = 4;

static const char itmGraphics1[] = "Spark";
static const char itmGraphics2[] = "Spectra";
static const char itmGraphics3[] = "Mesh";
static const char itmGraphics4[] = "Konami";
static const char *const mnuGraphics[] = {
    itmRootGraphics,
    itmGraphics1, itmGraphics2, itmGraphics3, itmGraphics4, itmBack};
const int cntGraphics = 5;

static const char itmAnimation1[] = "Snowflakes";
static const char itmAnimation2[] = "Circles";
static const char itmAnimation3[] = "Rectangles";
static const char itmAnimation4[] = "Triangles";
static const char itmAnimation5[] = "Round Rects";
static const char itmAnimation6[] = "Scroll Text";
static const char itmAnimation7[] = "All";
const char *const mnuAnimations[] = {
    itmRootAnimations,
    itmAnimation1, itmAnimation2, itmAnimation3, itmAnimation4,
    itmAnimation5, itmAnimation6, itmAnimation7, itmBack};
const int cntAnimations = 8;

static const char itmBlinky1[] = "Chase";
static const char itmBlinky2[] = "Pulsing";
static const char itmBlinky3[] = "Pulse Chase";
static const char itmBlinky4[] = "Random";
static const char itmBlinky5[] = "See-saw";
const char *const mnuBlinky[] = {
    itmRootBlinky,
    itmBlinky1, itmBlinky2, itmBlinky3, itmBlinky4, itmBlinky5, itmBack};
const int cntBlinky = 6;
