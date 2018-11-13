all: multiThreadSorter

multiThreadSorter:
	gcc -g -lpthread -Wall -Werror -fsanitize=address -o multiThreadSorter multiThreadSorter_thread.c mergesort.c

clean: 
	rm -rf multiThreadSorter
