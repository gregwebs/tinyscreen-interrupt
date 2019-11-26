#include <TinyScreen.h>


void onButtonPress(TinyScreen display) {
    display.on();
}

void setup() {
    Screen_setup(&onButtonPress);
}

void loop() {
    Screen_loop();
}