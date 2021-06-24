/* Author : Gert van Biljon 
 *
 * EMail : gertvb@gmail.com
 *
 * Date started : 2019/01/13
 *
 * What am I : I am an Atmel Studio 7 project for the ADAFruit Feather M4 Express (ATSAMD51J19) with Start.Atmel (ASF4).
 *
 * Licence and copyright : NONE, posted in the public domain, so as to be a help to anyone out there also trying to get their Feather M4 Express working with Atmel Studio.
 *
 * Disclaimer : Use this project entirely at your own risk, I take no responsibility for any damages incurred or suffered for when you use this project or part of it
 *
 *
 * Program start address increase, to cater for the pre-installed Bossac compatible UF2 bootloader on the ADAFruit Feather M4 Express that occupies the first 16kB/0x4000kB at the start of memory.
 *             Also see : https://reprapdad.wordpress.com/2016/08/19/adafruit-feather-m0-with-atmel-studio/
 *             Solution Explorer -> Right click on your project -> Properties (right at the bottom) -> Toolchain -> Arm/Gnu linker -> Miscellaneous -> Linker Flags
 *
 *             Modify :  -Tsamd51j19a_flash.ld
 *                 to :  -Wl,--section-start=.text=0x4000 -Tsamd51j19a_flash.ld 
 *
 *
 * Bossac programmer used on the PC to transfer your compiled program to the Feather M0 : 
 *             Also see : http://www.elecrom.com/program-arduino-due-atmel-studio/
 *
 *             Install Bossac, at least ver 1.9
 *
 *             Add Bossac as a menu item under tools in Atmel Studio
 *                 1.  Tools -> External Tools -> Add
 *
 *                 2.1 Title : Whatever you want to call it, I've called mine "Bossac M4 on COM12"
 *                 2.2 Command : C:\Program Files (x86)\BOSSA\bossac.exe
 *                 2.3 Arguments : --erase --write --verify --reset --info --debug --offset=0x4000 --port=COM12 $(TargetDir)$(TargetName).bin
 *                 2.4 Initial Directory : $(TargetDir)
 *                 2.5 Check : Use output window
 *                 2.6 Apply & OK   
 *                 3. FYI : This basically causes the following command to be executed : "C:\Program Files (x86)\BOSSA\bossac.exe" --erase --write --verify --reset --info --debug --offset=0x4000 --port=COM12 "C:\projects\Feather_M4\Feather_M4\Debug\Feather_M4.bin"
 *
 *             Get your Feather M4 Express into Bossac programming mode by double-clicking the reset button, then wait for the following:
 *                 1. The NeoPixel Led must turn green after going red for a second
 *                 2. The red LED next to the USB connector to fade in and out
 *                 3. Windows Explorer to show the Feather M4 Express as : FEATHERBOOT (D:)
 *                 4. Windows Device Manager to show the Feather M4 Express as : Adafruit Feather M4 Express UF2 Bootloader (0022:00)(COM12)
 *               
 *             Finally download your program to the Feather M4 : Atmel Studio -> Tools -> Bossac M4 on COM12
 *
 * Windows 7 USB PC Driver for Feather M0 when the bootloader program is running on the Feather, and waiting for Bossac on the PC to send the new program :
 *             https://github.com/adafruit/Adafruit_Windows_Drivers/releases/latest
 *
 * Printing floats in snprintf, normally disabled to save memory space
 *             Solution Explorer -> Right Click on your project (Feather_M4 in BOLD) -> Properties (right at the bottom) -> Toolchain -> Arm/Gnu linker -> General
 *             Uncheck : "Use size optimized library (--specs=nano.specs)"
 *
 * USB output under linux :
 *             Find which teletype the Feather was assigned to : dmesg | grep tty
 *             look for cdc_acm, will be something like ttyACM0
 *             cat /dev/ttyACM0 
 *
 * Diff tool for comparing files :
 *             Install WinMerge : http://winmerge.org/
 *             Atmel Studio -> Tools -> Options -> Atmel Software framework -> File compare -> Path of application used for comparing files
 *
 * Regards from the Southern side of Africa
 *
 * Gert van Biljon
 */

#include <atmel_start.h>
#include <stdio.h>

// VT200 escape codes to print colour in TerraTerm/xterm : http://www.johnrepici.com/serendipity/archives/192-The-xterm-256-color-table-set-colors-in-xterm-or-PuTTY.html
// print_ORANGE "\033[38;5;208m"
//
// \033[ : escape code
// 38    : foreground, use 48 for background
// 5     : escape mode
// 208   : colour to use : Orange, calculated as follows : 16 + (36 X Red = 5) + (6 X Green = 2) + (Blue = 0) : where Red Green and Blue ranges from 0 to 5
// m     : end of escape code
//
// Can also specify 0-255 Red Green Blue values as follows ( escape mode 2 ) : \033[38;2;<r>;<g>;<b>m
// but this doesn't work with Putty terminal
#define print_WHITE  "\033[38;5;231m"
#define print_RED    "\033[38;5;196m"
#define print_ORANGE "\033[38;5;208m"
#define print_YELLOW "\033[38;5;226m"
#define print_GREEN  "\033[38;5;46m"
#define print_BLUE   "\033[38;5;14m"
#define print_VIOLET "\033[38;5;213m"
#define print_LILAC  "\033[38;5;141m"
#define print_PURPLE "\033[38;5;93m"

#define HAPPY 1

int main(void)
{
    char usb_serial_port_write_buffer[1000];

    uint16_t ADC_val_A0_AIN0 = 0;     //0x00 from /CONFIG/hpl_adc_config.h
    uint16_t ADC_val_A1_AIN5 = 0;     //0x05 from /CONFIG/hpl_adc_config.h
    uint16_t ADC_val_A2_AIN2 = 0;     //0x02 from /CONFIG/hpl_adc_config.h
    uint16_t ADC_val_A3_AIN3 = 0;     //0x03 from /CONFIG/hpl_adc_config.h
    uint16_t ADC_val_A4_AIN4 = 0;     //0x04 from /CONFIG/hpl_adc_config.h
    uint16_t ADC_val_A5_AIN6 = 0;     //0x06 from /CONFIG/hpl_adc_config.h
    uint16_t ADC_val_BATT_AIN13 = 0;  //0x0D from /CONFIG/hpl_adc_config.h
    //Negative Mux Input Selection <0x18=>Internal ground
    
	system_init();

	usb_init();
    //while (!cdcdf_acm_is_enabled())
    //{
    //};
    //usbdc_deinit();

	adc_sync_enable_channel(&ADC_0, 0);

	while (HAPPY) 
    {
   		adc_sync_set_inputs(&ADC_0,	0x00, 0x18, 0);//from config/hpl_adc_config, read AIN0 (0x00) referenced to Internal Ground (0x18)
       	adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&ADC_val_A0_AIN0, 2);

   		adc_sync_set_inputs(&ADC_0,	0x05, 0x18, 0);
   		adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&ADC_val_A1_AIN5, 2);

   		adc_sync_set_inputs(&ADC_0,	0x02, 0x18, 0);
   		adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&ADC_val_A2_AIN2, 2);

   		adc_sync_set_inputs(&ADC_0,	0x03, 0x18, 0);
   		adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&ADC_val_A3_AIN3, 2);

   		adc_sync_set_inputs(&ADC_0,	0x04, 0x18, 0);
   		adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&ADC_val_A4_AIN4, 2);

   		adc_sync_set_inputs(&ADC_0,	0x06, 0x18, 0);
   		adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&ADC_val_A5_AIN6, 2);

   		adc_sync_set_inputs(&ADC_0,	0x0D, 0x18, 0);
   		adc_sync_read_channel(&ADC_0, 0, (uint8_t*)&ADC_val_BATT_AIN13, 2);

	    snprintf(usb_serial_port_write_buffer,
   		         sizeof(usb_serial_port_write_buffer),
	             "\n\r" //put it first, as when the string to be printed is too long, the /n/r sometimes does not get added
                 print_WHITE
                 "  *  "
                 print_RED
                 "%4d"
                 print_WHITE
                 "  *  "
                 print_ORANGE
                 "%4d"
                 print_WHITE
                 "  *  "
                 print_YELLOW
                 "%4d"
                 print_WHITE
                 "  *  "
                 print_GREEN
                 "%4d"
                 print_WHITE
                 "  *  "
                 print_BLUE
                 "%4d"
                 print_WHITE
                 "  *  "
                 print_VIOLET
                 "%4d"
                 print_WHITE
                 "  *  "
                 print_LILAC
                 "%4d"
                 print_WHITE
                 "  *  ",
                 ADC_val_A0_AIN0,
                 ADC_val_A1_AIN5,
                 ADC_val_A2_AIN2,
                 ADC_val_A3_AIN3,
                 ADC_val_A4_AIN4,
                 ADC_val_A5_AIN6,
                 ADC_val_BATT_AIN13
	            );
	    cdcdf_acm_write((uint8_t *)usb_serial_port_write_buffer, strlen(usb_serial_port_write_buffer));
        delay_ms(20);
        gpio_toggle_pin_level(Led);
    }        
}
