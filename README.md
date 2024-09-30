# Linked List Operations with Serial and Parallel Implementations

## Description

This project implements a linked list with `Member`, `Insert`, and `Delete` functions in three different ways:

1. Serial implementation.
2. Parallel implementation using Pthreads with a single mutex for the entire linked list.
3. Parallel implementation using Pthreads with read-write locks for the entire linked list.

## Files

- `linked_list.h` and `linked_list.c`: Implementation of the common linked list operations.
- `serial.h` and `serial.c`: Serial implementation of the linked list operations.
- `mutex.h` and `mutex.c`: Parallel implementation with a single mutex for the entire linked list.
- `rw_lock.h` and `rw_lock.c`: Parallel implementation with read-write locks for the entire linked list.
- `utils.h` and `utils.c` : Implementation of Durstenfeld shuffle algorithm
- `main.c`: Main program to run the tests and generate reports.

## Requirements

- GCC compiler
- Make
- A machine with at least four physical CPU cores

## Build and Run Instructions

### Building the Project

To compile the project, use the provided `Makefile`. Run the following command in the terminal:

```sh
make
```

### Run the Project

To run the project, use the newly created `run.exe`. Run the following command in the terminal:

```sh
./run
```

### Plot the Graphs

To plot the graphs, use the newly created `output csv` files. Run the following command in the terminal:

```sh
python -u graphs.py
```
