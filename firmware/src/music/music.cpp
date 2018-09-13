#include "notes.h"
#include "parti-badge.h"
#include "leds/leds.h"

int noteDuration = 115;

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

const int beegeesMelody[] = {
  NOTE_G4, NOTE_A4, 0, NOTE_C5, 0, 0, NOTE_G4, 0, 0, 0,
  NOTE_E4, 0, NOTE_D4, NOTE_E4, NOTE_G4, 0,
  NOTE_D4, NOTE_E4, 0, NOTE_G4, 0, 0,
  NOTE_D4, 0, NOTE_E4, 0, NOTE_G4, 0, NOTE_A4, 0, NOTE_C5, 0};

void playTones(int tonePin, const int tones[]) {
  for (int thisNote = 1; thisNote < (tones[0] * 2 + 1); thisNote = thisNote + 2) {
    tone(tonePin, tones[thisNote], (1000 / tones[thisNote + 1]));
    delay((1000 / tones[thisNote + 1]) * 1.30);
    noTone(tonePin);
  }
}

void playStartup(int tonePin) {
  playTones(tonePin, notes_MarioStartup);
}

void playGameOver(int tonePin) {
  playTones(tonePin, notes_MarioGameOver);
}

void playBeegees() {
  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE); // Turn on the other LEDs until you release button

  setLEDs(CHOICE_NONE); // Turn off LEDs

  for (int i = 0; i < 2; i++)
  {
    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 32; thisNote++) {
      changeLED();
      tone(BUZZER_PIN, beegeesMelody[thisNote],noteDuration);
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(BUZZER_PIN);
    }
  }

  setLEDs(CHOICE_NONE);
}

// Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms.
void buzz_sound(int buzz_length_ms, int buzz_delay_us)
{
  // Convert total play time from milliseconds to microseconds
  long buzz_length_us = buzz_length_ms * (long)1000;

  // Loop until the remaining play time is less than a single buzz_delay_us
  while (buzz_length_us > (buzz_delay_us * 2))
  {
    buzz_length_us -= buzz_delay_us * 2; //Decrease the remaining play time

    // Toggle the buzzer at various speeds
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(buzz_delay_us);

    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(buzz_delay_us);
  }
}
