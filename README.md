#srtos
This is a simple real time operating system (RTOS) for small foot print micro-controllers.
**features** includes setting thread priority, synchronisation objects  and power management. 

## Code Example
**x86 simulation**
   app/projects/test/main_app.c

**microcontroler srtos port  examples**

Atmel AT90USB1286  Teensy 2.0 ++
1. Unzip samples/mega-avr/teensy_2.0_++/srtos-atmel.zip. 

2. Compile the project  using atmel studio 7. 

3. flash the code using teensy flasher application  

4. Connect FTDI cable to uart1 
TODO   Detailed instructions with link to wiki/youtube etc.

## Motivation

Often thought that it would be nice to have scheduler with minimal over head to setting up when doing bare metal programming on small 8bit microcontrollers.
The idea is to include 2-3 files to your makefile and it should be ready to go.  

## HOWTO 

**x86 simulation**
Instructions (Tested in Ubuntu and Mac )
1. git clone https://github.com/sajithkamal/s-rtos.git
2. make
3 ./a.out

## License
Apache 2.0

