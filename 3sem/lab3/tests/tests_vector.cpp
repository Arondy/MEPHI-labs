#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../vector/vector.h"
#include "buffer.h"

using std::string, std::shared_ptr, std::make_shared;
using MyVec::vector, MyVec::VectorIterator;

TEST_CASE("Simple value (string)"){
    SECTION("Constructors"){
        SECTION("Default"){
            vector<string> v{};
        }SECTION("By _capacity"){
            vector<string> v{10};
            REQUIRE(!v.empty());
        }SECTION("By size and value"){
            vector<string> v{10, "- Wait, placement new is an answer?\n- Always has been..."};
            REQUIRE(v.size() == 10);
            REQUIRE(v[0] == "- Wait, placement new is an answer?\n- Always has been...");
            REQUIRE(v[5] == "- Wait, placement new is an answer?\n- Always has been...");
        }SECTION("By init list"){
            vector<string> v{"Wait, placement new is an answer?", "Always has been...", "That's just sad",
                             "Certified C++ moment"};
            REQUIRE(v.size() == 4);
            REQUIRE(v[0] == "Wait, placement new is an answer?");
            REQUIRE(v[3] == "Certified C++ moment");
        }SECTION("Copy"){
            vector<string> v{"Wait, placement new is an answer?", "Always has been..."};
            vector<string> v2{v};
            REQUIRE(v2.size() == v.size());
            REQUIRE(v2[1] == v[1]);
        }SECTION("Move"){
            vector<string> v(vector<string>{"Wait, placement new is an answer?", "Always has been..."});
            REQUIRE(v.size() == 2);
        }
    }

    SECTION("Operators"){
        SECTION("= - Copy"){
            vector<string> v{"Wait, placement new is an answer?", "Always has been..."};
            vector<string> v2 = v;
            REQUIRE(v2.size() == v.size());
            REQUIRE(v2[1] == v[1]);
        }SECTION("= - Move"){
            vector<string> v{"Wait, placement new is an answer?", "Always has been..."};
            int tempSize = v.size();
            string tempElem = v[0];
            vector<string> v2 = std::move(v);
            REQUIRE(v2.size() == tempSize);
            REQUIRE(v2[0] == tempElem);
        }SECTION("= - Init list"){
            vector<string> v{};
            v = {"Wait, placement new is an answer?", "Always has been..."};
            REQUIRE(v.size() == 2);
            REQUIRE(v[1] == "Always has been...");
            vector<string> v2{10, "123"};
            v2 = {"Wait, placement new is an answer?", "Always has been..."};
            REQUIRE(v.size() == 2);
            REQUIRE(v[1] == "Always has been...");
        }SECTION("[] - const"){
            vector<string> v{"Wait, placement new is an answer?", "Always has been..."};
            REQUIRE(v[0] == "Wait, placement new is an answer?");
            REQUIRE(v[1] == "Always has been...");
        }SECTION("[] - non const"){
            vector<string> v{"Wait, placement new is an answer?", "Always has been..."};
            v[0] = "Line without a joke";
            REQUIRE(v[0] == "Line without a joke");
        }
    }

    SECTION("Methods (without iterators)"){
        SECTION("Size"){
            vector<string> v{};
            REQUIRE(v.empty());
            v.push_back("1");
            REQUIRE(v.size() == 1);
        }SECTION("Capacity"){
            vector<string> v{};
            REQUIRE(v.capacity() == 0);
            v.push_back("1");
            REQUIRE(v.capacity() == 1);
            v.push_back("2");
            v.push_back("3");
            REQUIRE(v.capacity() == 4);
            v.push_back("4");
            v.push_back("5");
            REQUIRE(v.capacity() == 8);
            v.reserve(20);
            REQUIRE(v.capacity() == 20);
        }SECTION("Empty"){
            vector<string> v{};
            REQUIRE(v.empty());
            v.push_back("1");
            REQUIRE(!v.empty());
        }SECTION("Reserve"){
            vector<string> v{};
            v.reserve(10);
            v.reserve(20);
            v.reserve(10);
            REQUIRE(v.empty());
            vector<string> v2{10};
            v2.reserve(20);
            REQUIRE(v2.size() == 10);
            vector<string> v3{10, "123"};
            v3.reserve(20);
            REQUIRE(v3.size() == 10);
            REQUIRE(v3[9] == "123");
        }SECTION("At - const"){
            vector<string> v{10, "123"};
            REQUIRE(v.at(5) == "123");
            REQUIRE_THROWS(v.at(20) == "123");
            REQUIRE_THROWS(v.at(-2) == "123");
        }SECTION("At - non const"){
            vector<string> v{10, "123"};
            v.at(5) = "456";
            REQUIRE_THROWS(v.at(20) = "456");
            REQUIRE_THROWS(v.at(-2) = "456");
        }SECTION("Front - const"){
            vector<string> v{10, "123"};
            REQUIRE(v.front() == "123");
            v[0] = "456";
            REQUIRE(v.front() == "456");
        }SECTION("Front - non const"){
            vector<string> v{10, "123"};
            REQUIRE(v.front() == "123");
            v.front() = "456";
            REQUIRE(v[0] == "456");
        }SECTION("Back - const"){
            vector<string> v{10, "123"};
            REQUIRE(v.back() == "123");
            v.push_back("1");
            REQUIRE(v.back() == "1");
        }SECTION("Back - non const"){
            vector<string> v{10, "123"};
            v.back() = "1";
            REQUIRE(v[9] == "1");
        }SECTION("Data - const"){
            vector<string> v{10, "123"};
            string *data = v.data();
            REQUIRE(data[0] == "123");
            REQUIRE(data[9] == "123");
        }SECTION("Push back - Copy"){
            vector<string> v{};
            string temp = "1";
            v.push_back(temp);
            REQUIRE(v.size() == 1);
            REQUIRE(v.back() == "1");
        }SECTION("Push back - Move"){
            vector<string> v{};
            v.push_back("1");
            REQUIRE(v.size() == 1);
            REQUIRE(v.back() == "1");
        }SECTION("Clear"){
            vector<string> v{};
            v.clear();
            vector<string> v2{10, "123"};
            v2.clear();
            REQUIRE(v2.empty());
            REQUIRE_NOTHROW(v2[9] = "1");
            REQUIRE(v2[9] == "1");
            v2.push_back("345");
            REQUIRE(v2.size() == 1);
            REQUIRE(v2[0] == "345");
        }
    }

    SECTION("Methods (with iterators)"){
        using iterator = VectorIterator<string, false>;
        using const_iterator = VectorIterator<string, true>;
        SECTION("Begin and end"){
            vector<string> v{"1", "2"};
            iterator it = v.begin();
            REQUIRE(*it == "1");
            it = v.end();
            REQUIRE(*(--it) == "2");
            const_iterator cIt = v.cbegin();
            REQUIRE(*cIt == "1");
            cIt = v.cend();
            REQUIRE(*(--cIt) == "2");
        }SECTION("Find"){
            vector<string> v{"1", "2", "3", "4", "5"};
            iterator it = std::find(v.begin(), v.end(), "4");
            if (it != v.end()){
                REQUIRE(*it == "4");
            }
            it = std::find(v.begin(), v.end(), "6");
            REQUIRE(it == v.end());
        }SECTION("Erase - one"){
            vector<string> v{"1", "2", "3", "4", "5"};
            iterator it = std::find(v.begin(), v.end(), "4");
            v.erase(it);
            REQUIRE(v.size() == 4);
            REQUIRE(v[2] == "3");
            REQUIRE(v[3] == "5");
            it = std::find(v.begin(), v.end(), "6");
            REQUIRE_NOTHROW(v.erase(it));
        }SECTION("Erase - range"){
            vector<string> v{"1", "2", "3", "4", "5"};
            v.erase(v.begin(), v.begin() + 2);
            REQUIRE(v.size() == 3);
            REQUIRE(v[0] == "3");
            REQUIRE(v[1] == "4");
            REQUIRE(v[2] == "5");
            vector<string> v2{"1", "2", "3", "4", "5"};
            v2.erase(v2.end() - 1, v2.end());
            REQUIRE(v2.size() == 4);
            REQUIRE(v2.back() == "4");
            vector<string> v3{"1", "2", "3", "4", "5"};
            v3.erase(v3.begin() + 1, v3.end() - 1);
            REQUIRE(v3.size() == 2);
            REQUIRE(v3[0] == "1");
            REQUIRE(v3[1] == "5");
        }
    }
}

TEST_CASE("Complex value (Buffer of strings)"){
    SECTION("Constructors"){
        SECTION("Default"){
            vector<Buffer> v{};
        }SECTION("By _capacity"){
            vector<Buffer> v{10};
            REQUIRE(!v.empty());
        }SECTION("By size and value"){
            vector<Buffer> v{10, Buffer{5}};
            REQUIRE(v.size() == 10);
        }SECTION("By init list"){
            vector<Buffer> v{Buffer{1}, Buffer{5}, Buffer{10}, Buffer{20}};
            REQUIRE(v.size() == 4);
        }SECTION("Copy"){
            vector<Buffer> v{Buffer{1}, Buffer{5}, Buffer{10}, Buffer{20}};
            vector<Buffer> v2{v};
            REQUIRE(v2.size() == v.size());
            REQUIRE(v2[1] == v[1]);
        }SECTION("Move"){
            vector<Buffer> v(vector<Buffer>{Buffer{1}, Buffer{5}});
            REQUIRE(v.size() == 2);
        }
    }

    SECTION("Operators"){
        SECTION("= - Copy"){
            vector<Buffer> v{Buffer{1}, Buffer{5}, Buffer{10}, Buffer{20}};
            vector<Buffer> v2 = v;
            REQUIRE(v2.size() == v.size());
            REQUIRE(v2[1] == v[1]);
        }SECTION("= - Move"){
            vector<Buffer> v{Buffer{1}, Buffer{5}, Buffer{10}, Buffer{20}};
            int tempSize = v.size();
            Buffer tempElem = v[0];
            vector<Buffer> v2 = std::move(v);
            REQUIRE(v2.size() == tempSize);
            REQUIRE(v2[0] == tempElem);
        }SECTION("= - Init list"){
            vector<Buffer> v{};
            v = vector<Buffer>{Buffer{1}, Buffer{5}};
            REQUIRE(v.size() == 2);
            vector<Buffer> v2{10, Buffer{5}};
            v2 = vector<Buffer>{Buffer{1}, Buffer{5}};
            REQUIRE(v.size() == 2);
        }SECTION("[] - const"){
            vector<Buffer> v{Buffer{1}, Buffer{5}, Buffer{10}, Buffer{20}};
            Buffer b{1};
            REQUIRE(v[0] == b);
        }SECTION("[] - non const"){
            vector<Buffer> v{Buffer{1}, Buffer{5}, Buffer{10}, Buffer{20}};
            Buffer b{1};
            v[0] = b;
        }
    }

    SECTION("Methods (without iterators)"){
        SECTION("Size"){
            vector<Buffer> v{};
            REQUIRE(v.empty());
            v.push_back(Buffer{1});
            REQUIRE(v.size() == 1);
        }SECTION("Capacity"){
            vector<Buffer> v{};
            REQUIRE(v.capacity() == 0);
            v.push_back(Buffer{1});
            REQUIRE(v.capacity() == 1);
            v.push_back(Buffer{1});
            v.push_back(Buffer{1});
            REQUIRE(v.capacity() == 4);
            v.push_back(Buffer{1});
            v.push_back(Buffer{1});
            REQUIRE(v.capacity() == 8);
            v.reserve(20);
            REQUIRE(v.capacity() == 20);
        }SECTION("Empty"){
            vector<Buffer> v{};
            REQUIRE(v.empty());
            v.push_back(Buffer{1});
            REQUIRE(!v.empty());
        }SECTION("Reserve"){
            vector<Buffer> v{};
            v.reserve(10);
            v.reserve(20);
            v.reserve(10);
            REQUIRE(v.empty());
            vector<Buffer> v2{10};
            v2.reserve(20);
            REQUIRE(v2.size() == 10);
            vector<Buffer> v3{10, Buffer{1}};
            v3.reserve(20);
            REQUIRE(v3.size() == 10);
        }SECTION("At - const"){
            vector<Buffer> v{10, Buffer{1}};
            Buffer b{1};
            REQUIRE(v.at(5) == b);
            REQUIRE_THROWS(v.at(20) == b);
            REQUIRE_THROWS(v.at(-2) == b);
        }SECTION("At - non const"){
            vector<Buffer> v{10, Buffer{1}};
            Buffer b{5};
            v.at(5) = b;
            REQUIRE_THROWS(v.at(20) = b);
            REQUIRE_THROWS(v.at(-2) = b);
        }SECTION("Front - const"){
            vector<Buffer> v{10, Buffer{1}};
            Buffer b{1};
            Buffer c{5};
            REQUIRE(v.front() == b);
            v[0] = c;
            REQUIRE(v.front() == c);
        }SECTION("Front - non const"){
            vector<Buffer> v{10, Buffer{1}};
            Buffer b{1};
            Buffer c{5};
            REQUIRE(v.front() == b);
            v.front() = c;
            REQUIRE(v[0] == c);
        }SECTION("Back - const"){
            vector<Buffer> v{10, Buffer{1}};
            Buffer b{1};
            Buffer c{5};
            REQUIRE(v.back() == b);
            v.push_back(Buffer{5});
            REQUIRE(v.back() == c);
        }SECTION("Back - non const"){
            vector<Buffer> v{10, Buffer{1}};
            Buffer c{5};
            v.back() = c;
            REQUIRE(v[9] == c);
        }SECTION("Data - const"){
            vector<Buffer> v{10, Buffer{1}};
            Buffer *data = v.data();
        }SECTION("Push back - Copy"){
            vector<Buffer> v{};
            Buffer b{1};
            v.push_back(b);
            REQUIRE(v.size() == 1);
            REQUIRE(v.back() == b);
        }SECTION("Push back - Move"){
            vector<Buffer> v{};
            Buffer b{1};
            v.push_back(Buffer{1});
            REQUIRE(v.size() == 1);
            REQUIRE(v.back() == b);
        }SECTION("Emplace back"){
            vector<Buffer> v{};
            Buffer b{1};
            v.emplace_back(1);
            REQUIRE(v.back() == b);
        }SECTION("Clear"){
            vector<Buffer> v{};
            v.clear();
            vector<Buffer> v2{10, Buffer{1}};
            Buffer c{5};
            Buffer d{10};
            v2.clear();
            REQUIRE(v2.empty());
            v2.push_back(d);
            REQUIRE(v2.size() == 1);
            REQUIRE(v2[0] == d);
        }
    }

    SECTION("Methods (with iterators)"){
        using iterator = VectorIterator<Buffer, false>;
        using const_iterator = VectorIterator<Buffer, true>;
        SECTION("Begin and end"){
            vector<Buffer> v{Buffer{1}, Buffer{5}};
            Buffer b{1};
            Buffer c{5};
            iterator it = v.begin();
            REQUIRE(*it == b);
            it = v.end();
            REQUIRE(*(--it) == c);
            const_iterator cIt = v.cbegin();
            REQUIRE(*cIt == b);
            cIt = v.cend();
            REQUIRE(*(--cIt) == c);
        }SECTION("Find"){
            vector<Buffer> v{Buffer{1}, Buffer{2}, Buffer{3}, Buffer{4}, Buffer{5}};
            Buffer b4{4};
            iterator it = std::find(v.begin(), v.end(), b4);
            if (it != v.end()){
                REQUIRE(*it == b4);
            }
            Buffer b6{6};
            it = std::find(v.begin(), v.end(), b6);
            REQUIRE(it == v.end());
        }SECTION("Erase - one"){
            vector<Buffer> v{Buffer{1}, Buffer{2}, Buffer{3}, Buffer{4}, Buffer{5}};
            Buffer b4{4};
            iterator it = std::find(v.begin(), v.end(), b4);
            v.erase(it);
            REQUIRE(v.size() == 4);
            REQUIRE(v[2] == Buffer{3});
            REQUIRE(v[3] == Buffer{5});
            Buffer b6{6};
            it = std::find(v.begin(), v.end(), b6);
            REQUIRE_NOTHROW(v.erase(it));
        }SECTION("Erase - range"){
            vector<Buffer> v{Buffer{1}, Buffer{2}, Buffer{3}, Buffer{4}, Buffer{5}};
            v.erase(v.begin(), v.begin() + 2);
            REQUIRE(v.size() == 3);
            REQUIRE(v[0] == Buffer{3});
            REQUIRE(v[1] == Buffer{4});
            REQUIRE(v[2] == Buffer{5});
            vector<Buffer> v2{Buffer{1}, Buffer{2}, Buffer{3}, Buffer{4}, Buffer{5}};
            v2.erase(v2.end() - 1, v2.end());
            REQUIRE(v2.size() == 4);
            REQUIRE(v2.back() == Buffer{4});
            vector<Buffer> v3{Buffer{1}, Buffer{2}, Buffer{3}, Buffer{4}, Buffer{5}};
            v3.erase(v3.begin() + 1, v3.end() - 1);
            REQUIRE(v3.size() == 2);
            REQUIRE(v3[0] == Buffer{1});
            REQUIRE(v3[1] == Buffer{5});
        }
    }
}

TEST_CASE("Complex value (shared_ptr<string>)"){
    SECTION("Constructors"){
        SECTION("Default"){
            vector<shared_ptr<string>> v{};
        }SECTION("By _capacity"){
            vector<shared_ptr<string>> v{10};
            REQUIRE(!v.empty());
        }SECTION("By size and value"){
            vector<shared_ptr<string>> v{10, make_shared<string>("123")};
            REQUIRE(v.size() == 10);
        }SECTION("By init list"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4")};
            REQUIRE(v.size() == 4);
        }SECTION("Copy"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4")};
            vector<shared_ptr<string>> v2{v};
            REQUIRE(v2.size() == v.size());
            REQUIRE(v2[1] == v[1]);
        }SECTION("Move"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2")};
            REQUIRE(v.size() == 2);
        }
    }

    SECTION("Operators"){
        SECTION("= - Copy"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4")};
            vector<shared_ptr<string>> v2 = v;
            REQUIRE(v2.size() == v.size());
            REQUIRE(v2[1] == v[1]);
        }SECTION("= - Move"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4")};
            int tempSize = v.size();
            shared_ptr<string> tempElem = v[0];
            vector<shared_ptr<string>> v2 = std::move(v);
            REQUIRE(v2.size() == tempSize);
            REQUIRE(v2[0] == tempElem);
        }SECTION("= - Init list"){
            vector<shared_ptr<string>> v{};
            v = vector<shared_ptr<string>>{make_shared<string>("1"), make_shared<string>("2")};
            REQUIRE(v.size() == 2);
            vector<shared_ptr<string>> v2{10, make_shared<string>("1")};
            v2 = vector<shared_ptr<string>>{make_shared<string>("1"), make_shared<string>("2")};
            REQUIRE(v.size() == 2);
        }SECTION("[] - const"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4")};
            REQUIRE(*(v[0]) == "1");
        }SECTION("[] - non const"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4")};
            v[0] = make_shared<string>("4");
        }
    }

    SECTION("Methods (without iterators)"){
        SECTION("Size"){
            vector<shared_ptr<string>> v{};
            REQUIRE(v.empty());
            v.push_back(make_shared<string>("1"));
            REQUIRE(v.size() == 1);
        }SECTION("Capacity"){
            vector<shared_ptr<string>> v{};
            REQUIRE(v.capacity() == 0);
            v.push_back(make_shared<string>("1"));
            REQUIRE(v.capacity() == 1);
            v.push_back(make_shared<string>("1"));
            v.push_back(make_shared<string>("1"));
            REQUIRE(v.capacity() == 4);
            v.push_back(make_shared<string>("1"));
            v.push_back(make_shared<string>("1"));
            REQUIRE(v.capacity() == 8);
            v.reserve(20);
            REQUIRE(v.capacity() == 20);
        }SECTION("Empty"){
            vector<shared_ptr<string>> v{};
            REQUIRE(v.empty());
            v.push_back(make_shared<string>("1"));
            REQUIRE(!v.empty());
        }SECTION("Reserve"){
            vector<shared_ptr<string>> v{};
            v.reserve(10);
            v.reserve(20);
            v.reserve(10);
            REQUIRE(v.empty());
            vector<shared_ptr<string>> v2{10};
            v2.reserve(20);
            REQUIRE(v2.size() == 10);
            vector<shared_ptr<string>> v3{10, make_shared<string>("1")};
            v3.reserve(20);
            REQUIRE(v3.size() == 10);
        }SECTION("At - const"){
            vector<shared_ptr<string>> v{10, make_shared<string>("1")};
            REQUIRE(*(v.at(5)) == "1");
            REQUIRE_THROWS(*v.at(20) == "1");
            REQUIRE_THROWS(*v.at(-2) == "1");
        }SECTION("At - non const"){
            vector<shared_ptr<string>> v{10, make_shared<string>("1")};
            shared_ptr<string> b = make_shared<string>("2");
            v.at(5) = b;
            REQUIRE_THROWS(v.at(20) = b);
            REQUIRE_THROWS(v.at(-2) = b);
        }SECTION("Front - const"){
            vector<shared_ptr<string>> v{10, make_shared<string>("1")};
            REQUIRE(*v.front() == "1");
            v[0] = make_shared<string>("2");
            REQUIRE(*v.front() == "2");
        }SECTION("Front - non const"){
            vector<shared_ptr<string>> v{10, make_shared<string>("1")};
            REQUIRE(*v.front() == "1");
            v.front() = make_shared<string>("2");
            REQUIRE(*v[0] == "2");
        }SECTION("Back - const"){
            vector<shared_ptr<string>> v{10, make_shared<string>("1")};
            REQUIRE(*v.back() == "1");
            v.push_back(make_shared<string>("2"));
            REQUIRE(*v.back() == "2");
        }SECTION("Back - non const"){
            vector<shared_ptr<string>> v{10, make_shared<string>("1")};
            shared_ptr<string> c = make_shared<string>("2");
            v.back() = c;
            REQUIRE(v[9] == c);
        }SECTION("Data - const"){
            vector<shared_ptr<string>> v{10, make_shared<string>("1")};
            shared_ptr<string> *data = v.data();
        }SECTION("Push back - Copy"){
            vector<shared_ptr<string>> v{};
            shared_ptr<string> b = make_shared<string>("1");
            v.push_back(b);
            REQUIRE(v.size() == 1);
            REQUIRE(v.back() == b);
        }SECTION("Push back - Move"){
            vector<shared_ptr<string>> v{};
            v.push_back(make_shared<string>("1"));
            REQUIRE(v.size() == 1);
            REQUIRE(*v.back() == "1");
        }SECTION("Clear"){
            vector<shared_ptr<string>> v{};
            v.clear();
            vector<shared_ptr<string>> v2{10, make_shared<string>("1")};
            v2.clear();
            REQUIRE(v2.empty());
            v2.push_back(make_shared<string>("2"));
            REQUIRE(v2.size() == 1);
            REQUIRE(*v2[0] == "2");
        }
    }

    SECTION("Methods (with iterators)"){
        using iterator = VectorIterator<shared_ptr<string>, false>;
        using const_iterator = VectorIterator<shared_ptr<string>, true>;
        SECTION("Begin and end"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2")};
            iterator it = v.begin();
            REQUIRE(**it == "1");
            it = v.end();
            REQUIRE(**(--it) == "2");
            const_iterator cIt = v.cbegin();
            REQUIRE(**cIt == "1");
            cIt = v.cend();
            REQUIRE(**(--cIt) == "2");
        }SECTION("Find"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4"), make_shared<string>("5")};
            shared_ptr<string> b4 = v[3];
            iterator it = std::find(v.begin(), v.end(), b4);
            if (it != v.end()){
                REQUIRE(*it == b4);
            }
            shared_ptr<string> b6 = make_shared<string>("6");
            it = std::find(v.begin(), v.end(), b6);
            REQUIRE(it == v.end());
        }SECTION("Erase - one"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4"), make_shared<string>("5")};
            shared_ptr<string> b4 = v[3];
            iterator it = std::find(v.begin(), v.end(), b4);
            v.erase(it);
            REQUIRE(v.size() == 4);
            REQUIRE(*v[2] == "3");
            REQUIRE(*v[3] == "5");
            shared_ptr<string> b6 = make_shared<string>("6");
            it = std::find(v.begin(), v.end(), b6);
            REQUIRE_NOTHROW(v.erase(it));
        }SECTION("Erase - range"){
            vector<shared_ptr<string>> v{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4"), make_shared<string>("5")};
            v.erase(v.begin(), v.begin() + 2);
            REQUIRE(v.size() == 3);
            REQUIRE(*v[0] == "3");
            REQUIRE(*v[1] == "4");
            REQUIRE(*v[2] == "5");
            vector<shared_ptr<string>> v2{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4"), make_shared<string>("5")};
            v2.erase(v2.end() - 1, v2.end());
            REQUIRE(v2.size() == 4);
            REQUIRE(*v2.back() == "4");
            vector<shared_ptr<string>> v3{make_shared<string>("1"), make_shared<string>("2"), make_shared<string>("3"), make_shared<string>("4"), make_shared<string>("5")};
            v3.erase(v3.begin() + 1, v3.end() - 1);
            REQUIRE(v3.size() == 2);
            REQUIRE(*v3[0] == "1");
            REQUIRE(*v3[1] == "5");
        }
    }
}