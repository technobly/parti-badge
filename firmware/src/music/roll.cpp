/*  Rick Roll Code
    AUTHOR: Samantha Lagestee
    Copyright 2017 samilagestee at gmail dot com

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    DISCLAIMER: The song "Never Gonna Give You Up" by Rick Astley
    is not the creative property of the author. This code simply
    plays a BUZZER_PIN buzzer rendition of the song.
*/
#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "macros.h"
#include "images/spark.h"
#include "display/display.h"
#include "leds/leds.h"

#define a3f 208 // 208 Hz
#define b3f 233 // 233 Hz
#define b3 247  // 247 Hz
#define c4 261  // 261 Hz MIDDLE C
#define c4s 277 // 277 Hz
#define e4f 311 // 311 Hz
#define f4 349  // 349 Hz
#define a4f 415 // 415 Hz
#define b4f 466 // 466 Hz
#define b4 493  // 493 Hz
#define c5 523  // 523 Hz
#define c5s 554 // 554 Hz
#define e5f 622 // 622 Hz
#define f5 698  // 698 Hz
#define f5s 740 // 740 Hz
#define a5f 831 // 831 Hz

#define rest -1

extern Adafruit_SSD1306 display;
extern byte appmode;
extern byte btnid;

volatile int beatlength = 100; // determines tempo
float beatseparationconstant = 0.3;

int a = 4; // part index
int b = 0; // song index
int c = 0; // lyric index

boolean flag = true; // play/pause
boolean displaySetup = false;

// Parts 1 and 2 (Intro)

int song1_intro_melody[] =
    {c5s, e5f, e5f, f5, a5f, f5s, f5, e5f, c5s, e5f, rest, a4f, a4f};

int song1_intro_rhythmn[] =
    {6, 10, 6, 6, 1, 1, 1, 1, 6, 10, 4, 2, 10};

// Parts 3 or 5 (Verse 1)

int song1_verse1_melody[] =
    {rest, c4s, c4s, c4s, c4s, e4f, rest, c4, b3f, a3f,
     rest, b3f, b3f, c4, c4s, a3f, a4f, a4f, e4f,
     rest, b3f, b3f, c4, c4s, b3f, c4s, e4f, rest, c4, b3f, b3f, a3f,
     rest, b3f, b3f, c4, c4s, a3f, a3f, e4f, e4f, e4f, f4, e4f,
     c4s, e4f, f4, c4s, e4f, e4f, e4f, f4, e4f, a3f,
     rest, b3f, c4, c4s, a3f, rest, e4f, f4, e4f};

int song1_verse1_rhythmn[] =
    {2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
     1, 1, 1, 1, 3, 1, 2, 1, 5,
     1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3,
     1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
     5, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     2, 1, 1, 1, 3, 1, 1, 1, 3};

// Parts 4 or 6 (Chorus)

int song1_chorus_melody[] =
    {b4f, b4f, a4f, a4f,
     f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
     c5s, c5s, c5s, c5s,
     c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
     b4f, b4f, a4f, a4f,
     f5, f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
     c5s, c5s, c5s, c5s,
     c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest};

int song1_chorus_rhythmn[] =
    {1, 1, 1, 1,
     3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
     1, 1, 1, 1,
     3, 3, 3, 1, 2, 2, 2, 4, 8,
     1, 1, 1, 1,
     3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
     1, 1, 1, 1,
     3, 3, 3, 1, 2, 2, 2, 4, 8, 4};

void play();
void setupDisplay();

void playRoll()
{
  appmode = 1;
  btnid = 0;

  setupDisplay();
  toggleAllButtons(LOW);

  // play next step in song
  while (appmode)
  {
    play();
  }
}

void setupDisplay()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.drawBitmap(0, 16, sparkLogo, 128, 48, 1);
  display.display();
  display.startscrollleft(0x00, 0x0F);
}

void play()
{
  int notelength;
  if (a == 1 || a == 2)
  { // Intro
    // intro
    notelength = beatlength * song1_intro_rhythmn[b];
    if (song1_intro_melody[b] > 0)
    { // if not a rest, play note
      analogWrite(RED_LED, 255);
      tone(BUZZER_PIN, song1_intro_melody[b], notelength);
    }
    b++;
    if (b >= sizeof(song1_intro_melody) / sizeof(int))
    {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 3 || a == 5)
  { // Verse 1
    // verse
    notelength = beatlength * 2 * song1_verse1_rhythmn[b];
    if (song1_verse1_melody[b] > 0)
    {
      analogWrite(BLUE_LED, 255);
      tone(BUZZER_PIN, song1_verse1_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_verse1_melody) / sizeof(int))
    {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 4 || a == 6)
  { //chorus
    // chorus
    notelength = beatlength * song1_chorus_rhythmn[b];
    if (song1_chorus_melody[b] > 0)
    {
      analogWrite(GREEN_LED, 255);
      tone(BUZZER_PIN, song1_chorus_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_chorus_melody) / sizeof(int))
    {
      a++;
      b = 0;
      c = 0;
    }
  }
  delay(notelength); // necessary because BUZZER_PIN is on independent timer
  noTone(BUZZER_PIN);
  analogWrite(RED_LED, 0);
  analogWrite(BLUE_LED, 0);
  analogWrite(GREEN_LED, 0);
  delay(notelength * beatseparationconstant); // create separation between notes
  if (a == 7)
  { // loop back around to beginning of song
    a = 1;
  }
}
