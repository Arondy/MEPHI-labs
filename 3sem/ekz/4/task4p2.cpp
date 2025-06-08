#include <vector>
#include <thread>
#include <mutex>
#include <iostream>

#define NNNum 24
#define facNum 4

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

void threadedDist(std::mutex &nextMutex, int &nextStudent, int studNum, const vector<Student> &students, size_t i, vector<vector<const Student *>> results){
    Facility curFacility;
    nextMutex.lock();
    int currentStudent = nextStudent++;
    nextMutex.unlock();

    while (currentStudent < studNum){
        curFacility = runNN(students[currentStudent], i);
        nextMutex.lock();

        if (results[curFacility].size() > studNum / facNum){
            curFacility = static_cast<Facility>(findMin(results));
        }
        (results[curFacility]).push_back(&(students[currentStudent]));
        assignFacility(students[currentStudent], curFacility);
        currentStudent = nextStudent++;

        nextMutex.unlock();
    }
}

void distribute(const vector<Student> &students){
    const int studNum = students.size();
    vector<vector<const Student *>> results(4);
    int nextStudent = 0;
    std::mutex nextMutex;
    vector<std::thread> threads(NNNum);
    vector<Facility> facilities(NNNum);

    for (size_t i = 0; i < NNNum; ++i){
        threads[i] = std::thread(
                [=, &nextMutex, &nextStudent, &results]
                { threadedDist(nextMutex, nextStudent, studNum, students, i, results); });
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