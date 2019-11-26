# tinyscreen-interrupt

TinyScreenPlus interrupt example code.
Go to sleep and wake up when buttons are pressed.


# Extending the code

The example interrupt implementation is in [tinyscreen-interrupt-lib.ino](./tinyscreen-interrupt-lib.ino) for your reference.
To write your program you can change just the file [tinyscreen-interrupt.ino](./tinyscreen-interrupt.ino), in particular you can add to `onButtonPress`.

Configuration of the screen timeout is given as the first argument to the `Screen_setup` function.


# Sleep side effects

You need to wake up the processor (press buttons) to be able to reflash it.
Of course, you can always just use the reset switch.

Once the processor goes to sleep, the tty communication stops working.


# Compiling and running

The [Justfile](https://github.com/casey/just) has compilation, etc commands using the arduino-cli.