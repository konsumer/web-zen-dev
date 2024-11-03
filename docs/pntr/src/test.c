#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_PIXELFORMAT_RGBA
#define PNTR_IMPLEMENTATION
#include "pntr.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

FILE* fb;
pntr_image* screen;
int x[3] = {0,0,0};
int y[3] = {0,0,0};

int frameCount = 0;
time_t previousTime = 0;
time_t currentTime = 0;
int fps = 0;
pntr_font* defaultFont;
char fpsText[3];

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

void updatePositions(){
  // only update every 20 frames
  if (frameCount % 20 == 0) {
    x[0] = rando() % SCREEN_WIDTH + 40;
    x[1] = rando() % SCREEN_WIDTH+ 40;
    x[2] = rando() % SCREEN_WIDTH+ 40;
    y[0] = rando() % SCREEN_HEIGHT+ 40;
    y[1] = rando() % SCREEN_HEIGHT+ 40;
    y[2] = rando() % SCREEN_HEIGHT+ 40;
  }
}

void updateFps(){
  frameCount++;
  currentTime = time(NULL);
  if ( currentTime - previousTime >= 1.0 ) {
    previousTime = currentTime;
    fps = frameCount;
    frameCount = 0;
  }
  itoa(fps, &fpsText, 10);
  pntr_draw_text(screen, defaultFont, &fpsText, 5, 5, PNTR_BLACK);
}

// this will be called by host when it's ready to update
__attribute__((export_name("update")))
void update() {
  pntr_clear_background(screen, PNTR_RAYWHITE);
  pntr_draw_circle_fill(screen, x[0], y[0], 80, PNTR_BLUE);
  pntr_draw_circle_fill(screen, x[1], y[1], 80, PNTR_GREEN);
  pntr_draw_circle_fill(screen, x[2], y[2], 80, PNTR_RED);

  updateFps();
  updatePositions();

  // write screen to framebuffer
  if (fb != NULL) {
    fwrite(screen->data, sizeof(char), SCREEN_WIDTH*SCREEN_HEIGHT*4, fb);
  }
}

int main() {
  defaultFont = pntr_load_font_default();

  printf("Hi, from C.\n");
  screen = pntr_new_image(SCREEN_WIDTH, SCREEN_HEIGHT);

  fb = fopen("/dev/fb0", "wb");
  if (fb == NULL) {
    fprintf(stderr, "Error opening framebuffer\n");
    return 1;
  }

  return 0;
}
