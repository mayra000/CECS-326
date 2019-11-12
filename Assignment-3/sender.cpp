//
//  sender.cpp
//  Assignment_3
//
//  Created by Mayra Sanchez on 10/8/19.
//  Copyright © 2019 Mayra Sanchez. All rights reserved.
//

/*
 Sender (or Receiver), PID xxxxx, begins execution
 The sender process should prompt user for a line of input, then send the input line to receiver via the
 message queue created by master. Make sure that the prompt identifies the process making the prompt is
 sender along with its PID.
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
#include <stdio.h>

using namespace std;

//declares global message buffer
struct msg_buffer {/* structure for the message queue */
    long msg_type; //required to know the type
    char msg_text[50]; //message content
};

void sendmsg(int qid) {
    msg_buffer msg;     //message buffer
    int size = sizeof(msg) - sizeof(long); //size of message buffer - size of long variable
    msg.msg_type = 114;
    string message;
    
    sleep(2); //pauses sender process for 2 seconds in order to make sure that the receiver process is ready before continuing
    cout << "Please enter your message: " << endl;
    getline(cin,message); //stores user input into the message buffer chracter array greeting
    strcpy(msg.msg_text,message.c_str()); //outputs the message along with the sender PID
    
    cout << "Sender, " << "PID: " << getpid() << " sends message to Message Queue, " << qid << endl;
    cout << "Message Sent: " << msg.msg_text << endl;
    //sends message to the message queue associated with a queue (qid)
    msgsnd(qid,(struct messageBuf *)&msg, size, 0);
}

int main(int argc, const char* argv[]) {
    cout << "Sender, " << "PID: " << getpid() << ", begins execution" << endl;
    int msg_qid = atoi(argv[0]); //strings are converted to int
    return 0;
    //exit(0);
 }


