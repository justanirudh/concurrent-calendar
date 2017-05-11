#include<stdio.h>
#include<stdlib.h>
#include"declarations.h"
#include <string.h>
#include <assert.h>

Calendar initializeCalendar() {
  Calendar graph = (Calendar) malloc(sizeof(struct calendar));
	graph->size = 0;
	graph->head = NULL;
  return graph;
}
DayNode initializeDayNode(char* date){
	DayNode dn = (DayNode)malloc(sizeof(struct dayNode));
	dn->date = date;
	dn->head = NULL;
	dn->next = NULL;
	return dn;
}

EventNode initializeEventNode(char* name, char* time, char* location){
	EventNode en = (EventNode)malloc(sizeof(struct eventNode));
	en->time = time;
	en->name = name;
	en->location = location;
	en->next = NULL;
	return en;
}

void addDayNodeAndFirstEventNode(Calendar graph, DayNode pred, char* date, char* action, char* name, char* time, char* location){
	if(strcmp(action, "D") == 0 || strcmp(action, "X") == 0 ){
		printf("Cannot delete or change an event in an empty calendar for date %s\n", date);
		return;
	}
	DayNode dn = initializeDayNode(date);
	EventNode en = initializeEventNode(name, time, location);
	pred->next = dn;
	(graph->size)++;
	dn->head = en;
	printf("%s:%s\n", date, location);
}

void createEvent(DayNode dn, char* name, char*  time, char*  location ){
	EventNode	pred = NULL;
	EventNode curr = dn->head;
	while(curr != NULL && strcmp(time, curr->time) > 0){ //time already lexicographically sorted
		pred = curr;
		curr = curr->next;
	}
	EventNode en = initializeEventNode(name, time, location);
	en->next = curr;
	if(pred != NULL) //pred is a non-first event node
		pred->next = en;
	else //the first node itself was > input time
		dn->head = en;
}

void deleteEvent(DayNode dn, char* name, char* time, bool consider_time){ //consider_time is true for deletion and false for updation
	EventNode pred = NULL;
	EventNode curr = dn->head;
	if(consider_time){
		while(curr != NULL && !(strcmp(curr->name, name) == 0 && strcmp(curr->time, time) == 0)){
			pred = curr;
			curr = curr->next;
		}
		if(curr == NULL){
			printf("Cannot find any matching events for deletion for the date %s\n", dn->date);
			return;
		}
	}
	else{
		while(curr != NULL && strcmp(curr->name, name) != 0){
			pred = curr;
			curr = curr->next;
		}
		if(curr == NULL){
			printf("Cannot find any matching events for updation for the date %s\n", dn->date );
			return;
		}
	}	
	if(pred != NULL) //curr found later in the list
		pred->next = curr->next;
	else //deleting the first element of the list
		dn->head = curr->next;
}

void modifyEvent(DayNode dn, char* name, char* time, char* location){
	//delete the existing event having the same name from THIS date;
	deleteEvent(dn, name, time, false);	
	//create a new event
	createEvent(dn, name, time, location);	
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
