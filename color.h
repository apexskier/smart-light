// Color types
// Each component should be from 0 to 255
struct rgb_color {
    byte r;
    byte g;
    byte b;
};

struct hsv_color {
    float h;
    float s;
    float v;
};

typedef unsigned long hex_color;

union generic_color {
  struct rgb_color rgb;
  hex_color hex;
};

rgb_color multiply(rgb_color one, rgb_color two);
rgb_color interpolateColor(rgb_color from, rgb_color to, float percent);

float bytePercent(byte val);

bool equals(rgb_color one, rgb_color two);

rgb_color hex_to_rgb(hex_color color);
hex_color rgb_to_hex(rgb_color color);
rgb_color hsv_to_rgb(hsv_color color);
hsv_color rgb_to_hsv(rgb_color color);

rgb_color sunriseInterpolation(float percent);
