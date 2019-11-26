# tinyscreen-interrupt

TinyScreenPlus interrupt example code.
Go to sleep and wake up when buttons are pressed.

The Justfile has compilation, etc commands using the arduino-cli


# Extending the code

Set these two variables
* keepScreenOnForSeconds
* buttonCallback

You can use buttonCallback to implement your program.
Your buttonCallback is responsible for calling display.on()


# Sleep side effects
You need to wake up the processor (press buttons) to be able to reflash it.
Of course, you can always just use the reset switch.

Once the processor goes to sleep, the tty communication stops working.