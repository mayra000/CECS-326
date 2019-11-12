//
//  shmc1.cpp
//  Assignment_4
//
//  Created by Mayra Sanchez on 10/29/19.
//  Copyright © 2019 Mayra Sanchez. All rights reserved.
//

/* shmc1.cpp */
#include "registration.h" //including registration file from which the CLASS structure is called from
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>
using namespace std;


CLASS *class_ptr; //referencing header file that has a structure called CLASS
void *memptr; //pointer for shared memory
char *pname; //pointer for process name
int shmid, ret; //shared memory ID and return value that detahces from shared memory segment
void rpterror(char *), srand(), perror(), sleep(); //functions that are used throughput the program
void sell_seats(); //function

int main(int argc, char* argv[]) {
    if (argc < 2) { //verifies that at least 2 arguments are passed
        fprintf (stderr, "Usage:, %s shmid\n", argv[0]); //printing out usage message
        exit(1); //exiting the process
    }
    pname = argv[0]; //initializing process name to the first argument. (first name of the prcess)
    sscanf (argv[1], "%d", &shmid); //second argument has their ID stored as a decimal integer
    /*
     shared memory segment is attatched to the shared memory identifier through the shmid
     second parameter is given a null pointer
     */
    memptr = shmat (shmid, (void *)0, 0); //memptr stores the shared memory pointer's start address as long as the shmat function is completed successfully
    if (memptr == (char *)-1 ) { //if memptr = -1 then, shared memory segment attachment has failed.
        rpterror ("shmat failed"); //error message that prints out
        exit(2); //exits the system
    }
    class_ptr = (struct CLASS *)memptr; //assigns the shared memory segment pointer memptr to the class data structure that was declared in the header file
    sell_seats(); //function called to sell the seats
    ret = shmdt(memptr); //detaches shared memory segment associated w/ memptr and ret is initialized to 0 w/ successful completetion
    exit(0); //successful exiting of the program ( 0 means successful completion)
}

//function that sells all the seats in a CLASS, depends on (seats_left <=0) and (all_out = 1)
void sell_seats()
{
    int all_out = 0;//indicates amount of seats remaining (0 = available, 1 = occupied)
    srand ( (unsigned) getpid() ); //psuedo random number is initialized to get pid
    while ( !all_out) { /* loop to sell all seats */
        if (class_ptr->seats_left > 0) { //makes sure there are seats left before starting to sell
            sleep ( (unsigned)rand()%5 + 1); //sleeps for a random number of seconds (can be b/w 1 and 5 seconds)
            class_ptr->seats_left--; //a seat is removed from the class
            sleep ( (unsigned)rand()%5 + 1); //sleeps for a random number of seconds (can be b/w 1 and 5 seconds)
            cout << pname << " SOLD SEAT -- " //ouputs the amount of seats left
            << class_ptr->seats_left << " left" << endl;
        }
        else {//lets the user know that there are no seats left
            all_out++;//changed to 1 to let them know that the seats are unavailable
            cout << pname << " sees no seats left" << endl;
        }
        sleep ( (unsigned)rand()%10 + 1);//sleeps for a random number of seconds (can be b/w 1 and 10 seconds)
    }
}
/*
 prints out final message
 all passed parameters are printed out at the end
 */
void rpterror(char* string)
{
    char errline[50]; //char array for error message
    sprintf (errline, "%s %s", string, pname); //%s means string.
    perror (errline); //prints out custom error message stored in errline
}
