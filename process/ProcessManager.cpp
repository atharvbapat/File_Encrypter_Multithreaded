
#include <iostream>
#include "ProcessManager.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <atomic>
#include <stdexcept>
#include <semaphore.h>
#include "./Cryption/Cryption.hpp"

ProcessManagement::ProcessManagement() {
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    if (itemsSemaphore == SEM_FAILED) {
        perror("sem_open failed for items_semaphore");
        exit(EXIT_FAILURE);
    }

    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);
    if (emptySlotsSemaphore == SEM_FAILED) {
        perror("sem_open failed for empty_slots_semaphore");
        exit(EXIT_FAILURE);
    }

    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shmFd, sizeof(SharedMemory)) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    sharedMem = static_cast<SharedMemory*>(
        mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0)
    );
    if (sharedMem == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    sem_wait(emptySlotsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);

    if (sharedMem->size.load() >= 1000) {
        return false;
    }

    std::cout << "Parent process writing task: " << task->toString() << std::endl;
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);

    lock.unlock();
    sem_post(itemsSemaphore);

    int pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return false;
    } else if (pid > 0) {
        std::cout << "Entering the parent process" << std::endl;
    } else {
        std::cout << "Entering the child process" << std::endl;
        executeTasks();
        std::cout << "Exiting the child process" << std::endl;
        exit(0);
    }

    return true;
}

void ProcessManagement::executeTasks() {
    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);

    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);

    lock.unlock();
    sem_post(emptySlotsSemaphore);

    std::cout << "Executing child process" << std::endl;

    if (strlen(taskStr) == 0) {
        throw std::runtime_error("Invalid task data");
    }

    executeCryption(taskStr);
}

ProcessManagement::~ProcessManagement() {
    if (munmap(sharedMem, sizeof(SharedMemory)) == -1) {
        perror("munmap failed");
    }

    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink failed");
    }

    if (sem_close(itemsSemaphore) == -1 || sem_unlink("/items_semaphore") == -1) {
        perror("sem_close/unlink failed for items_semaphore");
    }

    if (sem_close(emptySlotsSemaphore) == -1 || sem_unlink("/empty_slots_semaphore") == -1) {
        perror("sem_close/unlink failed for empty_slots_semaphore");
    }
}
