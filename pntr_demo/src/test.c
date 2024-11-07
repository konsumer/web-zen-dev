#include "utils.h"

static FILE* fb;
static pntr_image* screen;

// draw some random circles
void drawCircles() {
  static int x[3] = {0, 0, 0};
  static int y[3] = {0, 0, 0};

  // colors used for circles
  pntr_color colors[3] = {PNTR_BLUE, PNTR_GREEN, PNTR_RED};

  for (int i = 0; i < 3; i++) {
    x[i] = rando() % SCREEN_WIDTH + 40;
    y[i] = rando() % SCREEN_HEIGHT + 40;
    pntr_draw_circle_fill(screen, x[i], y[i], 80, colors[i]);
  }
}

// this will be called by host when it's ready to update
__attribute__((export_name("update"))) void update() {
  pntr_clear_background(screen, PNTR_RAYWHITE);
  drawCircles();
  drawFps(screen);
  pntr_draw_text(screen, pntr_load_font_default(), "If you can see this, it's working!", 180, SCREEN_HEIGHT / 2, PNTR_BLACK);

  // write screen to framebuffer
  if (fb != NULL) {
    fwrite(screen->data, sizeof(char), SCREEN_WIDTH * SCREEN_HEIGHT * 4, fb);
  }
}

int main() {
  printf("Hi, from C.\n");
  screen = pntr_new_image(SCREEN_WIDTH, SCREEN_HEIGHT);

  fb = fopen("/dev/fb0", "wb");
  if (fb == NULL) {
    fprintf(stderr, "Error opening framebuffer\n");
    return 1;
  }

  return 0;
}
