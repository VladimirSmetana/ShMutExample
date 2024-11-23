#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <iostream>
#include <chrono>
#include <thread>

struct SharedData {
    bool ready;
    bool reader_started;
    int data;
};

int main() {
    int shmid = shmget(123, sizeof(SharedData), 0666 | IPC_CREAT); // Создание сегмента общей памяти
    SharedData* shared_data = (SharedData*)shmat(shmid, nullptr, 0); // Присоединение сегмента общей памяти

    shared_data->ready = false;
    shared_data->reader_started = false;
    shared_data->data = 0;

    // Ждем, пока reader не запустится
    while (!shared_data->reader_started) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    while (true) {
        shared_data->data++;
        shared_data->ready = true;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Имитация работы
    }

    shmdt(shared_data); // Отключение сегмента общей памяти
    shmctl(shmid, IPC_RMID, nullptr); // Удаление сегмента общей памяти

    return 0;
}