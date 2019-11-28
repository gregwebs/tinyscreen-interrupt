#include <TinyScreen.h>
#include <RTCZero.h>

#define DEBUG 1

bool displayOn = false;

void onButtonPress(TinyScreen display, RTCZero rtc) {
    byte buttons = display.getButtons();
    displayOn = true;
    display.on();
}

void setup() {
  TinyScreen screen = Screen_setup(5, &onButtonPress);
  setup_screen(screen);
}

void setup_screen(TinyScreen display) {
  // display.setFlip(true);
}

void loop() {
    Screen_loop();
    displayOn = false;
    Screen_sleep();
}