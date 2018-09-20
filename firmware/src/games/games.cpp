#include "macros.h"
#include "Adafruit_SSD1306.h"
#include "inputs/inputs.h"
#include "display/display.h"
#include "interrupts/interrupts.h"

extern Adafruit_SSD1306 display;
extern byte appmode;
extern byte btncounter;
extern byte btnid;

void drawFilledCircle(int displayX, int displayY)
{
  display.drawCircle(displayX, displayY, 1, WHITE);
  display.drawCircle(displayX, displayY, 2, WHITE);
  display.drawCircle(displayX, displayY, 3, WHITE);
  display.drawCircle(displayX, displayY, 4, WHITE);
  display.drawCircle(displayX, displayY, 5, WHITE);
  display.display();
}

// Init Etch A Sketch mode on the OLED
void initEtchASketch()
{
  clearScreen();
  display.setTextSize(2);
  display.setCursor(1, 18);
  display.println("  Etch A");
  display.println("  Sketch");
  display.setTextSize(1);
  display.println("Use the joystick...");
  display.display();
  delay(2000);

  clearScreen();
}

// Update the display during Etch A Sketch Mode. Draw a new filled circle
// based on the which joystick direction was used.
void etchASketch()
{
  appmode = 1;
  btnid = 0;
  int displayX = display.width() / 2;
  int displayY = display.height() / 2;

  initEtchASketch();
  drawFilledCircle(displayX, displayY);

  setupLEDButtonInterrupt();

  while (appmode)
  {
    int lastY = displayY;
    int lastX = displayX;

    joystickUpDebouncer.update();
    if (joystickUpDebouncer.read() == LOW)
    {
      displayY--;
    }

    joystickDownDebouncer.update();
    if (joystickDownDebouncer.read() == LOW)
    {
      displayY++;
    }

    joystickLeftDebouncer.update();
    if (joystickLeftDebouncer.read() == LOW)
    {
      displayX--;
    }

    joystickRightDebouncer.update();
    if (joystickRightDebouncer.read() == LOW)
    {
      displayX++;
    }

    // Reset the screen
    joystickCenterDebouncer.update();
    if (joystickCenterDebouncer.read() == LOW)
    {
      clearScreen();
      displayX = display.width() / 2;
      displayY = display.height() / 2;
    }

    if ((lastX != displayX) || (lastY != displayY))
    {
      drawFilledCircle(displayX, displayY);
    }
  }
}

/////// SNAKE ///////

const int D_NORTH = 1;
const int D_EAST = 2;
const int D_SOUTH = 3;
const int D_WEST = 4;

long score = 0;

byte snakePixelX[20];
byte snakePixelY[20];

int snakeX = 10;
int snakeY = 30;
int snakeLength = 1;
volatile int snakeDir = D_NORTH;

int minX = 0;
int minY = 20;
int maxX = MAX_X;
int maxY = MAX_Y;

int foodX = 0;
int foodY = 0;

bool outOfBounds()
{
  return snakeX <= minX || snakeY <= minY || snakeX >= maxX || snakeY >= maxY;
}

void renderScore()
{
  display.fillRect(0, 0, display.width() - 1, 8, BLACK);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print("Score: ");
  display.print(String(score, DEC));

  // TOP
  display.drawLine(0, 8, display.width() - 1, 8, WHITE);
  // LEFT
  display.drawLine(0, 8, 0, display.height() - 1, WHITE);
  // RIGHT
  display.drawLine(display.width() - 1, 8, display.width() - 1, display.height() - 1, WHITE);
  // BOTTOM
  display.drawLine(0, display.height() - 1, display.width() - 1, display.height() - 1, WHITE);
}

void gameOver()
{
  display.clearDisplay();
  display.setCursor(5, 5);
  display.print("GAME OVER :(");
  display.display();

  score = 0;
  snakeLength = 1;
  snakeX = display.width() / 2;
  snakeY = display.height() / 2;

  snakePixelX[snakeLength - 1] = snakeX;
  snakePixelY[snakeLength - 1] = snakeY;

  snakeDir = D_SOUTH;

  delay(2000);

  display.clearDisplay();
  renderScore();
}

// Drop food on random location
void dropFood()
{
  foodX = random(minX + 1, maxX - 1);
  foodY = random(minY + 1, maxY - 1);
}

bool collectFood()
{
  if (snakeX == foodX && snakeY == foodY)
  {
    score += 10;

    renderScore();
    dropFood();

    return true;
  }
  else
  {
    return false;
  }
}

bool crashedIntoSelf()
{
  for (byte i = 4; i < snakeLength; i++)
  {
    if (snakeX == snakePixelX[i] && snakeY == snakePixelY[i])
    {
      return true;
    }
  }

  return false;
}

void drawScreen()
{
  bool foodCollected = false;

  display.clearDisplay();

  display.drawPixel(foodX, foodY, WHITE);
  foodCollected = collectFood();

  // Check snake position
  if (outOfBounds() || crashedIntoSelf())
  {
    gameOver();
  }

  // Render the snake
  for (int i = 0; i < snakeLength; i++)
  {
    display.drawPixel(snakePixelX[i], snakePixelY[i], WHITE);
  }

  // Move pixel values
  for (int i = snakeLength; i > 0; i--)
  {
    snakePixelX[i] = snakePixelX[i - 1];
    snakePixelY[i] = snakePixelY[i - 1];
  }

  if (foodCollected)
  {
    snakeLength += 1;
    snakePixelX[snakeLength - 1] = snakeX;
    snakePixelY[snakeLength - 1] = snakeY;
  }

  switch (snakeDir)
  {
  case D_NORTH:
    snakeY -= 1;
    break;
  case D_EAST:
    snakeX += 1;
    break;
  case D_SOUTH:
    snakeY += 1;
    break;
  case D_WEST:
    snakeX -= 1;
    break;
  }

  snakePixelX[0] = snakeX;
  snakePixelY[0] = snakeY;

  renderScore();
  display.display();
}

void snakeGame()
{
  appmode = 1;
  btnid = 0;
  clearScreen();

  setupLEDButtonInterrupt();

  minX = 0;
  minY = 8;
  maxX = display.width() - 1;
  maxY = display.height() - 1;

  dropFood();

  while (appmode)
  {
    if (btnid == 5)
    {
      snakeDir = D_EAST;
    }
    else if (btnid == 2)
    {
      snakeDir = D_SOUTH;
    }
    else if (btnid == 3)
    {
      snakeDir = D_NORTH;
    }
    else if (btnid == 4)
    {
      snakeDir = D_WEST;
    }

    drawScreen();

    delay(60); //10 is too fast for snake. 300 is dirt slow. 100 is still slowish but decent
  }
  appmode = 0;
  btncounter++;
}
