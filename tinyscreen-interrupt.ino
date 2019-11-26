#include <TinyScreen.h>
#include <RTCZero.h>

#define DEBUG 1

void setup() {
    Screen_setup(5, &setup_screen, &onButtonPress);
}

void loop() {
    Screen_loop();
}

void onButtonPress(TinyScreen display, RTCZero rtc) {
    byte buttons = display.getButtons();
    display.on();
}

void setup_screen(TinyScreen display) {
	// display.setFlip(true);
}