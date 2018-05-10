/*
 *      Copyright 2017 Particle Industries, Inc.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
 */

#include "notes.h"

void playTones(int tonePin, const int tones[]) {
  for (int thisNote = 1; thisNote < (tones[0] * 2 + 1); thisNote = thisNote + 2) {
    tone(tonePin, tones[thisNote], (1000 / tones[thisNote + 1]));
    delay((1000 / tones[thisNote + 1]) * 1.30);
    noTone(tonePin);
  }
}

const int notes_MarioStartup[] = {
  11,
  NOTE_E4, 8,
  NOTE_E4, 8,
  NOTE_H, 8,
  NOTE_E4, 8,
  NOTE_H, 8,
  NOTE_C4, 8,
  NOTE_E4, 8,
  NOTE_H, 8,
  NOTE_G4, 8,
  NOTE_H, 3,
  NOTE_G3, 8
};

const int notes_MarioGameOver[] = {
  15,
  NOTE_C4, 8,
  NOTE_H, 8,
  NOTE_H, 8,
  NOTE_G3, 8,
  NOTE_H, 4,
  NOTE_E3, 4,
  NOTE_A3, 6,
  NOTE_B3, 6,
  NOTE_A3, 6,
  NOTE_GS3, 6,
  NOTE_AS3, 6,
  NOTE_GS3, 6,
  NOTE_G3, 8,
  NOTE_F3, 8,
  NOTE_G3, 4
};

void playStartup(int tonePin) {
  playTones(tonePin, notes_MarioStartup);
}

void playGameOver(int tonePin) {
  playTones(tonePin, notes_MarioGameOver);
}
