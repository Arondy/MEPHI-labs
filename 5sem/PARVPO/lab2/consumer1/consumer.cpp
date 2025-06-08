#include <iostream>
#include "crow_all.h"
#include <vector>
#include "json.hpp"
#include <cmath>
#include <list>
#include <cpr/cpr.h>

using std::vector, std::map, std::list, std::string, std::cout, std::endl, std::cerr;

vector<vector<int>> matrixes;
map<int, bool> got_data = {{1, false}, {2, false}};

vector<int> getData(const string &requestContent){
    string buffer;

    for (int i = 0; i < requestContent.length(); i++){
        buffer += requestContent[i];
    }

    nlohmann::json json = nlohmann::json::parse(buffer);

    int messageType = json["message_type"];
    int messageContent = json["message_content"];
    int messageCount = json["count"];

    //cout << messageType << " " << messageContent << endl;

    return vector<int>{messageType, messageContent, messageCount};
}

int send_compute_request(){
    string url = "http://nginx:80/compute";
    
//    int chunk = 1e6;

//    for (int i = 0; i * chunk < matrixes.size(); i++){
//        auto first = matrixes.begin() + i * chunk;
//        auto last = matrixes.begin() + std::min((i + 1) * chunk, static_cast<int>(matrixes.size()));
		//cout << "Creating vec" << endl;
		//cout << std::min((i + 1) * chunk, static_cast<int>(matrixes.size())) << endl;
//        vector<vector<int>> data_chunk(first, last);
		//cout << "Created vec!" << endl;
    nlohmann::json data = {{"matrixes", matrixes}};


    // Отправка POST-запроса с помощью cpr
    try{
        cpr::Response r = cpr::Post(
            cpr::Url{url},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Body{data.dump()}
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
//    }
    return 0;
}

int main(){
    crow::SimpleApp app;
    crow::logger::setLogLevel(crow::LogLevel::Warning);

    CROW_ROUTE(app, "/")
            .methods("POST"_method)(
                [&](const crow::request &req, crow::response &res){
                    // Read the HTTP request
                    string requestContent = req.body;

                    // Check the type of producer
                    auto data = getData(requestContent);
                    int msg_type = data[0], msg_content = data[1];

                    if (matrixes.size() % 5000 == 0){
                        cout << matrixes.size() << " messages received" << endl;
                    }

                    if (msg_content == -1){
                        got_data[msg_type] = true;
                    } else{
                        matrixes.push_back(data);
                    }

                    // Construct an HTTP response
                    string responseContent = std::to_string(msg_content); // Placeholder for the response content
                    res.code = 200;
                    res.set_header("Content-Type", "text/plain");
                    res.body = responseContent;
                    res.end();
                }
            );

    CROW_ROUTE(app, "/end").methods("POST"_method)(
        [&](const crow::request &req, crow::response &res){
            string requestContent = req.body;
            auto data = getData(requestContent);
            int msg_type = data[0];
            got_data[msg_type] = true;
            // cout << got_data[1] << " " << got_data[2] << endl;
			
			// Construct an HTTP response
			string responseContent = "success"; // Placeholder for the response content
			res.code = 200;
			res.set_header("Content-Type", "text/plain");
			res.body = responseContent;
			res.end();
			
			if (got_data[1] && got_data[2]){
                cout << "Receiving is completed. Sending compute request..." << endl;
                send_compute_request();
                exit(0);
            }
        }
    );

    cout << "Starting server..." << endl;
    app.port(8080).multithreaded().run();
}
