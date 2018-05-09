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

 // Credit:
// Midi to Arduino Converter
//     - Andy Tran (extramaster), 2015
// https://www.extramaster.net/tools/midiToArduino/
//
// Process:
// Midi -> Midi tracks -> Note mappings -> Frequency
//
// CC0

void playStartup(int tonePin) {
  tone(tonePin, 110, 34.8837209302);
  delay(38.7596899225);
  delay(19.3798449612);
  tone(tonePin, 110, 26.1627906977);
  delay(29.0697674419);
  delay(9.68992248062);
  delay(38.7596899225);
  delay(9.68992248062);
  tone(tonePin, 82, 17.4418604651);
  delay(19.3798449612);
  delay(19.3798449612);
  tone(tonePin, 1479, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 92, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 195, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 783, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 391, 8.72093023256);
  delay(9.68992248062);
  delay(38.7596899225);
  delay(9.68992248062);
  tone(tonePin, 92, 8.72093023256);
  delay(9.68992248062);
  delay(9.68992248062);
  tone(tonePin, 1174, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 220, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 110, 8.72093023256);
  delay(9.68992248062);
  delay(29.0697674419);
  delay(9.68992248062);
  delay(9.68992248062);
  tone(tonePin, 932, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 87, 8.72093023256);
  delay(9.68992248062);
  delay(9.68992248062);
  tone(tonePin, 1567, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 587, 34.8837209302);
  delay(38.7596899225);
  tone(tonePin, 61, 8.72093023256);
  delay(9.68992248062);
  delay(9.68992248062);
  delay(9.68992248062);
  tone(tonePin, 87, 8.72093023256);
  delay(9.68992248062);
  delay(58.1395348837);
  delay(9.68992248062);
  tone(tonePin, 92, 8.72093023256);
  delay(9.68992248062);
  delay(38.7596899225);
  tone(tonePin, 87, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 103, 34.8837209302);
  delay(38.7596899225);
  delay(9.68992248062);
  tone(tonePin, 87, 17.4418604651);
  delay(19.3798449612);
  delay(19.3798449612);
  tone(tonePin, 415, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 103, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 1244, 43.6046511628);
  delay(48.4496124031);
  delay(9.68992248062);
  tone(tonePin, 61, 26.1627906977);
  delay(29.0697674419);
  delay(19.3798449612);
  tone(tonePin, 65, 8.72093023256);
  delay(9.68992248062);
  delay(9.68992248062);
  tone(tonePin, 92, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 622, 34.8837209302);
  delay(38.7596899225);
  delay(19.3798449612);
  delay(9.68992248062);
  tone(tonePin, 195, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 587, 8.72093023256);
  delay(9.68992248062);
  delay(38.7596899225);
  tone(tonePin, 110, 17.4418604651);
  delay(19.3798449612);
  delay(19.3798449612);
  delay(9.68992248062);
  tone(tonePin, 92, 17.4418604651);
  delay(19.3798449612);
  delay(77.519379845);
  delay(19.3798449612);
  tone(tonePin, 932, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 103, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 195, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 466, 17.4418604651);
  delay(19.3798449612);
  delay(29.0697674419);
  tone(tonePin, 103, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 587, 34.8837209302);
  delay(38.7596899225);
  delay(19.3798449612);
  tone(tonePin, 195, 34.8837209302);
  delay(38.7596899225);
  delay(9.68992248062);
  tone(tonePin, 195, 17.4418604651);
  delay(19.3798449612);
  tone(tonePin, 698, 8.72093023256);
  delay(9.68992248062);
  tone(tonePin, 932, 130.813953488);
  delay(145.348837209);
}
