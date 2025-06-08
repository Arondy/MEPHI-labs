/*
Вовочку взял на работу Павел Дуров. Он будет участвовать в разработке нового мессенджера – «Телега».
Вовочке необходимо разработать библиотеку для работы нового мессенджера. Мессенджер представляет собой набор чатов.
Чаты характеризуются описателем контакта собеседника (уникальный id, логин, номер телефона) и списком сообщений. Сообщения могут быть трех типов: текстовое сообщение (содержит id отправителя, время отправки, статус (прочитано или нет), текст сообщения), фотографии (содержит id отправителя, время отправки, статус (прочитано или нет), числовой идентификатор файла на сервере), аудиосообщения (содержит id отправителя, время отправки, числовой идентификатор файла на сервере,статус (прочитано или нет), текст сообщения).
Для сообщений определена функция void print(ostream &), которая для текстовых сообщений выводит текст сообщение, для изображений — ссылку вида :
www.﻿telega.﻿org/img/<id файла>, для аудио сообщений: ссылку вида : www.﻿telega.﻿org/audio/<id файла> и текст сообщения.
Хранение чатов в мессенджере необходимо реализовать, используя один из стандартных контейнерных классов. Желательно, чтобы чаты были отсортированы по логину собеседника.
Помогите Вовочке не потерять работу!
1. Разработайте диаграмму классов в виде UML диаграммы (на экзаменационном листе) (6 баллов max)
2. Реализуйте объявление всех классов. Классы должны включать все необходимые методы, конструкторы, деструкторы, операторы присваивания. (6 баллов max)
3. Выполните реализацию всех методов классов «мессенджер» и «чат».
Для класса «мессенджер» необходимо перегрузить оператор вывода в поток << для вывода списка чатов в отсортированном по логину порядке и оператор индексации [ ] ( в двух вариантах!) для получения чата по логину. Для класса «чат» необходимо реализовать функции добавления нового сообщения и вывода всех непрочитанных сообщений с изменением их статуса. (8 баллов max)
 */

/*
   Мессенджер:
       Чаты - Собеседник: id, login, phone
            - Список сообщений: id отправителя, time, isRead и text/id
void print(ostream &)
 */

#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <map>

using std::string, std::vector, std::map;
using std::cout, std::cin, std::endl;

class User {
private:
    int id;
    string login;
    string phone;
public:
    User() : id{0}{};
    User(int id, string login, string phone) : id{id}, login{std::move(login)}, phone{std::move(phone)}{};
    ~User() = default;
    int getId() const{
        return id;
    }
    string getLogin() const{
        return login;
    }
    string getPhone() const{
        return phone;
    }
    void setId(int newId){
        id = newId;
    }
    void setLogin(string newLogin){
        login = std::move(newLogin);
    }
    void setPhone(string newPhone){
        phone = std::move(newPhone);
    }
};

class Message {
protected:
    int userId;
    string time;
    bool isRead;
    string type;
public:
    Message() : userId{0}, isRead{false}{};
    Message(int userId, string time, bool isRead, string type) : userId{userId}, time{std::move(time)}, isRead{isRead},
                                                                 type{type}{};
    ~Message() = default;
    int getUserId() const{
        return userId;
    }
    string getTime() const{
        return time;
    }
    bool getIsRead() const{
        return isRead;
    }
    string getType() const{
        return type;
    }
    void setUserId(int newId){
        userId = newId;
    }
    void setTime(string newTime){
        time = std::move(newTime);
    }
    void markAsRead(){
        isRead = true;
    }
    void setType(string newType){
        type = std::move(newType);
    }
    virtual void print(std::ostream &){};
};

class TextMessage : public Message {
private:
    string text;
public:
    TextMessage() : Message(){
        type = "text";
    }
    TextMessage(int userId, string time, bool isRead, string type) : Message(userId, time, isRead, type){};
    ~TextMessage() = default;
    string getText() const{
        return text;
    }
    void setText(string newText){
        text = std::move(newText);
    }
    void print(std::ostream &os) override{
        os << text << endl;
    }
};

class PhotoMessage : public Message {
private:
    int fileId;
public:
    PhotoMessage() : fileId{0}, Message(){
        type = "photo";
    };
    PhotoMessage(int userId, string time, bool isRead, int fileId, string type) : fileId{fileId},
                                                                                  Message(userId, time, isRead, type){};
    ~PhotoMessage() = default;
    int getFileId() const{
        return fileId;
    }
    void setFileId(int newFileId){
        fileId = newFileId;
    }
    void print(std::ostream &os) override{
        os << "www.telega.org/img/" << fileId << endl;
    }
};

class AudioMessage : public Message {
private:
    string text;
    int fileId;
public:
    AudioMessage() : fileId{0}, Message(){
        type = "audio";
    };
    AudioMessage(int userId, string time, bool isRead, string text, int fileId, string type) : text{text},
                                                                                               fileId{fileId},
                                                                                               Message(userId, time,
                                                                                                       isRead, type){};
    ~AudioMessage() = default;
    string getText() const{
        return text;
    }
    int getFileId() const{
        return fileId;
    }
    void setText(string newText){
        text = std::move(newText);
    }
    void setFileId(int newFileId){
        fileId = newFileId;
    }
    void print(std::ostream &os) override{
        os << "www.telega.org/audio/" << fileId << endl;
        os << text << endl;
    }
};

class Chat {
private:
    User user;
    vector<Message> messages;
public:
    Chat() = default;
    Chat(User user, vector<Message> messages) : user{user}, messages{messages}{};
    Chat(const Chat &other) = default;
    Chat(Chat &&other) = default;
    ~Chat() = default;
    User getUser() const{
        return user;
    }
    vector<Message> getMessages() const{
        return messages;
    }
    void setUser(User newUser){
        user = newUser;
    }
    void setMessages(vector<Message> newMessages){
        messages = newMessages;
    }
    void addMessage(Message message){
        messages.push_back(message);
    }
    void printEveryUnread(std::ostream &os){
        for (int i = messages.size() - 1; i >= 0; i--){
            if (messages[i].getIsRead()){
                break;
            }
            messages[i].print(os);
            messages[i].markAsRead();
        }
    }
};

class Messenger {
private:
    map<string, Chat> chats;
public:
    Messenger() = default;
    Messenger(map<string, Chat> chats) : chats{chats}{};
    ~Messenger() = default;
    map<string, Chat> getChats() const{
        return chats;
    }
    void setChats(map<string, Chat> newChats){
        chats = newChats;
    }
    Chat &operator[](string login){
        return chats[login];
    }
    const Chat &operator[](string login) const{
        return chats.at(login);
    }
    vector<string> get_logins(){
        vector<string> logins;
        for (std::pair<string, Chat> pair: chats){
            logins.push_back(pair.first);
        }
        return logins;
    }
    friend std::ostream operator<<(std::ostream &os, Messenger messenger){
        vector<string> logins = messenger.get_logins();
        for (string login: logins){
            os << login << endl;
            messenger.getChats()[login].printEveryUnread(os);
        }
    }
};

int main(){
    return 1;
}