//Ian Sullivan Z1830506
//Assignment 4
//assign4.cpp
//Due date 3/20/21

#include <iostream>
#include <string>
#include <cstring>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <thread>
#include <unistd.h>

using namespace std;

int readerThreads = 1;
int writerThreads = 1;
int iterations = 43; //defaults
const int MAXREADERTHREADS = 20;
string s1 = "All work and no play makes Jack a dull boy.";
sem_t rw_sem; //read write semaphore
sem_t cs_sem; //critical section semaphore
int readcount = 0;


void *reader (void *param);
void *writer (void *param); //rw functions

int main(int argc, char** argv) 
{
    
    if (argc >= 3) {

        readerThreads = stoi(argv[1], nullptr, 10);
        //cout << argv[1] << "\n";
        writerThreads = stoi(argv[2], nullptr, 10);
        //cout << argv[2] << "\n";
        iterations = stoi(argv[3], nullptr, 10);
        //cout << argv[3] << "\n"; 
         
    } else if (argc > 1) {
        //Then they input somewhere between 1-3 arguements but not enough
        cout << "ERROR: Please input either 0 or 3 additional arguments..." << endl;
        exit(1);
    }

    
     if (sem_init(&rw_sem, 0, 1) == 1) {
         cout << "Error: initializing rw semaphore..." << endl;
         exit(-1);
     }
    if (sem_init(&cs_sem, 0, 1) == 1) {
        cout << "Error: initializing the cs semaphore..." << endl;
        exit(-1);
    } //Initialize semaphores to 1
    
    pthread_t rThreads[20]; //The amount of reader threads
    pthread_t wThreads[20]; //the amount of writer threads
    int rc; //return code
    long rt; //reader thread counter
    long readerCount = 0;
    long writerCount = 0;

    for(rt = 0; rt < writerThreads + readerThreads; rt++) {
        
        if(readerCount < readerThreads) {

            readerCount++;
            cout << "Creating new reader thread " << readerCount << endl;
            rc = pthread_create(&rThreads[readerCount], NULL, reader, (void *)readerCount);
        
            if (rc) {
                cout << "ERROR CREATING A READER THREAD" << endl;
                exit(-1);
            }

        }

        if(writerCount < writerThreads) {

            writerCount++;
            cout << "Creating new writer thread " << writerCount << endl;
            rc = pthread_create(&wThreads[writerCount], NULL, writer, (void *)writerCount);

            if (rc) {
                cout << "ERROR CREATING A WRITER THREAD" << endl;
                exit(-1);
            }

        }

    }
   

    pthread_exit(NULL); //Guarentees that main exits after all threads have run 
    sem_destroy(&rw_sem);
    sem_destroy(&cs_sem); //Free up the semaphore resources
    return 0;
}

//Takes a reference to a long int
//This function makes use of semaphores to lock down critical sections during reading and passes access to writers when ready.
//This function also prints our string as it reads it.
//This function returns nothing
void *reader (void *param) {

    long tid;
    tid = (long)param;
    int sem1;
    int sem2;

    for (int i = 0; i < iterations; i++) {
        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader entering critical section value " << sem1 << endl;
        printf("Reader entering critical section value %i\n", sem1);
        sem_wait(&cs_sem); // enter the critical section
        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader has entered crit section value " << sem1 << endl;
        printf("Reader has entered crit section value %i\n", sem1);
        readcount++;
        //cout << "Read count is currently " << readcount << endl;
        printf("Read count is currently %i\n", readcount);
        if (readcount == 1) {
            sem_getvalue(&rw_sem, &sem2);
            //cout << "Waiting for read access value " << sem2 << endl;
            printf("Waiting for read access value %i\n", sem2);
            sem_wait(&rw_sem);
            //cout << "Gotten access value " << sem2 << endl;
            printf("Gotten access value %i\n", sem2);
        }
        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader is exiting crit section value " << sem1 << endl;
        printf("Reader is exiting crit section value %i\n", sem1);
        sem_post(&cs_sem); //exit the critical section
        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader has exited crit section value " << sem1 << endl;
        printf("Reader has exited crit section value %i\n", sem1);

    //Do some reading
        //cout << tid << " is reading..." << endl;
        printf("%li is reading...\n", tid);
        cout << s1 << endl; //This can't be printf'd
        

        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader entering critical section value " << sem1 << endl;
        printf("Reader entering critical section value %i\n", sem1);
        sem_wait(&cs_sem); // enter the critical section
        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader has entered crit section value " << sem1 << endl;
        printf("Reader has entered crit section value %i\n", sem1);
        readcount --;
        //cout << "Read count is currently " << readcount << endl;
        printf("Read count is currently %i\n", readcount);
        if (readcount == 0){
            sem_getvalue(&rw_sem, &sem2);
            //cout << "Giving readwrite access value " << sem2 << endl;
            printf("Giving readwrite access value %i\n", sem2);
            sem_post(&rw_sem);
            sem_getvalue(&rw_sem, &sem2);
            //cout << "Given access value " << sem2 << endl;
            printf("Given access value %i\n", sem2);
        }
        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader is exiting crit section value " << sem1 << endl;
        printf("Reader is exiting crit section value %i\n", sem1);
        sem_post(&cs_sem); //exit the critical section
        sem_getvalue(&cs_sem, &sem1);
        //cout << "Reader has exited crit section value " << sem1 << endl;
        printf("Reader has exited crit section value %i\n", sem1);
        sleep(1);
        }
        
    


    
    pthread_exit(NULL);
}

//This functions takes a reference to a long int
//This function 'writes'. It waits it's turn to get the semaphore then modifies the data chopping off the last char.
//This function returns nothing.
void *writer (void *param) {

    long tid;
    tid = (long)param;
    int sem1;
    //cout << param << endl;
    for(int i = 0; i < iterations; i++) {

        //wait for signal
        sem_getvalue(&rw_sem, &sem1);
        //cout << "Writter is wating for access current value " << sem1 << endl;
        printf("Writter is wating for access current value %i\n", sem1);
        sem_wait(&rw_sem);
        sem_getvalue(&rw_sem, &sem1);
        //cout << "Writter has gotten access current value " << sem1 << endl;
        printf("Writter has gotten access current value %i\n", sem1); 
        
        s1 = s1.substr(0, s1.size() - 1);
        //cout << "Thread :" << tid << " has written..." << endl;
        printf("Thread :%li has written...\n", tid);
        //send signal
        sem_getvalue(&rw_sem, &sem1);
        //cout << "Writter is giving access current value " << sem1 << endl;
        printf("Writter is giving access current value %i\n", sem1);
        sem_post(&rw_sem);
        sem_getvalue(&rw_sem, &sem1);
        //cout << "Writter has given acess current value " << sem1 << endl;
        printf("Writter has given acess current value %i\n", sem1);
        
        sleep(1);
        

    }
    pthread_exit(NULL);
}