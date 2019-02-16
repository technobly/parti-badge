static const char itmBack[] = "< Back";
static const char itmOn[] = "On";
static const char itmOff[] = "Off";
static const char itmEnabled[] = "Enabled";
static const char itmDisabled[] = "Disabled";

// mnu* note - first item is menu title and it does not count toward cnt
static const char itmRoot[] = "PartiBadge";
static const char itmRootMode[] = "Sumo Bot";
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
    itmRootMode, itmRootDisplay, itmRootMesh, itmRootSensors, itmRootMusic,
    itmRootGames, itmRootGraphics, itmRootAnimations, itmRootBlinky, itmRootAbout};
const int cntRoot = 10;

static const char itmMode1[] = "RC Mode";
static const char itmMode2[] = "Logo Turtle OS";
const char *const mnuMode[] = {
    itmRootMode,
    itmMode1, itmMode2, itmBack};
const int cntMode = 2;

static const char itmDisplay1[] = "Badge Marquee";
static const char itmDisplay2[] = "Name";
static const char itmDisplay3[] = "Twitter Handle";
static const char itmDisplay4[] = "Temp & Humidity";
static const char itmDisplay5[] = "Carousel";
const char *const mnuDisplay[] = {
    itmRootDisplay,
    itmDisplay1, itmDisplay2, itmDisplay3, itmDisplay4, itmDisplay5, itmBack};
const int cntDisplay = 6;

static const char itmMesh1[] = "Mesh Info";
static const char itmMesh2[] = "Hello Ping";
static const char itmMesh3[] = "Gateway Ping";
static const char itmMesh4[] = "Start spooky!";
static const char itmMesh5[] = "End spooky!";
const char *const mnuMesh[] = {
    itmRootMesh,
    itmMesh1, itmMesh2, itmMesh3, itmMesh4, itmMesh5, itmBack};
const int cntMesh = 6;

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
static const char itmMusic5[] = "Joy to the World";
static const char itmMusic6[] = "We Wish You";
static const char itmMusic7[] = "Rudolph";
static const char itmMusic8[] = "Jingle Bells";
static const char itmMusic9[] = "Silent Night";
const char *const mnuMusic[] = {
    itmRootMusic,
    itmMusic1, itmMusic2, itmMusic3, itmMusic4, itmMusic5,
    itmMusic6, itmMusic7, itmMusic8, itmMusic9, itmBack};
const int cntMusic = 10;

static const char itmGame1[] = "Etch a Sketch";
static const char itmGame2[] = "Snake";
static const char itmGame3[] = "Simon Says";
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
