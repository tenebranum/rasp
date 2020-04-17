#include <iostream>
#include <thread>
#include <limits>
#include <chrono>
#include <mutex>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;


bool is_buffer_empty = true;
bool finish = false;
int COUNT = 0;
string buffer = "";
int count_writers = 0;
int count_readers = 0;
int count_now = 0;
mutex mut;



void Write()
{
    while (count_now < COUNT)
    {
        if (is_buffer_empty && count_now < COUNT)
        {
            buffer = to_string(count_now++);
            is_buffer_empty = false;
        }
    }
}

void Read()
{
    while (!finish)
    {
        if (!is_buffer_empty)
        {
            cout << buffer << '\n';
            is_buffer_empty = true;
        }
    }
}

void Write_Mut()
{
    while (count_now < COUNT)
    {
        mut.lock();
        if (is_buffer_empty && count_now < COUNT)
        {
            buffer = to_string(count_now++);
            is_buffer_empty = false;
        }
        mut.unlock();
    }
}

void Read_Mut()
{
    while (!finish)
    {
        if (!is_buffer_empty)
        {
            mut.lock();
            if (!is_buffer_empty)
            {
                cout << buffer << '\n';
                is_buffer_empty = true;
            }
            mut.unlock();
        }
    }
}


int* Get_Slice(int* mas, int start, int end, int step)
{
    int* new_Mas = new int[end - start];
    int j = 0;
    for (int i = start; i <= end; i += step)
    {
        new_Mas[j] = mas[i];
        j++;
    }
    return new_Mas;
}

int* Sum(int* first_mas, int* second_mas, int* third_mas, int size)
{
    int* result_mas = new int[size];
    cout << "\n";
    for (int i = 0; i < size; i++){
        result_mas[i] = 0;
        result_mas[i] += first_mas[i] + second_mas[i] + third_mas[i];
        cout << result_mas[i] << "\n";
    }
    return result_mas;
}

void Sum_Parallel(int* first_mas, int* second_mas, int* third_mas, int size, int thread_count)
{
    int start, end;
    int range = size / thread_count;
    thread* threads = new thread[thread_count];
    for (int i = 0; i < thread_count; i++)
    {
        start = i * range;
        end = start + range - 1;
        if(i == thread_count - 1){
            end = size - 1;
        }
        threads[i] = std::thread(Sum, Get_Slice(first_mas, start, end, 1), Get_Slice(second_mas, start, end, 1), Get_Slice(third_mas, start, end, 1), end - start + 1);
    }
    for (int i = 0; i < thread_count; i++)
    {
        threads[i].join();
    }
    delete[] threads;
}

void Vector_Sum()
{
    int Size_Of_Vector = 0;
    int thread_count = 0;
    cout << "Введите размер массива (10, 100, 1000, 10000)\n";
    while (!(cin >> Size_Of_Vector) || Size_Of_Vector % 10 != 0)
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Некорректный ввод!\n";
    }
    cout << "Введите количество потоков для многопоточной обработки  (2, 3, 4, 5, 10)\n";
    while (!(cin >> thread_count) || thread_count > 10)
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Некорректный ввод!\n";
    }
    int* arVector1 = new int[Size_Of_Vector];
    int* arVector2 = new int[Size_Of_Vector];
    int* arVector3 = new int[Size_Of_Vector];
    for (int i = 0; i < Size_Of_Vector; i++){
        arVector1[i] = 1;
        arVector2[i] = 2;
        arVector3[i] = 3;
    }
    microseconds ms = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    Sum(arVector1, arVector2, arVector3, Size_Of_Vector);
    microseconds ms_1 = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    cout << "Время (в мкс) выполнения последовательной обработки вектора (" << Size_Of_Vector << " элементов): " << (ms_1 - ms).count() << "\n";
    ms = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    Sum_Parallel(arVector1, arVector2, arVector3, Size_Of_Vector, thread_count);
    ms_1 = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    cout << "Время (в мкс) выполнения многопоточной обработки вектора (" << Size_Of_Vector << " элементов и " << thread_count << " потоков): " << (ms_1 - ms).count() << "\n";
    delete[] arVector1;
    delete[] arVector2;
    delete[] arVector3;
}


void Readers_Writers()
{
    cout << "Введите количество сообщений для буфера (больше 0)\n";
    while (!(cin >> COUNT) || COUNT <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректный ввод!\n";
    }
    cout << "Введите количество потоков писателей (больше 0)\n";
    while (!(cin >> count_writers) || count_writers <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректный ввод!\n";
    }
    cout << "Введите количество потоков читателей (больше 0)\n";
    while (!(cin >> count_readers) || count_readers <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректный ввод!\n";
    }
    vector<thread> threads_writers(count_writers);
    vector<thread> threads_readers(count_readers);
    microseconds start_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    for (thread& th : threads_writers)
    {
        th = thread(Write);
    }
    for (thread& th : threads_readers)
    {
        th = thread(Read);
    }
    for (thread& th : threads_writers)
    {
        th.join();
    }
    finish = true;
    for (thread& th : threads_readers)
    {
        th.join();
    }
    microseconds end_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    cout << "Время работы с буфером без средств синхронизации: " << (end_time - start_time).count() << '\n';
    buffer = "";
    is_buffer_empty = true;
    finish = false;
    count_now = 0;
    vector<thread> threads_writers_1(count_writers);
    vector<thread> threads_readers_1(count_readers);
    start_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    for (thread& th : threads_writers_1)
    {
        th = thread(Write_Mut);
    }
    for (thread& th : threads_readers_1)
    {
        th = thread(Read_Mut);
    }
    for (thread& th : threads_writers_1)
    {
        th.join();
    }
    finish = true;
    for (thread& th : threads_readers_1)
    {
        th.join();
    }
    end_time = duration_cast<microseconds>(system_clock::now().time_since_epoch());
    cout << "Время работы с буфером со средствами синхронизации: " << (end_time - start_time).count() << '\n';
}


int main()
{
    setlocale(LC_ALL, "Russian");
    Vector_Sum();
    Readers_Writers();
}