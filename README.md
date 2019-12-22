This MPX project used Linux VM, C programming, and GDB debugging tool.

R1: Basic Interface Development
  - Polling
  - Command Handler
  - User Interface
  - Menu Display

R2: PCB (Process Control Block) Operations
  - Process management
  - Creating/deleting, keeping track of processes, and moving processes between multiple queues representing process states

R3/ R4: Dispatching **Mostly written in C, but some Assembly is required
  R3:
    - Track particular processes with PCBs 
    - Support system idling and exiting
    - Switch out of the running process using a context switch
    - Be able to add new processes, execute the processes until all are complete, then return to command handler
	R4:
    - Treat your command handler as a process with its own priority and state
    - Prevent polling from being blocked
    - Add an alarm command
    - Use an infinitely running process that has to be user suspended to be killed

R5: Memory Management
  - Implement a heap manager to dynamically allocate and deallocate memory 
  - Blocks of memory (MCB) will be kept track of through the use of complete memory control blocks (CMCB)
  - Blocks of memory will be stored in one of two lists, which must be implemented (free block list & allocated block list)

R6: Interrupt Driven I/O ---- WAS NOT COMPLETED IN THE GIVEN SOURCE CODE
  - Build a complete MultiProgramming eXecutive by integrating all previous modules
  - Implement a simple interrupt-driven serial port driver
    - Enable communication between your computer and: 
      - Another attached computer
      - Video terminal
    - Handle external interrupt events
  - Integrate the driver into the MPX via I/O queues and sys_call
