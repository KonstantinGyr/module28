#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::vector<std::string>readyDish;
std::mutex accessKitchen;

void kitchen(std::string dish){
    std::cout<<"Order "<<dish<<" receipt on a kitchen\n";
    std::this_thread::sleep_for(std::chrono::seconds(rand()%11+5));
    accessKitchen.lock();
    std::cout<<dish<<" ready\n";
    readyDish.push_back(dish);
    accessKitchen.unlock();
}

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

std::vector<std::string>orders;
std::mutex accessOrders;

void receipt() {
    std::string dish;
    std::string dishes[5]={"pizza","soup","steak","salad","sushi"};
    accessOrders.lock();
    dish = dishes[rand() % 5];
    std::cout << "Received an order: " << dish << std::endl;
    orders.push_back(dish);
    accessOrders.unlock();
    for(int i=0;i<10;i++) {
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 2));
        accessOrders.lock();
        dish = dishes[rand() % 5];
        std::cout << "Received an order: " << dish << std::endl;
        orders.push_back(dish);
        accessOrders.unlock();
    }
}

int main() {
    std::thread inOrder(receipt);
    std::thread delivery(courier);
    std::vector<std::string>::iterator it;
    for(int i=0;i<10;i++) {
        accessOrders.lock();
        if(!orders.empty()) {
            it=orders.begin();
            std::thread order(kitchen, orders.front());
            orders.erase(it);
            order.join();
        }
        else i--;
        accessOrders.unlock();
    }
    delivery.join();
    inOrder.join();
    return 0;
}
