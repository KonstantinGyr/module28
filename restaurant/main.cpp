#include <iostream>
#include <thread>
#include <vector>
#include <mutex>


std::vector<std::string>readyDish;
std::mutex accessKitchen;

void courier(){
    std::vector<std::string>::iterator it;
    for(int i=0;i<10;i++){
        std::this_thread::sleep_for(std::chrono::seconds(20));
        accessKitchen.lock();
        if(!readyDish.empty()) {
            it=readyDish.begin();
            std::cout<<"Order "<<readyDish.front()<<" will be delivered\n";
            readyDish.erase(it);
        }
        else i--;
        accessKitchen.unlock();
    }
    std::cout<<"All orders have been delivered";
}

void kitchen(std::string dish){
    std::cout<<"Order "<<dish<<" in queue on a kitchen\n";
    std::this_thread::sleep_for(std::chrono::seconds(rand()%11+5));
    accessKitchen.lock();
    readyDish.push_back(dish);
    accessKitchen.unlock();
    std::cout<<dish<<" ready\n";
}

void inOrder(std::string in_dish){
    std::cout<< "Received an order: " << in_dish << std::endl;
    std::thread order(kitchen,in_dish);
    order.detach();
}

int main() {
    std::string dish;
    std::string dishes[5]={"pizza","soup","steak","salad","sushi"};
    std::thread delivery(courier);
    for(int i=0;i<20;i++) {
        dish = dishes[rand() % 5];
        inOrder(dish);
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 6 + 5));
    }
    delivery.join();

    return 0;
}
