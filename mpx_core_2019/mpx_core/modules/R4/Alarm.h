
/*
Allows the user to set an alarm.
Return Type: Void
Parameters: None
*/
void setAlarm();


/*
Checks for current alarms.
Return Type: Void
Parameters: None
*/
void checkAlarms();

typedef struct alarm {
	struct alarm* next;
	int Hour, Minute;
	char message[50];

}alarm;

typedef struct alarmQueue {
	alarm *head;
	alarm *tail;
	int count;

}alarmQueue;
