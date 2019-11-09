/*
Version prints out the current version of the MPX.
Return Type: Void
Paramters: None
*/
void version();


/*
displayAllCommands prints out all the commands we currently have available to the user. 
Return Type: void
Paramters: none
*/
void displayAllCommands();


/*
clear will clear the users terminal from all previous command outputs.
Return Type: void
Paramters: none
*/
void clear();


/*
inputHelp prints out the information about the user specified command.
Return Type: int
Paramters: char[]: a buffer of the help commands
*/
int inputHelp(char helpBuffer[]);


/*
shutdown walks the user through the shutdown process and allows commhand to end.
Return Type: int
Paramters: none
*/
int shutdown();


/*
getHistorySize returns the size of the command in history.
Return Type: int
Paramters: none
*/
int getHistorySize();


/*
getCommand returns a command from our command history. 
Return Type: char*
Paramters: int: index of the command
*/
char* getCommand(int index);


/*
addToCmdHistory add the most recent command entered by the user into our command history list.
Return Type: void
Paramters: char[]: the command we are adding to our command history list
*/
void addToCmdHistory(char string[100]);

/*
comhand takes the user's input and calls the corresponding command, or prints an error.
Return Type: int
Paramters: none
*/
int comhand();


/*
displayMenu displays our menu at the start of our OS and then never displays it again.
Return Type: void
Paramters: none
*/
void displayMenu();


/*
substring creates a substring of a given string based of the start and end index of the string.
Return Type: char*
Paramters: char*: the source string
		   int: the start index of the source string
		   int: the end index of the source string
*/
char* substring(char *source, int start, int end);


/*
autocomplete will complete the command a user is typing when tab is hit, or prints out the available commands that are equivalent to what the user has typed. 
Return Type: char*
Paramters: char[]: the buffer when the user hits tab
*/
char* auto_complete(char partial_str[]);


/*
showHistory will display all the commands currently in history
Return Type: void
Paramters: none
*/
void showHistory();



