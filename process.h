#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    int id;
    float arrivalTime; //interarrival time
    float serviceTime; //time spent on CPU
    Process *nextProcess; 

    Process(int i, float arr, float serv){
        id = i;
        arrivalTime = arr;
        serviceTime = serv;
    }
};

#endif