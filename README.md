# tty_ioctl

Control DTR and RTS signals on tty (terminal) devices.

This small program can be used to run small control listings - basically small scripts - that control the behaviour of the DTR and RTS signals on a serial hardware device.

# Building

Use the Makefile or just run "`gcc tty_ioctl.c -o tty_ioctl`".

in a fitting development environemnt. The program was developed for Linux but will propably compile and work on MacOS and other *nix systems and maybe event crosscompile on Windows.

# Usage

```
tty_ioctl <port> [-vv] <R,r,D,d,B,b,S,s,l>
```

The first argument needs to be the tty-port e.g. `/dev/ttyUSB0`.

The next argument is optional and sets the verbosity level.

|Value|Level|Verbosity|
|---|---|---|
|no `-v`|0|Show only fatal error messages.
|`-v`|1|Show only message related to DTR or RTS singal changes or fatal/general errors.
|`-vv`|2|Show all messages including those related to program control commands such as sleeping or looping.

All further arguments are control scripts. Each letter in a control script makes the script perform a certain action. Then the script continues to the next letter until the end of the commands are reached. The program then exits reporting success execution.

## Commands

|Letter|Effect|
|---|---|
|R|Set *RTS* signal high.|
|r|Set *RTS* signal low.|
|D|Set *DTS* signal high.|
|d|Set *DTS* signal low.|
|B|Set both *RTS* and *DTS* signal high.|
|b|Set both *RTS* and *DTS* signal low.|
|S|Sleep for *500ms* = *0.5s*, then continue with the next command.|
|s|Sleep for *100ms* = *0.1s*, then continue with the next command.|
|l|Loop back to the beginning of the last argument.

## *Loop* command

The loop command runs the program indefinitely, repeating the commands of the last argument until the script is interrupted by a *SIGINT* signal. It can be send for example via `kill` or by pressing `CTRL-C` in a terminal running the program.

The program will exit instantly after the *SIGINT* was received.

# Examples

```
tty_ioctl /dev/ttyUSB0 DSd
```

Set the *DTR* signal high, wait for half a seconds, then set the signal to low and exit. This command can be used to reset most microcontroller dev-boards that use a UART for programming since most of these boards control the *reset*/*run*-line of their microcontroller using the *DTR* signal of the UART.

```
tty_ioctl /dev/ttyUSB0 -vv DrssssdRsl
```

Quickly switches between signal *DTR* being set to high and *RTS* to low (`Br`), pausing for 400ms (`ssss`), then switching the signals states around (`bR`) and pausing another 100ms (`s`). The the script repeats this sequence indefinitely due to the final `l` and needs to be stopped with a *SIGINT*.

This creates a 2 Hz PWM signal on both line, the *DTR* signal being 80% and the *RTS* signal being 20%.

```
tty_ioctl /dev/ttyUSB0 -vv dsDsdsDsdsDSSSl
```

Attaching an LED to the *DTR* signal pin on the `ttyUSB0` hardware will result in the LED blinking three times every two seconds and creates a simple way to indicate a hardware state to the user by just looking at the LED.