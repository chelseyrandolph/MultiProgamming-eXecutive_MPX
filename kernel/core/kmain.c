/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>
#include <string.h>
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/R1/polling.h"
#include "modules/R1/comhand.h"
#include "modules/R3_R4/Dispatcher.h"

void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

  
   // 0) Initialize Serial I/O and call mpx_init


 
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Initialize the support software by identifying the current
   //     MPX Module.  This will change with each module.
 	
   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }
  
   init_serial(COM1);
   set_serial_out(COM1);
   set_serial_in(COM1);
   mpx_init(MODULE_R4);
   
   // 3) Descriptor Tables
   klogv("Initializing descriptor tables...");

   init_idt();
   init_gdt();

   // 4) Virtual Memory
   klogv("Initializing virtual memory...");
   init_pic();
   init_irq();
   sti();
   init_paging();



   // THIS IS JUST OUR MEMU, NOT** COMHAND
   displayMenu();

    // Polling
   sys_set_read(init_polling);




//Loading initial processes
   // 5.-) IDLE process
   klogv("Initilizing idle process...");
   loadProcess("idle", 0, 9, &idle);

   // 5) Call YOUR command handler -  interface method
   klogv("Transferring control to commhand...");
   loadProcess("comhand", 0, 0, &comhand);

   // 5.-) infinite process
   klogv("Initilizing infinite process...");
   loadProcess("infinite", 1, 1, &infinite);





   // 6) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");

   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
