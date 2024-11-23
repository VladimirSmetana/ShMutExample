#ifndef SINGLETON_H
#define SINGLETON_H

#include <shared_mutex>
#include <condition_variable>
#include <memory>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton* instance = createInstance();
        return *instance;
    }

    std::shared_mutex shared_mtx;
    std::condition_variable_any cv;
    bool can_write = true;
    int data = 0;

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton* createInstance() {
        int fd = shm_open("/singleton_shm", O_CREAT | O_RDWR, 0666);
        ftruncate(fd, sizeof(Singleton));
        void* ptr = mmap(nullptr, sizeof(Singleton), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        return new(ptr) Singleton();
    }
};

#endif // SINGLETON_H