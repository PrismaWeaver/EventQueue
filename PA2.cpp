#include <iostream>

#include "eventQueue.cpp"

int main(int argc, char *argv[]){ //accepts two args: avg arrival rate (10-30), avg service time (0.04)
    //makes sure the command line variables are set correctly
    if(argc < 3 || argc > 3) {
        std::cout << "incorrect number of arguments: " << std::endl;
        return 0;
    }
    //initialization
    std::cout << "initializing" << std::endl;
    int lambda = std::stoi(argv[1]);
    float serv = std::stof(argv[2]);
    std::cout << "lambda set to: " << lambda << " and avg service time set to: " << serv << std::endl;
    //initializes the eventQueue with this process list
    Event_Queue EQ(lambda, serv);
    EQ.run();
    std::cout << "Metrics for Lambda of " << lambda << " and service time of " << serv << std::endl;
    //then outputs:
    //  mean Turnaround Time (sum of process (completion - arrival) / # of processes)
    std::cout << "Average Turnaround Time of a process: " << EQ.fetchTurnaround() << std::endl;
    //  throughput (processes / clock at end of simulation)
    std::cout << "Total Throughput: " << EQ.fetchThroughput() << std::endl;
    //  mean utilization (accumulated time where idle == false / clock at end of simulation)
    std::cout << "Average CPU Utilization: " << EQ.fetchUtilization() << std::endl;
    //  mean processes in RQ: sum of all processes (ammount in queue * (current time - last change time)) / total time
    std::cout << "Average number of processes in the Ready Queue: " << EQ.fetchAvgRQSize() << std::endl;
}

