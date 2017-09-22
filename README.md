#srtos
This is a simple multitasking OS for small foot print micro-controllers.
**features** includes setting thread priority, synchronisation objects  and power management.

##Target audience
 Those who write bare metal code but still need little help from a scheduler. 
 
 

## Code Example
**x86 emulation**
   app/projects/test/main_app.c

**microcontroler srtos   examples**

Atmel AT90USB1286  Teensy 2.0 ++
samples/mega-avr/teensy_2.0_++/srtos-atmel.zip. 

## Release
Not released yet. Current version is available for testing only.

## Motivation
I have used similar  techniques couple of years ago to do  multitasking for 8bit microcontroller based  hobby projects.
Now, added synchronization mechanism to the scheduler so that  it can function  as an OS with minimum feature set. 

## HOWTO Run PC emulator

**x86 simulation**
Instructions (Tested in Ubuntu and Mac )
1. git clone https://github.com/sajithkamal/s-rtos.git
2. make
3. ./a.out

## Running Sample code in AVR microcontroller.
Atmel AT90USB1286  Teensy 2.0 ++
1. Unzip samples/mega-avr/teensy_2.0_++/srtos-atmel.zip. 

2. Compile the project  using atmel studio 7. 

3. flash the code using teensy flasher application  

4. Connect FTDI cable to uart1 

## Documentation



## License
Apache 2.0

