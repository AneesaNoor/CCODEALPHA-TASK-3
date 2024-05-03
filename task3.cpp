#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;

int counter = 0;

void incrementCounter(int id) {
    unique_lock<mutex> lock(mtx);
    counter++;
    cout << "Thread " << id << " incremented counter to: " << counter << endl;
    lock.unlock();
    cv.notify_all();
}

void decrementCounter(int id) {
    unique_lock<mutex> lock(mtx);
    while (counter == 0) {
        cv.wait(lock);
    }
    counter--;
    cout << "Thread " << id << " decremented counter to: " << counter << endl;
    lock.unlock();
}

int main() {
    const int num_threads = 5;
    thread threads[num_threads];

    // Create threads for incrementing
    for (int i = 0; i < num_threads; ++i) {
        threads[i] = thread(incrementCounter, i + 1);
    }

    // Join threads for incrementing
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    // Create threads for decrementing
    for (int i = 0; i < num_threads; ++i) {
        threads[i] = thread(decrementCounter, i + 1);
    }

    // Join threads for decrementing
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    return 0;
}

