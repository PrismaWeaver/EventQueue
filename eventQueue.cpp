#include <stdio.h>
#include <random>
#include <ctime>
#include <iostream>

#include "event.h"

#define ARR 0
#define DEP 1

class Event_Queue {
    private:
        int queueCount, processCount;
        int lamb;
        long double clock, serv, turnaround, throughput, utilization, RQsize;
        bool idle;
        Event *head;      

    public:
        Event_Queue(int lambda, float service) {
            srand(time(0));
            queueCount = processCount = 0;
            lamb = lambda;
            serv = service;
            clock = turnaround = throughput = utilization = RQsize = 0.0;
            idle = true;
            Process *p = new Process(1, generateArrivalTime(), generateServiceTime());
            head = NULL;
            schedule_event(ARR, p->arrivalTime, p);
        }

        ~Event_Queue(){ //ensures that the rest of the event queue is emptied after the run completes
            Event *temp;
            while (head != NULL) {
                temp = head->nextEvent;
                free(head);
                head = temp;
            }
        }

        void run() {
            Event *temp, *e = head;
            long double old_clock = 0.0, time = 0.0;
            //this collects the data while running the sim
            std::cout << "simulation beginning" << std::endl;
            while(processCount < 10000){ //exits once 10000 processes have been completed
                old_clock = clock;
                clock = e->time;
                time = clock - old_clock;
                if (idle) utilization += time; //calculate utilization
                RQsize += queueCount * time; //calculate mean queue size
                switch(e->type) {
                    case ARR:
                        ARR_handler(e);
                        break;
                    case DEP:
                        DEP_handler(e);
                        break;
                }
                e = e->nextEvent;
            }
            std::cout << "simulation complete" << std::endl;
            //these are the metrics that are to be returned at the end
            turnaround = turnaround / processCount;
            throughput = processCount / clock;
            utilization = (1 - (utilization / clock)) * 100; //since utilization keeps track of idle time
            RQsize = RQsize / clock;
        }

        void schedule_event(int type, long double t, Process *p) {
            Event* e = new Event(type, t, p), *temp = head;
            if (head == NULL) head = e;
            else {
                while (!eventCompare(e, temp->nextEvent)) temp = temp->nextEvent;
                e->nextEvent = temp->nextEvent;
                temp->nextEvent = e;
            }
        }

        bool eventCompare(Event *e1, Event *e2) {
            if (e2 == NULL) return true;
            else if (e1->time < e2->time) return true;
            return false;
        }

        void ARR_handler(Event *e) { 
            if(idle == true) {
                idle = false;
                schedule_event(DEP, e->time + e->p->serviceTime, e->p); 
            }
            else queueCount++;
            e->p->nextProcess = new Process(e->p->id + 1, e->time + generateArrivalTime(), generateServiceTime());
            schedule_event(ARR, e->p->nextProcess->arrivalTime, e->p->nextProcess); 
        }

        void DEP_handler(Event *e) {
            turnaround += e->time - e->p->arrivalTime; //calculate the turnaround
            if (queueCount == 0) idle = true;
            else {
                queueCount--;
                schedule_event(DEP, e->time + e->p->nextProcess->serviceTime, e->p->nextProcess); 
            }
            processCount++;
            free(e->p);
        }

        float generateArrivalTime() {
            double p = 1.0;
            int x = 0;
            while (p >= exp(-lamb)) {
                x++;
                p *= (float) rand()/RAND_MAX;
            }
            return 1.0/(x-1);
        }

        float generateServiceTime() {
            float r =0;
            while(r == 0) r = 1-((float) rand()/RAND_MAX);
            return -serv*log(r);
        }

        long double fetchTurnaround() {return turnaround;}
        long double fetchThroughput() {return throughput;}
        long double fetchUtilization() {return utilization;}
        long double fetchAvgRQSize() {return RQsize;}
};