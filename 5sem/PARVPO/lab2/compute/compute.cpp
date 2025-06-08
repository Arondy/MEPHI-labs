#include <iostream>
#include "crow_all.h"
#include <vector>
#include "json.hpp"
#include <cmath>
#include <list>
#include <csignal>
#include <cpr/cpr.h>

using std::vector, std::list, std::pair, std::string, std::cout, std::cerr, std::endl;

int global_type = 0;
int global_count = 0;
int global_capacity1 = 0;
int global_capacity2 = 0;

template<typename T>
list<T> vectorToList(const vector<T> &vec){
    return list<T>(vec.begin(), vec.end());
}

list<int> matrixMultiply(const list<int> &matrix1, const list<int> &matrix2, int size){
    list<int> result(size * size, 0);

    auto it = result.begin();

    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            auto it1 = matrix1.begin();
            std::advance(it1, i * size);

            auto it2 = matrix2.begin();
            std::advance(it2, j);

            for (int k = 0; k < size; ++k){
                *it += *it1 * *it2;
                std::advance(it1, 1);
                std::advance(it2, size);
            }

            ++it;
        }
    }

    return result;
}

vector<vector<int>> getData(const string &requestContent){
    string buffer = requestContent;
    nlohmann::json json = nlohmann::json::parse(buffer);

    //cout << messageType << " " << messageContent << endl;

    return json["matrixes"];
}

int send_shutdown_request(){
    string url = "http://nginx:80/shutdown";

    try{
        cpr::Response r = cpr::Post(
            cpr::Url{url}
        );

        if (r.status_code == 200){
            cout << "Request sent successfully, response: " << r.text << endl;
        } else{
            cerr << "Failed to send request, status code: " << r.status_code << endl;
            return -1;
        }
    } catch (const std::exception &e){
        cerr << "Error: " << e.what() << endl;
        return -1;
    }

    return 0;
}

void signalHandler(int signal){
    if (signal == SIGSEGV){
        std::cerr << "Error: Segmentation fault occurred: " << global_type << " " << global_count << " " << global_capacity1 << " " << global_capacity2 << std::endl;
        exit(1);
    }
}

int main(int argc, char *argv[]){
    std::signal(SIGSEGV, signalHandler);
    crow::SimpleApp app;
    crow::logger::setLogLevel(crow::LogLevel::Warning);

    if (argc < 3){
        cout << "Not enough arguments" << endl;
        exit(1);
    }

    const int mSize = std::stoi(string(argv[1]));
    const int matrixFullSize = mSize * mSize;
    int totalMsgs = 2 * matrixFullSize;
    // int serverNum = std::stoi(string(argv[2]));

    vector<vector<int>> matrixes;
    vector<int> m1(matrixFullSize);
    vector<int> m2(matrixFullSize);
    matrixes.push_back(m1);
    matrixes.push_back(m2);
	global_capacity1 = matrixes[0].capacity();
	global_capacity2 = matrixes[1].capacity();

    CROW_ROUTE(app, "/compute")
            .methods("POST"_method)(
                [&](const crow::request &req, crow::response &res){
                    // Read the HTTP request
                    if (totalMsgs){
                        string requestContent = req.body;
                        vector<vector<int>> new_data = getData(requestContent);
                        cout << "Got data (" << new_data.size() << ")" << endl;

                        for (const auto &el: new_data){
                            int type = el[0] - 1;
                            int content = el[1];
                            int count = el[2];
                            global_type = type;
                            global_count = count;
                            // cout << type << " " << count << " ";
                            matrixes[type][count] = content;
                        }
                        // cout << endl;
                        totalMsgs -= new_data.size();
                        cout << "Added data (" << totalMsgs << " left)" << endl;

                        // Construct an HTTP response
                        string responseContent = "success"; // Placeholder for the response content
                        res.code = 200;
                        res.set_header("Content-Type", "text/plain");
                        res.body = responseContent;
                        res.end();

                        if (totalMsgs <= 0){
                            auto timer1 = std::chrono::high_resolution_clock::now();

                            if (matrixes[0].size() == matrixFullSize && matrixes[1].size() == matrixFullSize){
                                cout << "Computing multiplication..." << endl;
                                list<int> result = matrixMultiply(vectorToList(matrixes[0]), vectorToList(matrixes[1]),
                                                                  mSize);
                                // for (int i = 0; i < result.size(); i++) {
                                //     cout << result[i] << " ";
                                // }
                                //cout << endl;
                            } else{
                                cout << "Wrong size!" << endl;
                            }

                            auto timer2 = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> duration = timer2 - timer1;
                            cout << "Time: " << duration.count() << " ms" << endl;
                            // send_shutdown_request();
                            exit(0);
                        }
                    }
                }
            );

    cout << "Starting server... (" << matrixFullSize << ")" << endl;
    app.port(8081).multithreaded().run();
}
