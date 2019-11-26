//-------------------------------------------------------------------------------
//  TinyCircuits TinyScreen+ Basic Sleep Example
//  Last Updated 9 June 2017
//
//  This example shows how to use external interrupts to wake from sleep mode.
//
//  Written by Ben Rose, TinyCircuits http://TinyCircuits.com
//
//-------------------------------------------------------------------------------
#include <SPI.h>
#include <TinyScreen.h>
TinyScreen display = TinyScreen(TinyScreenPlus);

#define SerialMonitorInterface SerialUSB
#include <RTCZero.h>
RTCZero rtc;

unsigned long interruptTime = 0;
unsigned long millisOffsetCount = 0;

uint32_t millisOffset() {
#if defined (ARDUINO_ARCH_AVR)
  return millis();
#elif defined(ARDUINO_ARCH_SAMD)
  return (millisOffsetCount * 1000ul) + millis();
#endif
}

uint32_t sleepTime = 0;

void RTCwakeHandler() {
  msg("rtc wake");
  if (sleepTime == 0) { return; }
  debugDisplay("RTC Wake");
  millisOffsetCount += (rtc.getEpoch() - sleepTime);
  sleepTime = 0;
}

void (*buttonCallbackPtr)(TinyScreen, RTCZero) = NULL;

void buttonHandler() {
  msg("button press");
  debugDisplay("button");
  interruptTime = millisOffset();
  if (buttonCallbackPtr != NULL) {
    (*buttonCallbackPtr)(display, rtc);
  } else {
    display.on();
  }
}

void msg(const String arg) {
  SerialMonitorInterface.println(arg);
}

// Change this to change how long the screen stays on
int keepScreenOnForSeconds = 1;

void Screen_setup(int screenTimeout, void(*setButtonCallback)(TinyScreen, RTCZero))
{
  keepScreenOnForSeconds = screenTimeout;
  buttonCallbackPtr = setButtonCallback;

  for (int i = 0; i < 27; i++) {
    if ((i != PIN_USB_DM) && (i != PIN_USB_DP)) {
      pinMode(i, INPUT_PULLUP);
    }
  }

  display.begin();

  rtc.begin();
  rtc.attachInterrupt(RTCwakeHandler);

  attachInterrupt(TSP_PIN_BT1, buttonHandler, FALLING);
  attachInterrupt(TSP_PIN_BT2, buttonHandler, FALLING);
  attachInterrupt(TSP_PIN_BT3, buttonHandler, FALLING);
  attachInterrupt(TSP_PIN_BT4, buttonHandler, FALLING);

  SerialMonitorInterface.begin(115200);


#if defined(ARDUINO_ARCH_SAMD)
  // https://github.com/arduino/ArduinoCore-samd/issues/142
  // Clock EIC in sleep mode so that we can use pin change interrupts
  // The RTCZero library will setup generic clock 2 to XOSC32K/32
  // and we'll use that for the EIC. Increases power consumption by ~50uA
  GCLK->CLKCTRL.reg = uint16_t(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 | GCLK_CLKCTRL_ID( GCLK_CLKCTRL_ID_EIC_Val ) );
  while (GCLK->STATUS.bit.SYNCBUSY) {}
#endif
  debugDisplay("Setup");
  delay(1000);
  interruptTime = millisOffset();
}

void Screen_loop() {
  // While we were sleeping, a button may have been pressed
  // This will set interruptTime
  while (interruptTime != 0) {
    // Account for time taken time servicing the interrupt
    long elapsed = millisOffset() - interruptTime;
    long timeout = (long)keepScreenOnForSeconds*1000ul;
    interruptTime = 0;

    if (elapsed < timeout) {
      delay(timeout - elapsed);
    }
  }

  sleepTime = rtc.getEpoch();
  msg("display off");
  display.off();
  rtc.standbyMode();
}

void debugDisplay(String msg) {
#if defined(DEBUG)
  display.setCursor(1, 50);
  display.setFont(thinPixel7_10ptFontInfo);
  display.print(msg);
#else
  return;
#endif
}