##srtos
This is a minimal rtos (srtos)  implementation for embedded system.
it deosn't depends on assembly language and not based on a tick interrupt for context siwtch and scheduling.
**rtos** is designed primarly for   micro controller application programmers who need liitle help from a scheduler.
**features** includes setting thread priority, synchronisation objects  and power management. 

## Code Example
**x86 simulation**
   app/projects/test/main_app.c

**microcontroler srtos port  examples**

Atmel AT90USB1286  Teensy 2.0 ++
	Unzip samples/mega-avr/teensy_2.0_++/srtos-atmel.zip
        Compile the project file using atmel studio 7
        flash the code using teensy flasher application
	Connect FTDI cable to uart1
        TODO
             Detailed instructions with link to wiki/youtube etc.

## Motivation

Often thought that it would be nice to have scheduler with minimal over head in setting up when doing baremetal programming on small 8bit microcontrollers.
The idea is to include 2-3 files to your makefile and it should be ready to go.  

## HOWTO 

**x86 simulation**
Instructions (Tested in Ubuntu and Mac )
1. git clone https://github.com/sajithkamal/s-rtos.git
2. make
3 ./a.out

## License
Apache 2.0

