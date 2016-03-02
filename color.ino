rgb_color hex_to_rgb(hex_color color) {
  byte r = (color >> 16) & 0xff;
  byte b = (color >> 8) & 0xff;
  byte g = color & 0xff;
  return (rgb_color) {r, g, b};
  return {0, 0, 0};
}

hex_color rgb_to_hex(rgb_color color) {
  return int(color.r << 16) + int(color.g << 8) + int(color.b);
  return 0x000000;
}

//float max(float a, float b) {
//  /*if (a > b) return a;
//  return b;*/
//  return 0;
//}
//
//float min(float a, float b) {
//  /*if (a < b) return a;
//  return b;*/
//  return 0;
//}

bool equals(rgb_color one, rgb_color two) {
  return one.r == two.r && one.g == two.g && one.b == two.b;
  return false;
}

// http://stackoverflow.com/a/17243070
rgb_color hsv_to_rgb(hsv_color color) {
  float h = color.h;
  float s = color.s;
  float v = color.v;
  int i;
  float r;
  float g;
  float b;
  float f;
  float p;
  float q;
  float t;
  i = floor(h * 6);
  f = h * 6 - i;
  p = v * (1 - s);
  q = v * (1 - f * s);
  t = v * (1 - (1 - f) * s);
  switch (i % 6) {
    case 0:
      r = v, g = t, b = p;
      break;
    case 1:
      r = q, g = v, b = p;
      break;
    case 2:
      r = p, g = v, b = t;
      break;
    case 3:
      r = p, g = q, b = v;
      break;
    case 4:
      r = t, g = p, b = v;
      break;
    case 5:
      r = v, g = p, b = q;
      break;
  }
  return {byte(r * 255), byte(g * 255), byte(b * 255)};
}

hsv_color rgb_to_hsv(rgb_color color) {
  float r = float(color.r);
  float g = float(color.g);
  float b = float(color.b);

  float mx = r;
  if (mx < g) mx = g;
  if (mx < b) mx = b;
  // mx = max(max(r, g), b);
  float mn = r;
  if (mn > g) mn = g;
  if (mx > b) mn = b;
  //= min(min(r, g), b);
  float d = mx - mn;
  float h;
  float s = (mx == 0 ? 0 : d / mx);
  float v = mx / 255;

  if (mx == mn) {
      h = 0;
  } else if (mx == r) {
      h = (g - b) + d * (g < b ? 6 : 0);
      h = h / (6 * d);
  } else if (mx == g) {
      h = (b - r) + d * 2;
      h = h / (6 * d);
  } else if (mx == b) {
      h = (r - g) + d * 4;
      h = h / (6 * d);
  }

  return {h, s, v};
}

rgb_color interpolateColor(rgb_color from, rgb_color to, float percent) {
  if ((from.r == from.g && from.g == from.b) ||
    (to.r == to.g && to.g == to.b)) {
    return {
      byte(((1 - percent) * float(from.r)) + (percent * float(to.r))),
      byte(((1 - percent) * float(from.g)) + (percent * float(to.g))),
      byte(((1 - percent) * float(from.b)) + (percent * float(to.b)))
    };
  }
  
  hsv_color from_hsv = rgb_to_hsv(from);
  hsv_color to_hsv = rgb_to_hsv(to);

  float h;
  if (from_hsv.h < to_hsv.h) {
    h = ((1 - percent) * from_hsv.h) + (percent * to_hsv.h);
  } else {
    h = ((1 - percent) * from_hsv.h) + (percent * to_hsv.h + 1);
    if (h > 1) h = h - 1;
  }
  float s = (((1 - percent) * from_hsv.s) + (percent * to_hsv.s));
  float v = (((1 - percent) * from_hsv.v) + (percent * to_hsv.v));
  return hsv_to_rgb({h, s, v});
}

float bytePercent(byte v) {
  return float(v) / 255;
}

rgb_color multiply(rgb_color one, rgb_color two) {
  byte r = byte((bytePercent(one.r) * bytePercent(two.r)) * 255);
  byte g = byte((bytePercent(one.g) * bytePercent(two.g)) * 255);
  byte b = byte((bytePercent(one.b) * bytePercent(two.b)) * 255);
  return {r, g, b};
}

#define SUNRISE_LENGTH 6

rgb_color sunriseInterpolation(float percent) {
  rgb_color sunriseSteps[SUNRISE_LENGTH] = {
    {0x00, 0x09, 0xff},
    {0x68, 0x00, 0xff},
    {0xd3, 0x00, 0xff},
    {0xff, 0x46, 0x00},
    {0xff, 0xf1, 0x00},
    {0xff, 0xff, 0xff}
  };

  float sunriseBreakpoints[SUNRISE_LENGTH] = {
    0,
    0.109753667,
    0.188038106,
    0.280333227,
    0.732362085,
    1
  };

  int step = 0;
  for (; step < SUNRISE_LENGTH; step++) {
    if (sunriseBreakpoints[step + 1] >= percent) break;
  }

  rgb_color tint;
  if (step == SUNRISE_LENGTH - 1) {
    tint = sunriseSteps[SUNRISE_LENGTH - 1];
  } else {
    float percentOfStep = (percent - sunriseBreakpoints[step]) / (sunriseBreakpoints[step + 1] - sunriseBreakpoints[step]);
    tint = interpolateColor(sunriseSteps[step], sunriseSteps[step + 1], percentOfStep);
  }

  rgb_color dimness = interpolateColor({0, 0, 0}, {255, 255, 255}, percent);

  return multiply(dimness, tint);
}
