# tinyscreen-interrupt

TinyScreenPlus interrupt example code.
Go to sleep and wake up when buttons are pressed.

The Justfile has compilation, etc commands using the arduino-cli


# Extending the code

Set this variable
* keepScreenOnForSeconds

Change the function onButtonPress to implement your program.


# Sleep side effects

You need to wake up the processor (press buttons) to be able to reflash it.
Of course, you can always just use the reset switch.

Once the processor goes to sleep, the tty communication stops working.