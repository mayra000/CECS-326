/* shmp1.cpp */
#include "registration.h"
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

/*declaring new CLASS structure
 class number = 1001
 date = 12-01-19
 title = operating systems
 amount of seats left = 15 */
CLASS myclass = { "1001", "120119", "Operating Systems", 15 };
#define NCHILD 3 // there are 3 child processes being created

//predefining functions before they are called in order to avoid error
int shm_init( void * );
void wait_and_wrap_up( int [], void *, int );
void rpterror( char *, char * );

int main(int argc, char *argv[])
{
    int child[NCHILD], i, shmid; // storing child PIDs  ( i = # of process, shmid = Shared memory segment ID)
    void *shm_ptr; //shared memory pointer
    char ascshmid[10], pname[14]; //character array of shared memory segment and process name
    shmid = shm_init(shm_ptr); //initializes shared memory segment and assigns its identifier to shmid
    sprintf (ascshmid, "%d", shmid); //storing shmid to char array aschmid
    
    for (i = 0; i < NCHILD; i++) { //creating NCHILD amount of processes
        child[i] = fork(); //creating new child process
        switch (child[i]) {
            case -1: //failed creation of child process
                rpterror ("fork failure", pname); //printing out error mesage
                exit(1); //exits the process with specific error code
                
            case 0: //successful creation of child process
                sprintf (pname, "shmc%d", i+1); //Stores the process in the format "shmc%d" into pname
                execl("shmc1", pname, ascshmid, (char *)0); //replaces the program code with shmc1 and passes the pname and the PID as the arguments
                perror ("execl failed"); //error message printing
                exit (2); //exits the process with specific error code
        }
    }
    /*
     Waits for all the child processes in child to terminate before detaching the shared memory segment from shm_ptr and destroys the shared memory segment from the system
     */
    wait_and_wrap_up (child, shm_ptr, shmid);}

// shared memory segment is initialized and assigns to the pointer
int shm_init(void *shm_ptr) //returns shared memory segment ID
{
    int shmid; //shared memory segment ID
    /*
     creates shared memory segment as the size of the CLASS structure
     */
    shmid = shmget(ftok(".",'u'), sizeof(CLASS), 0600 | IPC_CREAT);
    if (shmid == -1) { //acquiring shared memory segment has failed
        perror ("shmget failed"); //error message
        exit(3); //exits with appropiate error message
    }
    shm_ptr = shmat(shmid, (void * ) 0, 0); //shm_ptr is attatched the created shared memory segment
    if (shm_ptr == (void *) -1) { //indicates that attatchment failed
        perror ("shmat failed"); //error message
        exit(4);
    }
    //data of the created CLASS structure is copied into shm_ptr
    memcpy (shm_ptr, (void *) &myclass, sizeof(CLASS) ); //size of data = bytes
    return (shmid);
}
/*
 Waits for all child processes to terminate before detaching the shared memory segment from the pointer *shm_ptr and removing the shared memory segment associated with the identifier shmid
 */
void wait_and_wrap_up(int child[], void *shm_ptr, int shmid)
{
    /*
     wait_rtn = flag that indicates the status of the child
     w = index of child process
     ch_active = # of child processes that are active
     */
    int wait_rtn, w, ch_active = NCHILD;
    while (ch_active > 0) { //runs until there are no child processes active
        wait_rtn = wait( (int *)0 );//waits for child process to terminate
        //decrements the amount of child processes running
        for (w = 0; w < NCHILD; w++)
            if (child[w] == wait_rtn) {
                ch_active--; //decrementing amount of child processes that are active
                break;
            }
    }
    cout << "Parent removing shm" << endl; //states which shm was removed
    shmdt (shm_ptr); //detaches the shared memory segment associated with the pointer
    shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0); //Removes the shared memory segment identifier specified with shmid and destroys the shared memory segment and data structure associated
    exit (0);
}

/*
 printing out message that states which parameters were passed
 also prints out error message
 */
void rpterror(char *string, char *pname)
{
    char errline[50]; //declaring array size
    sprintf (errline, "%s %s", string, pname); //storing passed parameters and concactening them
    perror (errline); //printing out error message
}
