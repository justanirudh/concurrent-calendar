#include <stdio.h>
#include <stdlib.h>
#include "declarations.h"
#include <pthread.h>

int main(int argc, char* argv[]) 
{
	if(argc != 2) {
		printf("format: ./executable buffer_size\n");
		return 0;
	}
	else{
		int buffer_size = atoi(argv[1]);

		BufferManager bufman = initializeBuffer(buffer_size);
		
	  pthread_t email_filter_t, cal_filter_t;
		pthread_attr_t attr;
		int ret;
		EventInfo status = NULL;
		
		//create and initialize attribute
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE);

		//create email thread
		ret = pthread_create(&email_filter_t, &attr, runEmailFilter, bufman);
		if (ret) {
			printf("ERROR %d", ret); 
			exit(1);
		}

		//create calendar thread
		ret = pthread_create(&cal_filter_t, &attr, runCalenderFilter, bufman);
		if (ret) {
			printf("ERROR %d", ret); 
			exit(1);
		}
	
		//join email thread
		ret = pthread_join(email_filter_t, (void *)&status);
		if (ret) {
			printf("ERROR %d", ret);
			exit(1);
		}

		//join calendar thread
		ret = pthread_join(cal_filter_t, (void *)&status);
    if (ret) {
			printf("ERROR %d", ret);
			exit(1);
		}

		//deallocate attribute
		pthread_attr_destroy(&attr);
		pthread_exit(NULL);

    return 0;

	}
	
}
