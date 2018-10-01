#ifndef PARTICLE_DISPLAY_H
#define PARTICLE_DISPLAY_H

void displayCarousel();
void resetDisplayBools();
void messageBoxWithArray(const char *textStrings[], int arrayLen, int textSize);
void clearScreen();
void showSplashscreen();
void displayWearerDetails();
void displayTwitterHandle();
void showTitle();
void showTempAndHumidity();
void showBattery();
void displayCredits();
void showSpark();
void showKonami();
void showMesh();
void showSpectra();
void displayMeshPing();
void displayGatewayPing(const char *data);
void displayHiFromZ();
void displayMeshStatus(const char *status);

#endif
