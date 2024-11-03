#define PNTR_PIXELFORMAT_RGBA
#define PNTR_IMPLEMENTATION
#include "pntr.h"

pntr_image* screen;


__attribute__((export_name("update")))
pntr_color* update() {
  pntr_clear_background(screen, PNTR_RAYWHITE);
  pntr_draw_circle_fill(screen, 100, 100, 80, PNTR_BLUE);
  pntr_draw_circle_fill(screen, 200, 200, 80, PNTR_GREEN);
  pntr_draw_circle_fill(screen, 300, 300, 80, PNTR_RED);

  return screen->data;
}

int main() {
  printf("Hi, from C.\n");
  screen = pntr_new_image(640, 480);
  return 0;
}