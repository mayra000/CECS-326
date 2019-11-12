//
//  receiver.cpp
//  Assignment_3
//
//  Created by Mayra Sanchez on 10/8/19.
//  Copyright © 2019 Mayra Sanchez. All rights reserved.
//

/*
 The receiver process retrieves a message from the message queue, and outputs the message on the screen.
 Make sure the output identifies that the line is printed by receiver along with PID.
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

//global message buffer created
struct msg_buffer {/* structure for the message queue */
    long msg_type;
    char msg_text[50]; //message content
};

void rcvmsg(int qid) {
    msg_buffer msg;
    int size = sizeof(msg) - sizeof(long); //size of message buf - size of long variable
    
    msgrcv(qid, (struct msgbuffer *)&msg, size, 114, 0);
    /* Output of the message received */
    cout << "Receiver, " << "PID: " << getpid() << " receives message from Message Queue, " << qid << endl;
    cout << "Message Received: " << msg.msg_text << endl;
}

int main(int argc, const char* argv[]) {
    cout << "Receiver, " << "PID " << getpid() << ", begins execution" << endl;
    int msg_qid = atoi(argv[0]);    //  Gets the queue ID from the argument executed from master.cpp
    rcvmsg(msg_qid);
    return 0;
    //exit(0);
}

