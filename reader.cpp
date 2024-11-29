#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>

struct SharedData {
    bool ready;
    bool reader_started;
    int data;
};

void readData(SharedData* shared_data) {
    if (shared_data->ready) {
        std::cout << "Reader received data: " << shared_data->data << std::endl;
        shared_data->ready = false; 
    }
}

void testReadData() {
    SharedData data = {true, false, 42};
    readData(&data);
    assert(data.ready == false);
    std::cout << "Test passed!" << std::endl;
}

int main() {
    testReadData();
    int shmid = shmget(123, sizeof(SharedData), 0666); 
    SharedData* shared_data = (SharedData*)shmat(shmid, nullptr, 0); 

    shared_data->reader_started = true;

    while (true) {
        readData(shared_data);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }

    shmdt(shared_data); 
    return 0;
}
