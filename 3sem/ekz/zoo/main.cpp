#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using std::string, std::vector, std::map, std::pair, std::cout, std::cin, std::endl;

enum Type {
    open = 1, closed = 2, water = 3, alien = 4, alienWater = 5
};

class Animal {
private:
    string name;
    int age;
    int danger;
public:
    Animal(const string &name, int age, int danger) : name(name), age(age), danger(danger){}
    Animal(const Animal &rhs) = default;
    Animal(Animal &&rhs) noexcept = default;
    Animal &operator=(const Animal &rhs) = default;
    Animal &operator=(Animal &&rhs) noexcept = default;
    ~Animal() = default;
    const string &getName() const{
        return name;
    }
    void setName(const string &name){
        Animal::name = name;
    }
    int getAge() const{
        return age;
    }
    void setAge(int age){
        Animal::age = age;
    }
    int getDanger() const{
        return danger;
    }
    void setDanger(int danger){
        Animal::danger = danger;
    }
    bool operator==(const Animal &rhs) const = default;
    bool operator!=(const Animal &rhs) const{
        return !(rhs == *this);
    }
    bool operator<(const Animal &rhs) const{
        return danger < rhs.danger;
    }
};

class Cage {
protected:
    int id;
    vector<Animal> animals;
    float temp;
    Type type;
public:
    Cage(int id, const vector<Animal> &animals, float temp, Type type) : id(id), animals(animals), temp(temp),
                                                                         type(type){}
    virtual ~Cage() = default;
    int getId() const{
        return id;
    }
    void setId(int id){
        Cage::id = id;
    }
    const vector<Animal> &getAnimals() const{
        return animals;
    }
    void setAnimals(const vector<Animal> &animals){
        Cage::animals = animals;
    }
    float getTemp() const{
        return temp;
    }
    void setTemp(float temp){
        Cage::temp = temp;
    }
    Type getType() const{
        return type;
    }
    void setType(Type type){
        Cage::type = type;
    }
    void addAnimal(Animal &animal){
        animals.push_back(animal);
        std::push_heap(animals.begin(), animals.end());
    }
    void removeAnimal(const Animal &animal){
        auto it = std::remove(animals.begin(), animals.end(), animal);
        animals.erase(it, animals.end());
        std::make_heap(animals.begin(), animals.end());
    }
    int getDanger(){
        if (!animals.empty()){
            return animals[0].getDanger() * type;
        }
    }
};

class OpenCage : public Cage {
public:
    OpenCage(int id, const vector<Animal> &animals, float temp) : Cage{id, animals, temp, open}{}
    ~OpenCage() override = default;
    void operator<<(string str);
};

class AlienCage;
class AlienWaterCage;

class ClosedCage : public Cage {
public:
    ClosedCage(int id, const vector<Animal> &animals, float temp) : Cage{id, animals, temp, closed}{}
    ~ClosedCage() override = default;
    void switchLight() const{
        cout << "You switched the light." << endl;
    }
    AlienCage operator~();
};

class WaterCage : virtual public ClosedCage {
protected:
    float waterLevel;
    map<string, float> wMap;
public:
    WaterCage(int id, const vector<Animal> &animals, float temp, float waterLevel, const map<string, float> &wMap) :
            ClosedCage(id, animals, temp), waterLevel(waterLevel), wMap(wMap){ type = water; }
    ~WaterCage() override = default;
    float getWaterLevel() const{
        return waterLevel;
    }
    void setWaterLevel(float waterLevel){
        WaterCage::waterLevel = waterLevel;
    }
    const map<string, float> &getWMap() const{
        return wMap;
    }
    void setWMap(const map<string, float> &wMap){
        WaterCage::wMap = wMap;
    }
    WaterCage &operator+=(const pair<string, float> &pair){
        wMap[pair.first] = pair.second;
        return *this;
    }
    AlienWaterCage operator~();
};

class AlienCage : virtual public ClosedCage {
protected:
    map<string, float> aMap;
public:
    AlienCage(int id, const vector<Animal> &animals, float temp, const map<string, float> &aMap) :
            ClosedCage(id, animals, temp), aMap(aMap){ type = alien; }
    ~AlienCage() override = default;
    const map<string, float> &getAMap() const{
        return aMap;
    }
    void setAMap(const map<string, float> &aMap){
        AlienCage::aMap = aMap;
    }
    AlienCage &operator+=(const pair<string, float> &pair){
        aMap[pair.first] = pair.second;
        return *this;
    }
    ClosedCage operator~() {
        return ClosedCage{id, animals, temp};
    }
};

class AlienWaterCage : public WaterCage, public AlienCage {
public:
    AlienWaterCage(int id, const vector<Animal> &animals, float temp, float waterLevel,
                   const map<string, float> &wMap, const map<string, float> &aMap) :
            WaterCage(id, animals, temp, waterLevel, wMap), AlienCage(id, animals, temp, aMap),
            ClosedCage(id, animals, temp){ type = alienWater; }
    ~AlienWaterCage() override = default;
    WaterCage operator~() {
        return WaterCage{id, animals, temp, 0, {}};
    }
};

AlienCage ClosedCage::operator~(){
    return AlienCage{id, animals, temp, {}};
}

AlienWaterCage WaterCage::operator~(){
    return AlienWaterCage{id, animals, temp, waterLevel, wMap, {}};
}

int main(){
    Animal a{"1", 3, 5};
    vector<Animal> an{a};
    OpenCage c2{0, an, 45};
    ClosedCage c{0, an, 45};
//    c2 << "sq";
    AlienWaterCage awc{0, an, 1, 1, {{"2", 1.0}}, {{"3", 1.0}}};
    return 1;
}