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

int sleepTimeout = 1;
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

void buttonHandler() {
  msg("button press");
  debugDisplay("button");
  interruptTime = millisOffset();
}

void msg(const String arg) {
  SerialMonitorInterface.println(arg);
}


void setup(void)
{
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
}

void debugDisplay(String msg) {
  display.clearScreen();
  display.setCursor(10, 10);
  display.setFont(thinPixel7_10ptFontInfo);
  display.print(msg);
}

void loop() {
  display.on();
  delay(sleepTimeout);
  // While we were sleeping, a button may have been pressed
  // This will set interruptTime
  while (interruptTime != 0) {
    // Account for time taken time servicing the interrupt
    long elapsed = millisOffset() - interruptTime;
    long timeout = (long)sleepTimeout*1000ul;
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

void hardwareDrawCommands(){
  //Accelerated drawing commands are executed by the display controller
  //clearScreen();//clears entire display- the same as clearWindow(0,0,96,64)
  display.clearScreen();
  //drawRect(x stary, y start, width, height, fill, 8bitcolor);//sets specified OLED controller memory to an 8 bit color, fill is a boolean- TSRectangleFilled or TSRectangleNoFill
  display.drawRect(10,10,76,44,TSRectangleFilled,TS_8b_Red);
  //drawRect(x stary, y start, width, height, fill, red, green, blue);//like above, but uses 6 bit color values. Red and blue ignore the LSB.
  display.drawRect(15,15,66,34,TSRectangleFilled,20,30,60);
  //clearWindow(x start, y start, width, height);//clears specified OLED controller memory
  display.clearWindow(20,20,56,24);
  //drawLine(x1, y1, x2, y2, 8bitcolor);//draw a line from (x1,y1) to (x2,y2) with an 8 bit color
  display.drawLine(0,0,95,63,TS_8b_Green);
  //drawLine(x1, y1, x2, y2, red, green, blue);//like above, but uses 6 bit color values. Red and blue ignore the LSB.
  display.drawLine(0,63,95,0,0,63,0);
  delay(1000);
  //use 16 bit version of drawLine to fade a rectangle from blue to green:
  for(int i=0;i<64;i++){
    display.drawLine(0,i,95,i,0,i,63-i);
  }
  delay(1000);
}