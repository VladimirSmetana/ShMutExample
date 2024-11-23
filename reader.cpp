#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <iostream>
#include <chrono>
#include <thread>

struct SharedData {
    bool ready;
    bool reader_started;
    int data;
};

int main() {
    int shmid = shmget(123, sizeof(SharedData), 0666); // Создание сегмента общей памяти
    SharedData* shared_data = (SharedData*)shmat(shmid, nullptr, 0); // Присоединение сегмента общей памяти

    shared_data->reader_started = true;

    while (true) {
        if (shared_data->ready) {
            std::cout << "Reader received data: " << shared_data->data << std::endl;
            shared_data->ready = false; // Сброс флага после чтения данных
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Небольшая задержка
    }

    shmdt(shared_data); // Отключение сегмента общей памяти

    return 0;
}