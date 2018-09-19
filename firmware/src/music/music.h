#ifndef PARTICLE_MUSIC_H
#define PARTICLE_MUSIC_H

void playTones(int tonePin, const int tones[]);
void playStartup(int tonePin);
void playGameOver(int tonePin);
void playBeegees();
void buzz_sound(int buzz_length_ms, int buzz_delay_us);

#endif
