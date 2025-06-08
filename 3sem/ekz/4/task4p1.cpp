#include <vector>
#include <thread>
#include <mutex>
#include <iostream>

#define NNNum 24

using std::vector, std::string;

enum Facility {
    graphfinder, czerin, configrun, pythonduj, unknown
};

class Student {
public:
    int x;
    Student() : x(0){}
    Student(int x) : x(x){}
};

void assignFacility(Student student, Facility facility){
    string message = std::to_string(student.x) + " assignment complete\n";
    std::cout << message;
}

Facility runNN(const Student &student, int N){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int r = std::rand() % 4;
    switch (r){
        case 0:
            return graphfinder;
        case 1:
            return czerin;
        case 2:
            return configrun;
        case 3:
            return pythonduj;
    }
}

void threadedDist(std::mutex &nextMutex, int &nextStudent, int studNum, const vector<Student> &students, size_t i){
    nextMutex.lock();
    int currentStudent = nextStudent++;
    nextMutex.unlock();
    while (currentStudent < studNum){
        assignFacility(students[currentStudent], runNN(students[currentStudent], i));
        nextMutex.lock();
        currentStudent = nextStudent++;
        nextMutex.unlock();
    }
}

void distribute(const vector<Student> &students){
    int studNum = students.size();
    vector<Facility> results(students.size());
    int nextStudent = 0;
    std::mutex nextMutex;
    vector<std::thread> threads(NNNum);
    vector<Facility> facilities(NNNum);
    for (size_t i = 0; i < NNNum; ++i){
        threads[i] = std::thread(
                [=, &nextMutex, &nextStudent]
                { threadedDist(nextMutex, nextStudent, studNum, students, i); });
    }
    for (size_t j = 0; j < NNNum; ++j){
        threads[j].join();
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