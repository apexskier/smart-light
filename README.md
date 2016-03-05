# Smart Light

I decided to put an Arduino inside my touch lamp.

My lamp orginally had three levels of lighting and an off state. Touching
cycled through these. The most basic thing I wanted was to turn off if I hadn't
touched in a while, so I don't blind myself when turning my light off before
bed.

I ended up deciding on using an LED, and then decided on an RGB LED. This means
I can do color, so I decided to turn it into a [wake up
light](https://www.usa.philips.com/c-m-li/light-therapy/wake-up-light/latest#filters=WAKEUP_LIGHT_SU&sliders=&support=&price=&priceBoxes=&page=&layout=12.subcategory.p-grid-icon).

## Dev notes

I increased the [number of possible
alarms](https://github.com/PaulStoffregen/TimeAlarms/blob/master/TimeAlarms.h#L10)
to 14.

To generate html, I edit `index.html`, then run `./html.sh`. This overwrites
the first two lines of `server.ino`.
