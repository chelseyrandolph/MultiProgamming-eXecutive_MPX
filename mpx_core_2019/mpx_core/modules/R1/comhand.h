void version();

char* getCommand(int index);

void displayAllCommands();

int inputHelp(char helpBuffer[]);

int getHistorySize();

int shutdown();

int comhand();

void displayMenu();

char* auto_complete(char partial_str[]);

void addToCmdHistory(char string[100]);

void showHistory();
