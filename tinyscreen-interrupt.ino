#include <TinyScreen.h>
#include <RTCZero.h>

#define DEBUG 1

void onButtonPress(TinyScreen display, RTCZero rtc) {
    byte buttons = display.getButtons();
    display.on();
}

void setup() {
    Screen_setup(1, &onButtonPress);
}

void loop() {
    Screen_loop();
}