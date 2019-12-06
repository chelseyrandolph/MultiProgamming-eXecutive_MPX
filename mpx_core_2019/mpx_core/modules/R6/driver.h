// The file to house your r6 header.
// You need the have functions similar to the following
#define PIC_REG 0x20
#define PIC_EOI 0x20
#define PIC_MASK 0x21
#define IRQ_COM1 0x10
/*!
+* enum for the possible dcb states.
*/
typedef enum status_t {
STATUS_IDLE, /*!< Port is idle */
STATUS_READING, /*!< Port is reading */
STATUS_WRITING /*!< Port is writing */
} status_t;
/*!
+* struct dcb represents a Device Control Block.
+* A dcb should exist for each COM port, but you can just use COM1
+* @param com_port the COM port. (You can omit this and just always use COM1)
+* @param port_open whether the COM is open.
+* @param e_flag whether the operation has completed (0 or 1).
+* @param status the different operations (IDLE, READ, WRITE).
+* @param buffer_ptr the buffer array to read into/write from.
+* @param count_ptr how many characters to read/write.
+* @param buffer_loc the current location we are reading/writing at.
+* @param byte_count the number of bytes that have been read/written so far.
*/
typedef struct dcb {
// params here
} dcb;
/*!
+* struct iod represents an I/O Desriptor.
+* @param pcb_id the process that this iod is representing.
+* @param op_code the operation that the process requested.
+* @param com_port the COM port. (You can omit this and just always use COM1)
+* @param buffer_ptr the buffer pointer to read to/write from.
+* @param count_ptr the amount of characters to be read/written.
+* @param next the next IOD in the IO queue after this one.
*/
typedef struct iod {
// params here
} iod;
/*!
+* pic_mask() masks so only the desired PIC interrupt is enabled or disabled.
+* @param enable 1 to enable or 0 to disable.
*/
void pic_mask(char enable);
/*!
+* disable_interrupts() disables all interrupts to device.
*/
void disable_interrupts();
/*!
+* enable_interrupts() enables interrupts to device.
*/
void enable_interrupts();
/*!
+* com_open() Opens the communication port.
+* @param e_flag event flag will be set to 1 if read/write
+* @param baud_rate the desired baud rate
+* @return Returns three possible error codes, or a 0 for successful
operation.
*/
int com_open(int* e_flag, int baud_rate);
/*!
+* com_close() Closes the communication port.
+* @return error code if port was not open, or a 0 for successful operation
*/
int com_close(void);
/*!
+* com_read() Reads the buffer from the port. Non-blocking.
+* @param buf_ptr buffer in which the read characters will be stored.
+* @param count_ptr the maximum number of bytes to read. After completion,
+* this will contain the number of characters read.
+* @return Returns four possible error codes, or a 0 for successful operation.
*/
int com_read(char* buf_ptr, int* count_ptr);
/*!
+* com_write() Writes the buffer to the port. Non-blocking.
+* @param buf_ptr buffer in which the characters to write are stored.
+* @param count_ptr the number of characters from the buffer to write.
+* @return Returns four possible error codes, or a 0 for successful operation.
*/
int com_write(char* buf_ptr, int* count_ptr);
/*!
+* serial_io() is the interrupt C routine for serial IO.
*/
void serial_io();
/*!
+* serial_write() provides interrupt routine for writing IO.
*/
void serial_write();
/*!
+* serial_read() provides interrupt routine for reading IO.
*/
void serial_read();
