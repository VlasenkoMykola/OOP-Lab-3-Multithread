#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <random>

class Algorithms_testclass {
private:
    int bigarray[];//TODO: set exact array size in const
public:
    //return pointer
    int* get_bigarray() { return bigarray; };
};


void print(int n, const std::string& str) {
    std::string msg = std::to_string(n) + " : " + str;
    std::cout << msg << std::endl;
}

//different threads will fill data between different spots

static std::default_random_engine generator;
static std::uniform_int_distribution<int> distribution_test(-100, 100);

void fill_int_array_with_data_singlethread(int array[], int size) {
    std::chrono::steady_clock::time_point _start(std::chrono::steady_clock::now());
    for (int i = 0; i < size; i++) {
        array[i] = distribution_test(generator);
    }
    std::chrono::steady_clock::time_point _end(std::chrono::steady_clock::now());

    std::cout << "singlethreaded random data fill duration (seconds): " << std::chrono::duration_cast<std::chrono::duration<double>>(
        _end - _start).count() << std::endl; // in seconds
}

//has start/end indexes so that each thread works on different parts of the array

void fill_int_array_with_data_multithread_part(int array[], int start_index, int end_index) {
    for (int i = start_index; i < end_index; i++) {
        array[i] = distribution_test(generator);
    }
}

void fill_int_array_with_data_multithread(int array[], int array_size, int num_threads) {
    std::vector<std::thread> threads_datafill;

    int start_pos;
    int end_pos;

    std::chrono::steady_clock::time_point _start(std::chrono::steady_clock::now());

    for (int i = 0; i < num_threads; i++) {
        int start_pos = (array_size / num_threads) * i;
        int end_pos = (array_size / num_threads) * (i + 1);
        std::cout << "start pos" << start_pos << std::endl;
        std::cout << "end pos" << end_pos << std::endl;
        threads_datafill.push_back(std::thread(fill_int_array_with_data_multithread_part, array, start_pos, end_pos));
    }

    for (auto& th : threads_datafill) {
        th.join();
    }


    std::chrono::steady_clock::time_point _end(std::chrono::steady_clock::now());

    std::cout << "multithreaded random data fill duration (seconds): " << std::chrono::duration_cast<std::chrono::duration<double>>(
        _end - _start).count() << std::endl; // in seconds

}

void print_array_singlethread(int array[], int array_size) {
    std::chrono::steady_clock::time_point _start(std::chrono::steady_clock::now());

    for (int i = 0; i < array_size; i++) {
//        std::cout << array[i] << std::endl;
        std::cout << array[i] << " ";//no endl here, to keep the output more readable
    }
    std::cout << std::endl;
    std::chrono::steady_clock::time_point _end(std::chrono::steady_clock::now());

    std::cout << "singlethreaded print duration (seconds): " << std::chrono::duration_cast<std::chrono::duration<double>>(
        _end - _start).count() << std::endl; // in seconds
}

void print_array_multithread_part(int array[], int start_index, int end_index) {
    for (int i = start_index; i < end_index; i++) {
        std::cout << array[i] << " ";//no endl here, to keep the output more readable
    }
}

void print_array_multithread(int array[], int array_size, int num_threads) {
    std::vector<std::thread> threads_datafill;

    int start_pos;
    int end_pos;

    std::chrono::steady_clock::time_point _start(std::chrono::steady_clock::now());

    for (int i = 0; i < num_threads; i++) {
        int start_pos = (array_size / num_threads) * i;
        int end_pos = (array_size / num_threads) * (i + 1);
        std::cout << "start pos" << start_pos << std::endl;
        std::cout << "end pos" << end_pos << std::endl;
        threads_datafill.push_back(std::thread(print_array_multithread_part, array, start_pos, end_pos));
    }

    for (auto& th : threads_datafill) {
        th.join();
    }
    std::cout << std::endl;

    std::chrono::steady_clock::time_point _end(std::chrono::steady_clock::now());

    std::cout << "multithreaded print duration (seconds): " << std::chrono::duration_cast<std::chrono::duration<double>>(
        _end - _start).count() << std::endl; // in seconds

}


int main()
{
    srand(time(NULL));

    const int big_array1_size = 10000;


    int big_array1[big_array1_size];

    std::cout << "filling with singlethread first" << std::endl;

    fill_int_array_with_data_singlethread(big_array1, big_array1_size);

    std::cout << "printing singlethread-filled array with singlethread print:" << std::endl;
    print_array_singlethread(big_array1, big_array1_size);

    fill_int_array_with_data_multithread(big_array1, big_array1_size, 5);//5 threads

    std::cout << "printing multithread-filled array with multithread print:" << std::endl;
    print_array_multithread(big_array1, big_array1_size, 10);//10 threads

    return 0;
}