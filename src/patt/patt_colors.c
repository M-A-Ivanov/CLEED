#include "patt.h"
#include "patt_colors.h"
#include "strings.h"
#include <stdbool.h>

const char colors[NUM_COLORS][2][30] = {
      {"black", "dark gray"}, {"dark red", "red"},
      {"blue", "light blue"}, {"dark green", "green"},
      {"dark orange", "orange"}, {"gold", "yellow"}
};
  
const patt_color_rgb_t grays[NUM_GRAYS] = {
    {0.1, 0.1, 0.1}, {0.2, 0.2, 0.2}, {0.3, 0.3, 0.3},
    {0.4, 0.4, 0.4}, {0.5, 0.5, 0.5}, {0.6, 0.6, 0.6},
    {0.7, 0.7, 0.7}, {0.8, 0.8, 0.8}, {0.9, 0.9, 0.9}
};

const patt_color_rgb_t PATT_BLACK = {0, 0, 0};
const patt_color_rgb_t PATT_RED = {1, 0, 0};
const patt_color_rgb_t PATT_BLUE = {0, 1, 0};
const patt_color_rgb_t PATT_GREEN = {0, 1, 0};
const patt_color_rgb_t PATT_MAGENTA = {1, 0, 1};
const patt_color_rgb_t PATT_LIGHT_BLUE = {0, 1, 1};
const patt_color_rgb_t PATT_ORANGE = {1, 0.7, 0};
const patt_color_rgb_t PATT_CYAN = {0.08, 0.92, 0.92};
const patt_color_rgb_t PATT_YELLOW = {1, 1, 0};
const patt_color_rgb_t PATT_PURPLE = {0.7, 0.3, 1};
const patt_color_rgb_t PATT_GRAY = {0.5, 0.5, 0.5};
const patt_color_rgb_t PATT_DARK_RED = {0.7, 0.1, 0};
const patt_color_rgb_t PATT_DARK_CYAN = {0, 0.67, 0.67};
const patt_color_rgb_t PATT_DARK_GRAY = {0.25, 0.25, 0.25};
const patt_color_rgb_t PATT_DARK_GREEN = {0, 0.5, 0};
const patt_color_rgb_t PATT_DARK_ORANGE = {1, 0.7, 0};
const patt_color_rgb_t PATT_GOLD = {0.83, 0.83, 0.17};
const patt_color_rgb_t PATT_BROWN = {0.7, 0.3, 0};
const patt_color_rgb_t PATT_WHITE = {1, 1, 1};

const patt_color_rgb_t PATT_EDGE_COLOR = {0.55, 1., 0.55};
const patt_color_rgb_t PATT_SCREEN_COLOR = {0.85, 1., 0.85};
const patt_color_rgb_t PATT_GUN_COLOR = {0., 0., 0.};

void patt_color_copy(patt_color_rgb_t *new_color,
                    const patt_color_rgb_t *old_color)
{
  new_color->red = old_color->red;
  new_color->blue = old_color->blue;
  new_color->green = old_color->green;
}

bool patt_color_is_equal(const patt_color_rgb_t *dst,
                         const patt_color_rgb_t *src)
{
  if ((dst->red == src->red) &&
      (dst->blue == src->blue) &&
      (dst->green == src->green)) return(true);

  return(false);
}

patt_color_rgb_t *patt_color_from_name(const char *color)
{
  patt_color_rgb_t *patt_color;
  if (strcasecmp(color, "red") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_RED;
  }
  else if (strcasecmp(color, "blue") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_BLUE;
  }
  else if (strcasecmp(color, "green") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_GREEN;
  }
  else if (strcasecmp(color, "magenta") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_MAGENTA;
  }
  else if (strcasecmp(color, "light blue") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_LIGHT_BLUE;
  }
  else if (strcasecmp(color, "orange") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_ORANGE;
  }
  else if (strcasecmp(color, "cyan") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_CYAN;
  }
  else if (strcasecmp(color, "yellow") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_YELLOW;
  }
  else if (strcasecmp(color, "purple") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_PURPLE;
  }
  else if (strcasecmp(color, "gray") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_GRAY;
  }
  else if (strcasecmp(color, "dark cyan") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_DARK_CYAN;
  }
  else if (strcasecmp(color, "dark gray") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_DARK_GRAY;
  }
  else if (strcasecmp(color, "dark green") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_DARK_GREEN;
  }
  else if (strcasecmp(color, "dark orange") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_DARK_ORANGE;
  }
  else if (strcasecmp(color, "dark red") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_DARK_RED;
  }
  else if (strcasecmp(color, "gold") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_GOLD;
  }
  else if (strcasecmp(color, "brown") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_BROWN;
  }
  else if (strcasecmp(color, "white") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_WHITE;
  }
  else if (strcasecmp(color, "black") == 0) {
    patt_color = (patt_color_rgb_t*) &PATT_BLACK;
  }
  else {
    patt_color = NULL;
  }
  return(patt_color);
}

const char *patt_color_get_name(const patt_color_rgb_t *color)
{
  if (patt_color_is_equal(color, &PATT_RED)) {
    return("red");
  }
  else if (patt_color_is_equal(color, &PATT_BLUE)) {
    return("blue");
  }
  else if (patt_color_is_equal(color, &PATT_GREEN)) {
    return("green");
  }
  else if (patt_color_is_equal(color, &PATT_MAGENTA)) {
    return("magenta");
  }
  else if (patt_color_is_equal(color, &PATT_LIGHT_BLUE)) {
    return("light blue");
  }
  else if (patt_color_is_equal(color, &PATT_ORANGE)) {
    return("orange");
  }
  else if (patt_color_is_equal(color, &PATT_CYAN)) {
    return("cyan");
  }
  else if (patt_color_is_equal(color, &PATT_YELLOW)) {
    return("yellow");
  }
  else if (patt_color_is_equal(color, &PATT_PURPLE)) {
    return("purple");
  }
  else if (patt_color_is_equal(color, &PATT_GRAY)) {
    return("gray");
  }
  else if (patt_color_is_equal(color, &PATT_DARK_CYAN)) {
    return("dark cyan");
  }
  else if (patt_color_is_equal(color, &PATT_DARK_GRAY)) {
    return("dark gray");
  }
  else if (patt_color_is_equal(color, &PATT_DARK_GREEN)) {
    return("dark green");
  }
  else if (patt_color_is_equal(color, &PATT_DARK_ORANGE)) {
    return("dark orange");
  }
  else if (patt_color_is_equal(color, &PATT_DARK_RED)) {
    return("dark red");
  }
  else if (patt_color_is_equal(color, &PATT_GOLD)) {
    return("gold");
  }
  else if (patt_color_is_equal(color, &PATT_BROWN)) {
    return("brown");
  }
  else if (patt_color_is_equal(color, &PATT_WHITE)) {
    return("white");
  }
  else if (patt_color_is_equal(color, &PATT_BLACK)) {
    return("black");
  }

  return("unknown");
}
