// small utils-layer for making the main source a bit simpler.

// setup pntr
#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_PIXELFORMAT_RGBA
#define PNTR_IMPLEMENTATION
#include "pntr.h"

#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// int to string
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
  
    // Reverse the string
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

// fake random
int rando() {
  static unsigned long long int x = 4694882287132787932;
  return x ^= x << 10, x ^= x >> 13, (int)(x ^= x >> 10);
}

// draw the FPS-counter
void drawFps(pntr_image* screen){
  static int frameCount = 0;
  static time_t previousTime = 0;
  static time_t currentTime = 0;
  static int fps = 0;
  static char* fpsText = "999999";

  frameCount++;
  currentTime = time(NULL);
  if ( currentTime - previousTime >= 1.0 ) {
    previousTime = currentTime;
    fps = frameCount;
    frameCount = 0;
  }
  itoa(fps, fpsText, 10);
  pntr_draw_text(screen, pntr_load_font_default(), fpsText, 5, 5, PNTR_BLACK);
}