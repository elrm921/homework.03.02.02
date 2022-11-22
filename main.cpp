#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;
mutex mlock;

void gotoxy(int x,int y)    
{
    printf("%c[%d;%df",0x1B,y,x);
}

void calc(int num, int length, int &val) {
    mlock.lock();
    gotoxy(0,num);
    cout << "Thread: " << num << " ";
    cout << "Id: " << this_thread::get_id() << " ";
    cout << "Progress: ";
    mlock.unlock();

    float threshold = 0;
    int progress = 0;
    int max_progress = length;
    auto start = chrono::steady_clock::now();

    int n = 0, pos = 0;
    while (progress < max_progress)
    {
        n++;
        progress++;
        val *= n;
        float current = (float) progress / max_progress;
        if (current > threshold)
        {
            mlock.lock();
            gotoxy(40+(pos*2),num);
            cout << string(2, '*') << flush;
            threshold += 0.1;
            pos++;
            mlock.unlock();
        }
    }

    auto end = chrono::steady_clock::now();
    chrono::duration<double, milli> time = end - start;

    mlock.lock();
    gotoxy(60,num);
    cout << " Time: " << time.count() << "ms";
    mlock.unlock();
}

int main()
{
    system("clear");
    int number_threads = 4;
    int calc_length_each_thread = 100'000'000;
    vector<thread> poll(number_threads);
    vector<int> vec(number_threads);
    for (int i=0, n=0; i<poll.size(); i++,n++)
    {
        poll[i] = thread(calc, n+1, calc_length_each_thread, ref(vec[n]));
    }
    for (int i=0; i<poll.size(); i++)
    {
        poll[i].join();
    }
    gotoxy(0,number_threads+1);
    return 0;
}