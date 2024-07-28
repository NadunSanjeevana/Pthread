
### Makefile


CC = gcc
CFLAGS = -pthread -Wall -O2
OBJ = linked_list.o serial.o mutex.o rw_lock.o main.o
EXEC = linked_list_test

.PHONY: all clean

all: output_dir $(EXEC)

output_dir:
	mkdir output

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CFLAGS) -c $<

serial.o: serial.c serial.h linked_list.h
	$(CC) $(CFLAGS) -c $<

mutex.o: mutex.c mutex.h linked_list.h
	$(CC) $(CFLAGS) -c $<

rw_lock.o: rw_lock.c rw_lock.h linked_list.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c linked_list.h serial.h mutex.h rw_lock.h
	$(CC) $(CFLAGS) -c $<

clean:
	del /Q $(OBJ) $(EXEC)
	if exist output rd /S /Q output
