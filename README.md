#srtos
This is a simple real time operating system (RTOS) for small foot print micro-controllers.
**features** includes setting thread priority, synchronisation objects  and power management. 

## Code Example
**x86 simulation**
   app/projects/test/main_app.c

**microcontroler srtos   examples**

Atmel AT90USB1286  Teensy 2.0 ++
samples/mega-avr/teensy_2.0_++/srtos-atmel.zip. 

## Release
TODO

## Motivation

Often thought that it is desirable to have an RTOS with minimal over head to setup,  when developing small low footprint microcontroller programs.
The idea is to include 2-3 files to your makefile and it should be ready to go.  

## HOWTO 

**x86 simulation**
Instructions (Tested in Ubuntu and Mac )
1. git clone https://github.com/sajithkamal/s-rtos.git
2. make
3 ./a.out


Atmel AT90USB1286  Teensy 2.0 ++
1. Unzip samples/mega-avr/teensy_2.0_++/srtos-atmel.zip. 

2. Compile the project  using atmel studio 7. 

3. flash the code using teensy flasher application  

4. Connect FTDI cable to uart1 

## License
Apache 2.0

