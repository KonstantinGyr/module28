#include <iostream>
#include <thread>
#include <mutex>

void station(char name){
    std::cout<<"Train "<<name<<" arrived on station\n";
}
std::mutex train_on_station;

void train(char name,int time){
    std::string command;
    std::this_thread::sleep_for(std::chrono::seconds(time));
    train_on_station.lock();
    station(name);
    do {
        std::cout <<"Train "<<name<<" waiting depart.";
        std::cin >> command;
    }
    while(command!="depart");
        train_on_station.unlock();
        std::cout<<"Train "<<name<<" out of station\n";
}

int main() {
    int duration1;
    int duration2;
    int duration3;
    std::cout<<"Enter way time A train: ";
    std::cin>>duration1;
    std::cout<<"Enter way time B train: ";
    std::cin>>duration2;
    std::cout<<"Enter way time C train: ";
    std::cin>>duration3;
    std::thread way1(train,'A',duration1);
    std::thread way2(train,'B',duration2);
    std::thread way3(train,'C',duration3);
    way1.join();
    way2.join();
    way3.join();
    return 0;
}
