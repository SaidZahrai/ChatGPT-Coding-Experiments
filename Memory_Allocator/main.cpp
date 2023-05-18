// #include "MemoryManager.h"

// int main() {
//     MemoryManager memoryManager(1024);

//     auto [ptr1, name1, type1, err1] = memoryManager.allocate<int>("var1");
//     if (err1 == 0) {
//         std::cout << "Allocated new variable: " << name1 << std::endl;
//     }

//     auto [ptr2, name2, type2, err2] = memoryManager.allocate<int>("var1");
//     if (err2 == 1) {
//         std::cout << "Returned existing variable: " << name2 << std::endl;
//     }

//     std::shared_lock<std::shared_timed_mutex> lock;
//     auto var1_ptr = memoryManager.access<int>("var1", lock);
//     *var1_ptr = 42;
//     std::cout << "Value of var1: " << *var1_ptr << std::endl;

//     if (memoryManager.deallocate<int>("var1")) {
//         std::cout << "Deallocated variable: var1" << std::endl;
//     }

//     return 0;
// }

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include "MemoryManager.h"


template <typename T>
constexpr std::size_t CalculateSize() {
    return sizeof(T);
}

struct Data1{
    int counter = 0;
    float x = 0.123;
} data1;

struct Data2{
    long counter = 0;
    double x = 0.123;
} data2;

void worker(MemoryManager& memoryManager, int increment, const std::string& thread_name) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(10, 300);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));

        std::shared_lock<std::shared_timed_mutex> lock;
        auto var1_ptr = memoryManager.access<Data1>("var1", lock);
        
        if (var1_ptr->counter > 1000) {
            break;
        }

        var1_ptr->counter += increment;
        std::cout << thread_name << " updated var1 to: " << var1_ptr->counter << std::endl;
    }
}

int main() {
    constexpr std::size_t BufferSize=CalculateSize<Data1>()*10+CalculateSize<Data2>()*100;

    MemoryManager memoryManager(BufferSize);

    auto [ptr1, name1, type1, err1] = memoryManager.allocate<Data1>("var1");
    if (err1 == 0) {
        std::cout << "Allocated new variable: " << name1 << std::endl;
    }

    std::shared_lock<std::shared_timed_mutex> lock;
    auto var1_ptr = memoryManager.access<Data1>("var1", lock);
    var1_ptr->counter = 0;

    std::thread t1(worker, std::ref(memoryManager), 1, "Thread 1");
    std::thread t2(worker, std::ref(memoryManager), 100, "Thread 2");

    t1.join();
    t2.join();

    std::cout << "Final value of var1: " << var1_ptr->counter << std::endl;

    return 0;
}
