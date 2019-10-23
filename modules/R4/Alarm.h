void setAlarm();
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
