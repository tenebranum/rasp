#include <iostream>
#include <thread>
#include <limits>
#include <chrono>
#ifdef __cplusplus
#include <atomic>
#include <mutex>
using namespace std;
#else
#include <stdatomic.h>
#endif
using namespace std::chrono;

int COUNT_READERS_ATOMIC(0);
int COUNT_WRITERS_ATOMIC(0);
int COUNT = 0;
int buffer = 0;
int count_writers = 0;
int count_readers = 0;
std::mutex mut;


void Write(){
    while(count_writers < COUNT && count_readers > 0){
        if(!buffer){
            buffer = count_writers;
            count_writers++;
        }
    }
}

void Read(){
    while(count_readers > 0 && count_writers < COUNT){
        if(buffer){
            buffer = 0;
            count_readers--;
        }
    }
}

void WriteAtomic(){
    while(COUNT_WRITERS_ATOMIC <= COUNT){
        mut.lock();
        if(!buffer){
            buffer = COUNT_WRITERS_ATOMIC;
            COUNT_WRITERS_ATOMIC++;
        }
        mut.unlock();
    }
}

void ReadAtomic(){
    while(COUNT_READERS_ATOMIC >= 0){
        mut.lock();
        if(buffer && COUNT_READERS_ATOMIC >= 0){
            buffer = 0;
            COUNT_READERS_ATOMIC--;
        }
        mut.unlock();
    }
}

int* Get_Slice(int* mas, int start, int end, int step){
    int* new_Mas = new int[end - start];
    int j = 0;
    for (int i = start; i < end; i += step)
    {
        new_Mas[j] = mas[i];
        j++;
    }
    return new_Mas;
}

int* Multiply(int *matrix, int *vector, int matrix_size, int size){
    int* result = new int[matrix_size * size];
    for(int i = 0; i < size; i++){
        result[i] = 0;
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < matrix_size; j++){
            result[i] += matrix[size * i + j] * vector[i];
        }
    }
    return result;
}

void Multiply_Parallel(int *matrix, int *vector, int thread_count, int matrix_size){
    int range = matrix_size * matrix_size / thread_count;
    std::thread *threads = new std::thread[thread_count];
    for (int i = 0; i < thread_count; i++){
        int start = i * range, end = start + range;
        if (i == thread_count - 1 && end != matrix_size * matrix_size){
            end = matrix_size * matrix_size;
        }
        threads[i] = std::thread(Multiply,
                                 Get_Slice(matrix, start, end, 1),
                                 Get_Slice(vector, start / matrix_size, end / matrix_size, 1),
                                 matrix_size,
                                 (end - start) / matrix_size);
    }
    for (int i = 0; i < thread_count; i++)
    {
        threads[i].join();
    }
    delete[] threads;
}

int main(){
    setlocale(LC_ALL, "Russian");
    int matrix_size = 0;
    int thread_count = 0;
    int writers_count = 0;
    int readers_count = 0;
    int is_print = 0;
    std::cout << "Введите размер квадратной матрицы (10, 100, 1000, 10000)\n";
    while (!(std::cin >> matrix_size) || matrix_size %10 != 0)
    { 
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неправильный ввод!\n";
    }
    std::cout << "Введите количество потоков  (2, 3, 4, 5, 10)\n";
    while (!(std::cin >> thread_count) || thread_count > 10)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неправильный ввод!\n";
    }
    int *matrix = new int[matrix_size * matrix_size];
    int *vector = new int[matrix_size];
    for (int i = 0; i < matrix_size * matrix_size; i++){
        matrix[i] = i;
    }
    for(int i = 0; i < matrix_size; i++){
        vector[i] = 4;
    }
    microseconds start_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    int* tmp = Multiply(matrix, vector, matrix_size, matrix_size);
    microseconds end_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    std::cout << "Введите 1, если хотите увидеть результат обработки\n";
    std::cin >> is_print;
    if (is_print == 1){
        for (int i = 0; i < matrix_size; i++){
            cout << "\n" << tmp[i];
        }
    }
    std::cout << "\n" << "Длительность однопотоковой обработки квадратной матрицы и вектора\nразмером ("
              << matrix_size << "): " << (end_time - start_time).count() << "мкс\n";
    start_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    Multiply_Parallel(matrix, vector, thread_count, matrix_size);
    delete[] vector;
    delete[] matrix;
    end_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    std::cout << "Длительность многопотоковой обработки квадратной матрицы и вектора\nразмером (" << matrix_size
              << " потоков " << thread_count << "): " << (end_time - start_time).count() << "мкс\n";
    std::cout << "Введите количество сообщений для буфера (больше 0)\n";
    while (!(std::cin >> COUNT) || COUNT <= 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неправильный ввод!\n";
    }
    std::cout << "Введите количество потоков писателей (больше 0)\n";
    while (!(std::cin >> writers_count) || writers_count <= 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неправильный ввод!\n";
    }
    std::cout << "Введите количество потоков читателей (больше 0)\n";
    while (!(std::cin >> readers_count) || readers_count <= 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неправильный ввод!\n";
    }
    COUNT_READERS_ATOMIC = COUNT;
    count_readers = COUNT;

    start_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    std::thread *threads_writers = new std::thread[writers_count];
    std::thread *threads_readers = new std::thread[readers_count];
    for (int i = 0; i < writers_count; i++){
        threads_writers[i] = std::thread(Write);
    }
        for (int i = 0; i < readers_count; i++)
    {
        threads_readers[i] = std::thread(Read);
    }
    for (int i = 0; i < writers_count; i++){
        threads_writers[i].join();
    }
    for (int i = 0; i < readers_count; i++){
        threads_readers[i].join();
    }
    delete[] threads_writers;
    delete[] threads_readers;
    end_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    cout << "Длительность выполнения без синхронизации" << " потоков " << thread_count  <<\
    (end_time - start_time).count() << "мкс\n";

    start_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    threads_writers = new std::thread[writers_count];
    threads_readers = new std::thread[readers_count];
    for (int i = 0; i < writers_count; i++){
        threads_writers[i] = std::thread(WriteAtomic);
    }
    for (int i = 0; i < readers_count; i++)
    {
        threads_readers[i] = std::thread(ReadAtomic);
    }
    for (int i = 0; i < writers_count; i++){
        threads_writers[i].join();
    }
    for (int i = 0; i < readers_count; i++){
        threads_readers[i].join();
    }
    delete[] threads_writers;
    delete[] threads_readers;
    end_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    cout << "Длительность выполнения c синхронизацией" << " потоков " << thread_count  <<\
     (end_time - start_time).count() << "мкс\n";
    return 0;
}
