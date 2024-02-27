#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Struct {
public:
    int processId;
    int burstTime;
    int remainingTime;//Bursttime-time quantum
    int processPriority;
    string processState;

    Struct(int pid, int burst_time, int priority = 0)
        : processId(pid), burstTime(burst_time), remainingTime(burst_time), processPriority(priority), processState("Ready") {}
};
class ReadyQueue {
public:
    queue<Struct> q;

    void addProcess(Struct process) {
        q.push(process);
    }

    Struct getNextProcess() {
        Struct process = q.front();
        q.pop();
        return process;
    }

    bool isEmpty() {
        return q.empty();
    }
};

void dispatcher(Struct& currentProcess, int timeQuantum) {
    if (currentProcess.remainingTime <= timeQuantum) {
        cout << "Executing process " << currentProcess.processId << " for " << currentProcess.remainingTime << " time units" << endl;
        currentProcess.processState = "Terminated";
        currentProcess.remainingTime = 0;
    } else {
        cout << "Executing process " << currentProcess.processId << " for " << timeQuantum << " time units" << endl;
        currentProcess.remainingTime -= timeQuantum;
    }
}

void createProcess(int pid, int burstTime, ReadyQueue& readyQueue) {
    Struct newProcess(pid, burstTime);
    readyQueue.addProcess(newProcess);
}

void terminateProcess(int processId, int burstTime, int remainingTime) {
    if (remainingTime > 0) {
        cout << "Adding process " << processId << " back to the queue with " << remainingTime << " remaining time" << endl;
    } else {
        cout << "Terminating process " << processId << endl;
    }
}

void roundRobinScheduling(int processIds[], int n, int burstTimes[], int timeQuantum) {
    int waitingTimes[n];
    int totalWaitingTime = 0;
    int turnaroundTimes[n];
    int totalTurnaroundTime = 0;
    int remainingTimes[n];

    for (int i = 0; i < n; i++) {
        remainingTimes[i] = burstTimes[i];
    }

    int t = 0;
    bool done = false;

    while (!done) {
        done = true;
        for (int i = 0; i < n; i++) {
            if (remainingTimes[i] > 0) {
                done = false;
                if (remainingTimes[i] > timeQuantum) {
                    t += timeQuantum;
                    remainingTimes[i] -= timeQuantum;
                } else {
                    t += remainingTimes[i];
                    waitingTimes[i] = t - burstTimes[i];
                    remainingTimes[i] = 0;
                }
                terminateProcess(processIds[i], burstTimes[i], remainingTimes[i]);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        turnaroundTimes[i] = burstTimes[i] + waitingTimes[i];
        totalWaitingTime += waitingTimes[i];
        totalTurnaroundTime += turnaroundTimes[i];
    }

    cout << "Process\t" << "|""\t" << "Burst Time\t" << "|""\t" << "Waiting Time\t" << "|""\t" << "Turnaround Time""\n";
    for (int i = 0; i < n; i++) {
        cout << processIds[i] << "\t\t\t\t" << burstTimes[i] << "\t\t\t\t\t" << waitingTimes[i] << "\t\t\t\t\t" << turnaroundTimes[i] << endl;
    }

    cout << "Average Waiting Time = " << (float)totalWaitingTime / n << endl;
    cout << "Average Turnaround Time = " << (float)totalTurnaroundTime / n << endl;
}

int main() {
    int timeQuantum;
    int numProcesses;

    cout << "Enter the number of processes: ";
    cin >> numProcesses;

    cout << "Enter the time quantum: ";
    cin >> timeQuantum;

    ReadyQueue readyQueue;
    cout << "       " << " Execution steps:" << endl;
    cout << "---------------------------------------------------------------" << endl;

    int processIds[numProcesses];
    int burstTimes[numProcesses];

    for (int i = 0; i < numProcesses; i++) {
        cout << "Enter burst time for process " << (i + 1) << ": ";
        cin >> burstTimes[i];
        processIds[i] = i + 1;
    }

    roundRobinScheduling(processIds, numProcesses, burstTimes, timeQuantum);
    cout << "----------------------------------------------------------------" << endl;

    cout << "All processes have been executed. Thank you!!!" << endl;

    return 0;
}

