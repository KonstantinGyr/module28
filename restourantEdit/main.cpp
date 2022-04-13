#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <pthread.h>


std::vector<std::string>readyDish;
std::mutex accessKitchen;

void courier(){
    std::string dish;
    for(int i=0;i<10;i++){
        std::this_thread::sleep_for(std::chrono::seconds(20));
        accessKitchen.lock();
        dish=readyDish.at(i);
        accessKitchen.unlock();
        std::cout<<"Order "<<dish<<" will be delivered\n";
    }
}

void kitchen(){
    std::string dish;
    for(int i=0;i<10;i++){
        accessKitchen.lock();
        dish=readyDish.at(i);
        accessKitchen.unlock();
        std::cout<<"Order "<<dish<<" in queue on a kitchen\n";
        std::this_thread::sleep_for(std::chrono::seconds(rand()%11+5));
        std::cout<<dish<<" ready\n";
    }
}

void inOrder(){
    std::string dishes[5]={"pizza","soup","steak","salad","sushi"};
    std::string dish;
    for(int i=0;i<10;i++){
        dish=dishes[rand()%5];
        std::cout<< "Received an order: " << dish << std::endl;
        accessKitchen.lock();
        readyDish.push_back(dish);
        accessKitchen.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(rand()%6+5));
    }
}

int main() {
    std::thread input (inOrder);
    std::thread order(kitchen);
    std::thread delivery(courier);
    input.join();
    order.join();
    delivery.join();
    std::cout<<"All orders have been delivered";
    return 0;
}
