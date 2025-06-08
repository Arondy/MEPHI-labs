#ifndef BUFFER_H
#define BUFFER_H

using std::string;

class Buffer {
private:
    string *buffer = nullptr;
    int size = 0;
public:
    Buffer() noexcept = default;
    explicit Buffer(int size) : size{size}, buffer{new string[size]}{};
    Buffer(const Buffer &second){
        size = second.size;
        buffer = new string[size];
        for (int i = 0; i < size; i++){
            buffer[i] = second.buffer[i];
        }
    };
    Buffer(Buffer &&other) noexcept: size{other.size}, buffer{other.buffer}{ other.buffer = nullptr; };
    ~Buffer(){ delete[] buffer; };

    Buffer &operator=(const Buffer &second){
        if (this != &second){
            Buffer temp(second);
            std::swap(size, temp.size);
            std::swap(buffer, temp.buffer);
        }
        return *this;
    };

    Buffer &operator=(Buffer &&second) noexcept{
        if (this != &second){
            size = second.size;
            std::swap(buffer, second.buffer);
        }
        return *this;
    }

    bool operator==(const Buffer &second) const{
        if (size != second.size){
            return false;
        }
        for (int i = 0; i < size; ++i){
            if (buffer[i] != second.buffer[i]){
                return false;
            }
        }
        return true;
    }
};

#endif