#include <iostream>
#include <thread>
#include <map>
#include <mutex>
#include <vector>

struct Result{
    std::string name;
    int time;
};

Result result;
std::vector<Result>resultList;
std::mutex accessList;

void swimmerVisual(int speed,std::string name){
    std::cout<<"Swimmer: "<<name<<std::endl;
    std::cout<<"swimming \n";
    int distance=0;
    int time=0;
    while(distance<100){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        distance+=speed;
        time++;
        std::cout<<distance<<std::endl;
    }
    std::cout<<"finish! "<<std::endl;
    accessList.lock();
    result.name=name;
    result.time=time;
    resultList.push_back(result);
    accessList.unlock();
}

void swimmer(int speed,std::string name){
    int distance=0;
    int time=0;
    while(distance<100) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        distance += speed;
        time++;
    }
    accessList.lock();
    result.name=name;
    result.time=time;
    resultList.push_back(result);
    accessList.unlock();
}

int main() {
    std::string name;
    int speed;
    std::map<std::string,int>inputMap;
    for(int i=0;i<6;i++) {
        std::cout << "Enter name swimmer: ";
        std::cin >> name;
        std::cout << "Enter speed swimmer: ";
        std::cin >> speed;
        inputMap[name]={speed};
    }
    auto it=inputMap.begin();
    std::thread trek1(swimmerVisual, it->second, it->first);
    it++;
    std::thread trek2(swimmer,it->second, it->first);
    it++;
    std::thread trek3(swimmer,it->second, it->first);
    it++;
    std::thread trek4(swimmer,it->second, it->first);
    it++;
    std::thread trek5(swimmer,it->second, it->first);
    it++;
    std::thread trek6(swimmer,it->second, it->first);
    trek1.join();
    trek2.join();
    trek3.join();
    trek4.join();
    trek5.join();
    trek6.join();
    accessList.lock();
    for(const auto& item:resultList){
        std::cout<<item.name<<" time "<<item.time<<"sec."<<std::endl;
    }
    accessList.unlock();
    return 0;
}