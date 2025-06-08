#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <queue>

#define NNNum 24
#define facNum 4

using std::vector, std::string;

enum Facility {
    graphfinder, czerin, configrun, pythonduj, unknown
};

class Student {
public:
    int x;
    Student() : x(-1){}
    Student(int x) : x(x){}
};

void assignFacility(Student student, Facility facility, int i){
    string message = std::to_string(student.x) + " assignment completed by thread " + std::to_string(i) + "\n";
    std::cout << message;
}

Facility runNN(const Student &student, int N){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int r = std::rand() % 5;
    switch (r){
        case 0:
            return graphfinder;
        case 1:
            return czerin;
        case 2:
            return configrun;
        case 3:
            return pythonduj;
        case 4:
            return unknown;
    }
}

template <typename T>
int findMin(const vector<T> input){
    int cur, min = 9999, res = -1;
    for (int i = 0; i < input.size(); ++i){
        cur = input[i].size();
        if (cur < min){
            min = cur;
            res = i;
        }
    }
    return res;
}

/*!
 * ATTENTION! The distribute function code contents:\n
 *  - Violence;\n
 *  - Pornographic & BDSM content;\n
 *  - Self-harm & suicide scenes;\n
 *  - Blood & murder.\n
 *  \n
 *  FOR MENTALLY STABLE PROGRAMMERS ONLY!
 */

void threadedDist(std::mutex &nextMutex, int studNum, const vector<Student> &students, size_t i,
                  vector<vector<const Student *>> results, std::queue<size_t> &queue, int &completeCounter){
    Facility curFacility;

    threadStart:

    nextMutex.lock();
    size_t currentStudent = (queue.empty()) ? -1 : queue.front();
    if (!queue.empty()){
        queue.pop();
    }
    nextMutex.unlock();

    while (currentStudent != -1){
        curFacility = runNN(students[currentStudent], i);
        if (curFacility == unknown){
            nextMutex.lock();
            queue.push(currentStudent);
            nextMutex.unlock();
            string message = std::to_string(currentStudent) + " dropped by thread " + std::to_string(i) + "\n";
            std::cout << message;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            goto takeNextStudent;
        }
        nextMutex.lock();

        if (results[curFacility].size() > studNum / 3){
            curFacility = static_cast<Facility>(findMin(results));
        }
        (results[curFacility]).push_back(&(students[currentStudent]));
        nextMutex.unlock();
        assignFacility(students[currentStudent], curFacility, i);
        ++completeCounter;

        takeNextStudent:

        nextMutex.lock();
        currentStudent = (queue.empty()) ? -1 : queue.front();
        if (!queue.empty()){ queue.pop(); }

        nextMutex.unlock();
    }
    if (completeCounter < studNum)  // while there are any unassigned students
    {
        goto threadStart;   // every free thread will wait for them,
    }                       // because current running NN will overheat.
}

void distribute(const vector<Student> &students){
    const int studNum = students.size();
    vector<vector<const Student *>> results(4);
    int completeCounter = 0;
    std::queue<size_t> queue;
    for (size_t i = 0; i < students.size(); ++i){
        queue.push(i);
    }
    std::mutex nextMutex;
    vector<std::thread> threads(NNNum);
    vector<Facility> facilities(NNNum);

    for (size_t i = 0; i < NNNum; ++i){
        threads[i] = std::thread(
                [=, &nextMutex, &queue, &results, &completeCounter]{
                    threadedDist(nextMutex, studNum, students, i, results, queue, completeCounter);
                });
    }

    for (size_t j = 0; j < NNNum; ++j){
        threads[j].join();
    }
    for (size_t i = 0; i < results.size(); ++i){
        for (size_t j = 0; j < results[i].size(); ++j){
            std::cout << results[i][j]->x << ", ";
        }
        std::cout << std::endl;
    }
}

int main(){
    vector<Student> students(100);
    for (int i = 0; i < 100; ++i){
        students[i].x = i;
    }
    distribute(students);
    std::cout << std::endl;
}