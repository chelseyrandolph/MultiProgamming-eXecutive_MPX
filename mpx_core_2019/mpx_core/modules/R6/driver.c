#include "driver.h"




// The file to house your r6 code.
// You need the have functions similar to the following
dcb* DCB; // the device representing the terminal.
void disable_interrupts() {
	outb(COM1 + 1, 0x00); //disable interrupts
}
void enable_interrupts() {
	outb(COM1 + 4, 0x0B); //enable interrupts, rts/dsr set
}
void pic_mask(char enable) {
// If enable, do a logical NOT on the IRQ for COM1.
// Obtain the mask by inb(the PIC_MASK register).
// outb (PIC MASK register, (logical AND the mask with the irq from step 1))
}
int com_open(int* e_flag, int baud_rate) {
// Check the event flag is not null, the baud rate valid,and port is open.
// Set the status of the device to open and idle.
// Set the event flag of the device to the one passed in
// Disable your interrupts.
// Set registers. Take a look at init_serial() in serial.c
// PIC mask enable
// Enable your interrupts.
// Read a single byte to reset the port.
}
int com_close(void) {
// Set the status of the device to closed
// Disable pic mask
// Disable interrupts
}
int com_read(char* buf_ptr, int* count_ptr) {
// check port open, check valid pointer, check port is idle, etc.
// set dcb vars
// disable interrupts
// read from ring buffer into the dcb buffer if there is anything
// enable interrupts
// enable input ready interrupts
}
int com_write(char* buf_ptr, int* count_ptr) {
// check port open, check valid pointer, check port is idle, etc.
// set dcb vars
// disable interrupts
// write a single byte to the device.
// enable interrupts
// enable write interrupts interrupts
}
void serial_io() {
// check port open.
// obtain interrupt type. Call appropriate second level handler
// Check if the event has completed. If so call io scheduler.
// outb(PIC register, PIC end of interrupt)
}
void serial_write() {
// Ensure the dcb status is writing
// If there are any more characters left in the buffer, print them
// Othewise we are done printing
// Update the dcb status. Disable output interrupts
}
void serial_read() {
// Ensure the dcb status is reading. If not, push to the ring buffer.
// Read a character from the COM port & add it to the buffer.
// If we reached a new line or the buffer size, we are done reading
// Update the dcb status. Disable intput interrupts
}
