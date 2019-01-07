#Template project for milandr-K1986BE92QI (arm cortex m3) in VS CODE.

##What should be installed
- [Openocd](http://openocd.org/2017/01/openocd-0-10-0-release-is-out/)
- [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm)
- [libusb](https://libusb.info/)
- make

##How build
Run make relatively on makefile 
make COMPILER_DIRS={path to arm-none-eabi-gcc directory}

## How to program
Run relatively on makefile 

openocd -f interface/stlink-v2.cfg -c "transport select hla_swd" -f target/mdr32f9q2i.cfg -c "program ./build/app.elf verify reset exit"

##How to debug in vs code
1. install [Native Debug](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) plugin.
2. In .vscode folder fix path to arm-none-eabi-gdb
3. run [Openocd](http://openocd.org/2017/01/openocd-0-10-0-release-is-out/):
```
openocd -f interface/stlink-v2.cfg -c "transport select hla_swd" -f target/mdr32f9q2i.cfg
```
4. In vs code click to start debugging.

##What have this template
- [RTOS](https://www.freertos.org/)
- [std periph](https://github.com/eldarkg/emdr1986x-std-per-lib)
