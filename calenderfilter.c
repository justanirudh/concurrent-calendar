#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<regex.h>
#include"declarations.h"
#include <stdbool.h>
#include<pthread.h>

void* runCalenderFilter(void* arg){
		
	BufferManager bufman = (BufferManager) arg;
	Calendar graph = initializeCalendar();	
	
	for(;;){ 
		EventInfo ei = removeFromBuffer(bufman);
		if(strcmp(ei->action, "eof") == 0 && 
				strcmp(ei->name, "eof") == 0 && 
				strcmp(ei->date, "eof") == 0 && 
				strcmp(ei->time, "eof") == 0 && 
				strcmp(ei->location, "eof") == 0){
			pthread_exit(NULL);
		}
	
		char* action = ei->action;
		char* name = ei->name;
		char* date = ei->date;
		char* time = ei->time;
		char* location = ei->location;
		
		if(graph->size == 0){ //graph is empty, first entry
			if(strcmp(action, "D") == 0 || strcmp(action, "X") == 0 ){
				printf("Cannot delete or change an event in an empty calendar\n");
				continue;
			}
			DayNode dn = initializeDayNode(date);
			EventNode en = initializeEventNode(name, time, location);
			graph->head = dn;
			(graph->size)++;
			dn->head = en;
			printf("%s:%s\n", date, location);
		}
		else{ //graph is non-empty
			//check if the date exists or not
			DayNode pred;
			DayNode curr = graph->head;
			while(curr != NULL && strcmp(curr->date, date) != 0){
				pred = curr;
				curr = curr->next;
			}
			if(curr == NULL ) //date doesnt exist in graph
				addDayNodeAndFirstEventNode(graph, pred, date,action, name, time, location); //also prints 
			else{ //date exists in graph
				if(curr->head == NULL){ //head of the date node points to null
					if(strcmp(action, "D") == 0 || strcmp(action, "X") == 0 ){
						printf("Cannot delete or change an event in an empty calendar for the date %s\n", date);
						continue;
					}
					EventNode en = initializeEventNode(name, time, location);
					curr->head = en;
					printf("%s:%s\n", date, location);
				}
				else{ //head of date node does not point to null. Then iterate over eventNodes
					//location of old first event node
					EventNode en = curr->head; //curr is current DayNode and is not null
					char* oldLocation = en->location;
					//now do any one of the stated actions
					if( strcmp(action, "C") == 0 ){
						createEvent(curr, name, time, location );
					}
					else if (strcmp(action, "D") == 0 ){
						deleteEvent(curr, name, time, true);
					}
					else if (strcmp(action, "X") == 0 ){
						modifyEvent(curr, name, time, location);
					}
					else {
						printf("Unknown event type: %s\n", action);
						continue;
					}
					//if there is any change to the location of the earliest meeting on a date, 
					//then it will output the date along with the new location of the earliest meeting on that date.
					en = curr->head; //potentially new head
					if(en == NULL) //was not null before
						printf("%s:None\n", date);
					else{
						char* newLocation = en->location;
						if(strcmp(oldLocation, newLocation) != 0) //if location changed, print
							printf("%s:%s\n", date, newLocation);
					}
				}
			}
		}		
	}
}

