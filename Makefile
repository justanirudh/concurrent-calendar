all:	prog1

prog1: emailfilter.c calenderfilter.c cal_functions.c locationupdater.c buffer_ops.c
	gcc -pthread emailfilter.c calenderfilter.c cal_functions.c locationupdater.c buffer_ops.c -o locationupdater
clean:
	rm  locationupdater
