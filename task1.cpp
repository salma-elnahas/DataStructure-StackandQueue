#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip> // For fixed and setprecision

using namespace std;
// template to create a node for a linked list.
template <class T>
struct Node {
    T info;
    Node* next;
    Node(T job) : info(job), next(nullptr) {}
};
// template for queue that stores print jobs using a linked list.
template <class T>
class Queue {
    Node<T>* front;
    Node<T>* rear;
public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enQueue(T& job) {
        Node<T>* newNode = new Node<T>(job);
        if (rear != nullptr)
            rear->next = newNode;
        else
            front = newNode;
        rear = newNode;
    }

    T deQueue() {
        if (isEmpty()) {
            cout << "Printer Queue is Empty" << endl;
            exit(1);
        }
        Node<T>* delptr = front;
        T job = front->info;
        front = front->next;
        if (front == nullptr)
            rear = nullptr;
        delete delptr;
        return job;
    }

    bool isEmpty() {
        return front == nullptr;
    }
};
// structure for a print job object.
struct printJob {
    string pcName;
    string arrivalStr;
    int arrivalTime;
    int Duration;
    int executionTime;
    int waitingTime;

    printJob(string PC, string time, int dur) : pcName(PC), arrivalStr(time), Duration(dur) {
        executionTime = waitingTime = 0;
        arrivalTime = getMinutes(time);
    }
    // method to convert time in "hh:mm" format to total minutes
    int getMinutes(string timeStr) {
        int hours = stoi(timeStr.substr(0, 2));
        int minutes = stoi(timeStr.substr(3, 2));
        return hours * 60 + minutes;
    }
//  operator< to sort print jobs based on arrival time and duration
    bool operator<(const printJob& other) const {
        if (arrivalTime == other.arrivalTime)
            return Duration < other.Duration;
        return arrivalTime < other.arrivalTime;
    }
};

int main() {
    ifstream file("Printer.txt");
    if (!file) {
        cout << "File can't be opened" << endl;
        return -1;
    }

    vector<printJob> jobs;// vector to store the jobs read from the file and store them later
    int num;
    file >> num; // read number of jobs from file
    string pc, timeStr;
    int duration;

    for (int i = 0; i < num; i++) {
        file >> pc >> timeStr >> duration;
        jobs.push_back(printJob(pc, timeStr, duration));// built in function to add created objects to vector
    }

    sort(jobs.begin(), jobs.end());// a built in function for arrival time and duration

    Queue<printJob> queue1;
    for (int i = 0; i < jobs.size(); i++) {
        queue1.enQueue(jobs[i]); // add each job into queue
    }

    int totalTime = 0 ;// track current time
    int waitingSum = 0;
    int timeOccupied = 0;// track occupied duration

    cout << "Jobs Executed: " << num << endl;

    while (!queue1.isEmpty()) {
        printJob job = queue1.deQueue();//dequeue the next job to process others

        if(totalTime < job.arrivalTime)// if current time less than arrival time(means unoccupied)  set it to current
        {
            totalTime = job.arrivalTime;
        }

        job.waitingTime = totalTime - job.arrivalTime;
        job.executionTime = job.Duration;

        waitingSum += job.waitingTime;// add waiting time to total

        timeOccupied += job.executionTime;//add execution or duration to occupied time
       
        // format time again into hh:mm of job finished
        int hours = totalTime / 60;
        int minutes =  totalTime % 60;
        totalTime += job.executionTime;// update current time after job started

        cout << job.pcName << " "
             << job.arrivalStr << " "
             << job.Duration << " "
             << "executed at "
             << ((hours < 10) ? "0" : "") << hours << ":"// ternary operator for format
             << ((minutes < 10) ? "0" : "") << minutes << " "
             << "waiting time: " << job.waitingTime << endl;
 

       
    }
    // after while loop output total time the printer was occupied and the average waiting time
    cout << "The printer was occupied for " << timeOccupied  << " minutes" << endl;
    cout << "Average waiting time for all jobs is " << fixed << setprecision(2) << (double)waitingSum / num << " minutes" << endl;

    return 0;
}
