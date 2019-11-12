//
//  main.cpp
//  Project2_MayraSanchez
//
//  Created by Mayra Sanchez on 9/26/19.
//  Copyright © 2019 Mayra Sanchez. All rights reserved.
//

// use_msgQ.cpp
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;
// declare my global message buffer


struct buf {
    long mtype; // required type of number being made
    char greeting[50]; // message content letters/numbers
};

void child_proc_one(int qid) {
    buf msg;
    int size = sizeof(msg)-sizeof(long);
    msgrcv(qid, (struct msgbuf *)&msg, size, 113, 0);
    cout << getpid() << ": gets message" << endl; //pid manages the process (the child process is identified with this function)
    cout << "message: " << msg.greeting << endl;
    strcat(msg.greeting, " and Adios.");
    cout << getpid() << ": sends reply" << endl;
    msg.mtype = 114; // prepare message with type mtype = 114
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    cout << getpid() << ": now exits" << endl;
}

void child_proc_two(int qid) {
    buf msg;
    int size = sizeof(msg)-sizeof(long);
    // sending a message that will never be read
    msg.mtype = 12;
    strcpy(msg.greeting, "Fake message");
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);//message is being sent to process Queue
    // prepare my message to send
    strcpy(msg.greeting, "Hello there");
    cout << getpid() << ": sends greeting" << endl;
    msg.mtype = 113; // set message type mtype = 113
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);//sending another message to process queue
    msgrcv(qid, (struct msgbuf *)&msg, size, 114, 0); //message is recieved and interpreted by the process P
    //links for sending and recieving are constructed automatically
    //each link is associated with exactly one pair of communicating processes
    //the link here is bidirectional
    cout << getpid() << ": gets reply" << endl; //parent process relates to the getpid()
    cout << "reply: " << msg.greeting << endl;
    cout << getpid() << ": now exits" << endl;
}
/*
 Both classes are sharing resources since they are child processes.
 Child has program loaded into it (ie the creation of function 'child_proc_one')
 */

int main() {//both processes run simultaneously. One system call runs and then the second one follows after waiting to receive the message
    int qid = msgget(IPC_PRIVATE, IPC_EXCL|IPC_CREAT|0600);//message queue pid is set to default (empty)
    //spawning two child processes
    //fork a child process
    pid_t cpid = fork(); //fork calls a new process
    if (cpid == 0) {//if there is no pid existing, then a new message queue will be created and identified by the PID
        child_proc_one(qid); //these processes are terminated once the parent class is terminated
        exit(0); //process resources are deallocated by the OS. Parent gets the status of the child process
    }
    cpid = fork(); //new process is being created
    if (cpid == 0) {//checks to see if there is already a message queue existing, if there is not then it creates a new one
        child_proc_two(qid);
        exit(0);
    }
    while(wait(NULL) != -1); // waiting for both children to terminate
    msgctl(qid, IPC_RMID, NULL); //removing the info the message queue holds
    cout << "parent proc: " << getpid()
    << " now exits" << endl;
    exit(0);//both processes are now terminated and the program ends
}
