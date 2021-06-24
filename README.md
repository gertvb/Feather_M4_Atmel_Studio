Author : Gert van Biljon 

EMail : gertvb@gmail.com

Date started : 2019/01/13

What am I : I am an Atmel Studio 7 project for the ADAFruit Feather M4 Express (ATSAMD51J19) with Start.Atmel (ASF4).

Licence and copyright : NONE, posted in the public domain, so as to be a help to anyone out there also trying to get their Feather M4 Express working with Atmel Studio.

Disclaimer : Use this project entirely at your own risk, I take no responsibility for any damages incurred or suffered for when you use this project or part of it


Program start address increase, to cater for the pre-installed Bossac compatible UF2 bootloader on the ADAFruit Feather M4 Express that occupies the first 16kB/0x4000kB at the start of memory.
            Also see : https://reprapdad.wordpress.com/2016/08/19/adafruit-feather-m0-with-atmel-studio/
            Solution Explorer -> Right click on your project -> Properties (right at the bottom) -> Toolchain -> Arm/Gnu linker -> Miscellaneous -> Linker Flags

            Modify :  -Tsamd51j19a_flash.ld
                to :  -Wl,--section-start=.text=0x4000 -Tsamd51j19a_flash.ld 


Bossac programmer used on the PC to transfer your compiled program to the Feather M0 : 
            Also see : http://www.elecrom.com/program-arduino-due-atmel-studio/

            Install Bossac, at least ver 1.9

            Add Bossac as a menu item under tools in Atmel Studio
                1.  Tools -> External Tools -> Add

                2.1 Title : Whatever you want to call it, I've called mine "Bossac M4 on COM12"
                2.2 Command : C:\Program Files (x86)\BOSSA\bossac.exe
                2.3 Arguments : --erase --write --verify --reset --info --debug --offset=0x4000 --port=COM12 $(TargetDir)$(TargetName).bin
                2.4 Initial Directory : $(TargetDir)
                2.5 Check : Use output window
                2.6 Apply & OK   
                3. FYI : This basically causes the following command to be executed : "C:\Program Files (x86)\BOSSA\bossac.exe" --erase --write --verify --reset --info --debug --offset=0x4000 --port=COM12 "C:\projects\Feather_M4\Feather_M4\Debug\Feather_M4.bin"

            Get your Feather M4 Express into Bossac programming mode by double-clicking the reset button, then wait for the following:
                1. The NeoPixel Led must turn green after going red for a second
                2. The red LED next to the USB connector to fade in and out
                3. Windows Explorer to show the Feather M4 Express as : FEATHERBOOT (D:)
                4. Windows Device Manager to show the Feather M4 Express as : Adafruit Feather M4 Express UF2 Bootloader (0022:00)(COM12)
              
            Finally download your program to the Feather M4 : Atmel Studio -> Tools -> Bossac M4 on COM12

Windows 7 USB PC Driver for Feather M0 when the bootloader program is running on the Feather, and waiting for Bossac on the PC to send the new program :
            https://github.com/adafruit/Adafruit_Windows_Drivers/releases/latest

Printing floats in snprintf, normally disabled to save memory space
            Solution Explorer -> Right Click on your project (Feather_M4 in BOLD) -> Properties (right at the bottom) -> Toolchain -> Arm/Gnu linker -> General
            Uncheck : "Use size optimized library (--specs=nano.specs)"

USB output under linux :
            Find which teletype the Feather was assigned to : dmesg | grep tty
            look for cdc_acm, will be something like ttyACM0
            cat /dev/ttyACM0 

Diff tool for comparing files :
            Install WinMerge : http://winmerge.org/
            Atmel Studio -> Tools -> Options -> Atmel Software framework -> File compare -> Path of application used for comparing files

Regards from the Southern side of Africa

Gert van Biljon

