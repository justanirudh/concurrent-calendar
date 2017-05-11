#include <stdbool.h>
#include<pthread.h>
struct eventNode
{
  char* time;
  char* name;
	char* location;
  struct eventNode  *next;
};

typedef struct eventNode* EventNode;

struct dayNode
{
  char* date;
  EventNode head;
  struct dayNode  *next;
};

typedef struct dayNode* DayNode;

struct calendar{
  int size;
  DayNode head;
};

typedef struct calendar* Calendar;

struct eventInfo{
	char* action;
	char* name;
	char* date;
	char* time;
	char* location;
};

typedef struct eventInfo* EventInfo;

struct bufferManager {
	int in;
	int out;
	int numElems;
	int size;
	pthread_mutex_t lock;
  pthread_cond_t notFull; //waiting for it to be not full
	pthread_cond_t notEmpty;//waiting for it to be not empty
	EventInfo buffer;
};

typedef struct bufferManager* BufferManager;

Calendar initializeCalendar();
char** str_split(char*, const char);
DayNode initializeDayNode(char*);
EventNode initializeEventNode(char*, char*, char*);
void addDayNodeAndFirstEventNode(Calendar, DayNode, char*, char*, char*, char*, char*); 
void createEvent(DayNode, char*, char*, char*);
void deleteEvent(DayNode, char*, char*, bool);
void modifyEvent(DayNode, char*, char*, char*);

BufferManager initializeBuffer(int);
void* runEmailFilter(void*);
void* runCalenderFilter(void*);
void addToBuffer(BufferManager, EventInfo);
EventInfo removeFromBuffer(BufferManager bm);
void printBuffer(BufferManager); 
