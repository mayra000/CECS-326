//
//  master.cpp
//  Assignment_3
//
//  Created by Mayra Sanchez on 10/8/19.
//  Copyright © 2019 Mayra Sanchez. All rights reserved.
//

/*
 The master process should first acquire a message queue from the operating system, followed by creating
 two child processes, with one to execute sender and the other to execute receiver, using fork and exec
 system calls. The master process should output the message queue ID, the process IDs of the sender and
 receiver processes it has created, then waits for both child processes to return. Then master will remove
 the message queue before exit.
 */

/*
 msgget(): either returns the message queue identifier for a newly created message
 queue or returns the identifiers for a queue which exists with the same key value.
 */



#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;

int main() {
    //  Creates a message queue and assigns the queue ID to qid
    int qid = msgget(IPC_PRIVATE, IPC_EXCL|IPC_CREAT|0600);
    
    //  Converting int into a string, then into a character constant to allow it to be passed as an argument
    auto qidstr = to_string(qid);
    char const *qidchar = qidstr.c_str();
    
    cout << "Message Queue " << qidchar << " created" << endl;
    /* Creating the child processes (sender and receiver) */
    //  Creates the Receiver process (for receiving messages to the message queue)
    pid_t cpid = fork();
    if(cpid < 0) {
        cout << "Fork failed." << endl;
        return 1;
      }
    else if(cpid == 0) { //receiver in execution. Queue ID is taken as an argument
        execlp("./receiver", qidchar, NULL);
        exit(0);
    }
    
    //creating the sender process
    cpid= fork();
    if(cpid < 0) {
        cout << "Fork failed." << endl;
        return 1;
      }
    else if(cpid == 0) { //  Sending message program execution, passing the message queue ID as an argument
        execlp("./sender", qidchar, NULL);
        exit(0);
    }
    
    //  Waits for the child processes to finish executing
    while(wait(NULL) != -1);

    //  Terminates the message queue along with the data structure associated with it
    msgctl(qid, IPC_RMID, NULL);

    cout << "Message Queue " << qid << " terminated" << endl;
    //exit(0);    //  Terminates the parent process
    
    return 0;
    
}

//struct msg_buffer {/* structure for the message queue */
//    long msg_type;
//    char msg_text[50];
//} message;
//
////executes sender
//void child_1(int qid) {
//    msg_buffer msg;
//    int msgid;
//
//    key_t key;
//    key = ftok("progfile", 65); /* ftok used to create a unique key */
//
//    /*
//     msgget creates a message queue
//     returns identifier
//     */
//    msgid = msgget(key, 0666 | IPC_CREAT);
//    message.msg_type = 1;
//
//    cout << "Write Data: " << message.msg_text << endl;
//
//    //mgsnd to send message
//    msgsnd(msgid, &message, sizeof(message), 0);
//
//    //display the message
//    cout << "The data sent is: " << message.msg_text << endl;
//
//}
//
////executes receiver
//void child_2(int qid) {
//
//}
//
//int main() {
//    pid_t pid = fork();
//    int qid = msgget(IPC_PRIVATE, IPC_EXCL|IPC_CREAT|0600);//message queue pid is set to default (empty)
//
//
//
//    if (pid < 0) { //error occured
//        fprintf(stderr, "Fork failed.");
//        return 1;
//    }
//    else if(pid == 0) { //child process
//        child_1(qid);
//        exit(0);
//        //execlp("/bin/ls", "ls", NULL);
//    }
//
//    else if (pid > 0) { //parent process
//        /* parent will wait for the child to complete*/
//        wait(NULL);
//        cout << "Child Complete" <<endl;
//    }
//
//    /* fork a child process*/
//    pid = fork();
//    if (pid == 0) {
//        child_2(qid);
//    }
//    return 0;
//
//    //exit(0)
//}
