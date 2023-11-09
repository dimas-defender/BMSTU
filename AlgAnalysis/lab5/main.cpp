#include <iostream>
#include <chrono>
#include <queue>
#include <thread>
#include <mutex>
#include <cmath>
#include <experimental/random>
#include "windows.h"

using namespace std;

typedef chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() chrono::high_resolution_clock::now()
#define EPS 1e-6

class request {
public:
    request();
    ~request();
    void input();
    void print();
    void setToCofactor();
    void setToTranspose();
    void divideByDet();
    float **values;
    float det;
    TimeVar in_line1;
    TimeVar out_line1;
    TimeVar in_line2;
    TimeVar out_line2;
    TimeVar in_line3;
    TimeVar out_line3;
};

request::request()
{
    values = new float*[3];
    for (int i = 0; i < 3; i++)
        values[i] = new float[3];
}

request::~request()
{
    for (int i = 0; i < 3; ++i)
        delete[] values[i];
    delete[] values;
}

void request::divideByDet()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (fabs(det) < EPS)
                values[i][j] = 0;
            else
                values[i][j] /= det;
        }
    }
}

void request::setToCofactor()
{
    float temp1 = values[1][1]*values[2][2] -\
              values[1][2]*values[2][1];
    float temp2 = -(values[1][0]*values[2][2] -\
                values[1][2]*values[2][0]);
    float temp3 = values[1][0]*values[2][1] -\
              values[1][1]*values[2][0];
    float temp4 = -(values[0][1]*values[2][2] -\
                values[0][2]*values[2][1]);
    float temp5 = values[0][0]*values[2][2] -\
              values[0][2]*values[2][0];
    float temp6 = -(values[0][0]*values[2][1] -
                values[0][1]*values[2][0]);
    values[2][0] = values[0][1]*values[1][2] -\
              values[0][2]*values[1][1];
    values[2][1] = -(values[0][0]*values[1][2] -\
                values[0][2]*values[1][0]);
    values[2][2] = values[0][0]*values[1][1] -\
              values[0][1]*values[1][0];
    values[0][0] = temp1;
    values[0][1] = temp2;
    values[0][2] = temp3;
    values[1][0] = temp4;
    values[1][1] = temp5;
    values[1][2] = temp6;
}

void request::setToTranspose()
{
    float temp = values[0][1];
    values[0][1] = values[1][0];
    values[1][0] = temp;
    temp = values[0][2];
    values[0][2] = values[2][0];
    values[2][0] = temp;
    temp = values[1][2];
    values[1][2] = values[2][1];
    values[2][1] = temp;
}

void calcDet(request *req)
{
    req->det =\
       req->values[0][0]*(req->values[1][1]*req->values[2][2] -\
                             req->values[1][2]*req->values[2][1]) -\
       req->values[0][1]*(req->values[1][0]*req->values[2][2] -\
                             req->values[1][2]*req->values[2][0]) +\
       req->values[0][2]*(req->values[1][0]*req->values[2][1] -\
                             req->values[1][1]*req->values[2][0]);
}

void request::input()
{
    cout<<"Input matrix:\n";
    for (int i = 0; i < 3; i++)
        cin>>values[i][0]>>values[i][1]>>values[i][2];
    calcDet(this);
}

void request::print()
{
    cout<<"Calculated matrix:\n";
    for (int i = 0; i < 3; i++)
        cout<<values[i][0]<<' '<<values[i][1]<<' '<<values[i][2]<<"\n";
    cout<<"\n";
}

void printMenu()
{
    cout<<"Menu:\n    1. Calculate inverse matrix by order."
           "\n    2. Calculate inverse matrix by threads."
           "\n    3. Calculate some inverse matrixes by order."
           "\n    4. Calculate some inverse matrixes by threads."
           "\n    5. Complex test both executions."
           "\n\nYour choice: ";
}

queue<request *> queue1;
queue<request *> queue2;
queue<request *> queue3;
queue<request *> queue4;

mutex mutex1;
mutex mutex2;
mutex mutex3;

int queue_size = 0;

void line1()
{
//    int processed = 0;
//    while (processed != queue_size)
//    {
//        mutex1.lock();
//        if (queue1.empty())
//        {
//            mutex1.unlock();
//            continue;
//        }
//        request *temp = queue1.front();
//        queue1.pop();
//        mutex1.unlock();
//        temp->in_line1 = timeNow();
//        temp->setToCofactor();
//        temp->out_line1 = timeNow();
//        mutex2.lock();
//        queue2.push(temp);
//        mutex2.unlock();
//        processed++;
//    }
     Sleep(100);
}

void line2()
{
//    int processed = 0;
//    while (processed != queue_size)
//    {
//        mutex2.lock();
//        if (queue2.empty())
//        {
//            mutex2.unlock();
//            continue;
//        }
//        request *temp = queue2.front();
//        queue2.pop();
//        mutex2.unlock();
//        temp->in_line2 = timeNow();
//        temp->setToTranspose();
//        temp->out_line2 = timeNow();
//        mutex3.lock();
//        queue3.push(temp);
//        mutex3.unlock();
//        processed++;
//    }
     Sleep(100);
}

void line3()
{
//    int processed = 0;
//    while (processed != queue_size)
//    {
//        mutex3.lock();
//        if (queue3.empty())
//        {
//            mutex3.unlock();
//            continue;
//        }
//        request *temp = queue3.front();
//        queue3.pop();
//        mutex3.unlock();
//        temp->in_line3 = timeNow();
//        temp->divideByDet();
//        temp->out_line3 = timeNow();
//        queue4.push(temp);
//        processed++;
//    }
     Sleep(100);
}

void line1Order()
{
//    int processed = 0;
//    while (processed != queue_size)
//    {
//        request *temp = queue1.front();
//        queue1.pop();
//        temp->in_line1 = timeNow();
//        temp->setToCofactor();
//        temp->out_line1 = timeNow();
//        queue2.push(temp);
//        processed++;
//    }
     Sleep(100);
}

void line2Order()
{
//    int processed = 0;
//    while (processed != queue_size)
//    {
//        request *temp = queue2.front();
//        queue2.pop();
//        temp->in_line2 = timeNow();
//        temp->setToTranspose();
//        temp->out_line2 = timeNow();
//        queue3.push(temp);
//        processed++;
//    }
     Sleep(100);
}

void line3Order()
{
//    int processed = 0;
//    while (processed != queue_size)
//    {
//        request *temp = queue3.front();
//        queue3.pop();
//        temp->in_line3 = timeNow();
//        temp->divideByDet();
//        temp->out_line3 = timeNow();
//        queue4.push(temp);
//        processed+=1;
//    }
     Sleep(100);
}

int main()
{
    srand(time(nullptr));
    bool to_continue = true;
    int mode = 0;
    while (to_continue)
    {
        printMenu();
        cin>>mode;
        if (mode == 1)
        {
            queue_size = 1;
            request *new_one = new request;
            new_one->input();
            queue1.push(new_one);
            line1();
            line2();
            line3();
            cout<<'\n';
            new_one->print();
            delete new_one;
        }
        else if (mode == 2)
        {
            queue_size = 1;
            request *new_one = new request;
            new_one->input();
            thread thread1(line1);
            thread thread2(line2);
            thread thread3(line3);
            mutex1.lock();
            queue1.push(new_one);
            mutex1.unlock();
            thread1.join();
            //if (thread2.joinable())
            thread2.join();
            thread3.join();
            cout<<'\n';
            new_one->print();
            delete new_one;
        }
        else if (mode == 3)
        {
            cout<<"Input count matrixes: ";
            cin>>queue_size;
            if (queue_size < 1)
            {
                cout<<"Wrong input! Try again...\n\n";
                continue;
            }
            request *temp;
            for (int i = 0; i < queue_size; i++)
            {
                temp = new request;
                temp->input();
                queue1.push(temp);
            }
            cout<<'\n';
            line1();
            line2();
            line3();
            for (int i = 0; i < queue_size; i++)
            {
                temp = queue4.front();
                temp->print();
                queue4.pop();
                delete temp;
            }
        }
        else if (mode == 4)
        {
            cout<<"Input count matrixes: ";
            cin>>queue_size;
            if (queue_size < 1)
            {
                cout<<"Wrong input! Try again...\n\n";
                continue;
            }
            thread thread1(line1);
            thread thread2(line2);
            thread thread3(line3);
            request *temp;
            for (int i = 0; i < queue_size; i++)
            {
                temp = new request;
                temp->input();
                mutex1.lock();
                queue1.push(temp);
                mutex1.unlock();
            }
            thread1.join();
            thread2.join();
            thread3.join();
            cout<<'\n';
            for (int i = 0; i < queue_size; i++)
            {
                temp = queue4.front();
                temp->print();
                queue4.pop();
                delete temp;
            }
        }
        else if (mode == 5)
        {
            for (int k = 50; k < 81; k += 10)
            {
                queue_size = k;
                request **requests1 = new request*[k];
                request **requests2 = new request*[k];
                for (int l = 0; l < k; l++)
                {
                    requests1[l] = new request;
                    requests2[l] = new request;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            float temp = experimental::randint(-64, 64);
                            requests1[l]->values[i][j] = temp;
                            requests2[l]->values[i][j] = temp;
                        }
                    }
                    calcDet(requests1[l]);
                    calcDet(requests2[l]);
                }
                // TimeVar start_time = timeNow();
                TimeVar t1 = timeNow();
                thread thread1(line1);
                thread thread2(line2);
                thread thread3(line3);
                for (int i = 0; i < k; i++)
                {
                    mutex1.lock();
                    queue1.push(requests1[i]);
                    mutex1.unlock();
                }
                thread1.join();
                thread2.join();
                thread3.join();
                TimeVar t2 = timeNow();
                cout<<"\nQueue size = "<< k << "\n";
                cout<<"|parall "<<duration(t2-t1)<<"|\n";
                // for (int i = 0; i < k; ++i) {
                //    queue4.pop();
                // }
                // TimeVar start_time1 = timeNow();
                t1 = timeNow();
                for (int i = 0; i < k; ++i) {
                    queue1.push(requests2[i]);
                }
                line1Order();
                line2Order();
                line3Order();
                t2 = timeNow();
                cout<<"|consec "<<duration(t2-t1)<<"|\n";
                // for (int i = 0; i < k; ++i) {
                //    queue4.pop();
                // }
                // cout<<"Line N       Request N       Start Time       End Time\n";
                // cout<<"---------------------------------------------------------\n";
                // for (int i = 0; i < 9; ++i) {
                //     cout<<"Line 1       Request "<<i+1<<"       "<<duration(requests1[i]->in_line1-start_time)<<" & "
                //         <<duration(requests1[i]->out_line1-start_time)<<"\n";
                //     cout<<"Line 2       Request "<<i+1<<"       "<<duration(requests1[i]->in_line2-start_time)<<" & "
                //         <<duration(requests1[i]->out_line2-start_time)<<"\n";
                //     cout<<"Line 3       Request "<<i+1<<"       "<<duration(requests1[i]->in_line3-start_time)<<" & "
                //         <<duration(requests1[i]->out_line3-start_time)<<"\n";
                //     cout<<"---------------------------------------------------------\n";

                // }
//                for (int i = 9; i < k; ++i) {
//                    cout<<"Line 1       Request "<<i+1<<"      "<<duration(requests1[i]->in_line1-start_time)<<" & "
//                        <<duration(requests1[i]->out_line1-start_time)<<"\n";
//                    cout<<"Line 2       Request "<<i+1<<"      "<<duration(requests1[i]->in_line2-start_time)<<" & "
//                        <<duration(requests1[i]->out_line2-start_time)<<"\n";
//                    cout<<"Line 3       Request "<<i+1<<"      "<<duration(requests1[i]->in_line3-start_time)<<" & "
//                        <<duration(requests1[i]->out_line3-start_time)<<"\n";
//                    cout<<"----------------------------------------------------------\n";
//                }
                // cout<<"\n\nLine N       Request N       Start Time       End Time\n";
                // cout<<"---------------------------------------------------------\n";
                // for (int i = 0; i < 9; ++i) {
                //     cout<<"Line 1       Request "<<i+1<<"       "<<duration(requests2[i]->in_line1-start_time1)<<" & "
                //         <<duration(requests2[i]->out_line1-start_time1)<<"\n";
                //     cout<<"Line 2       Request "<<i+1<<"       "<<duration(requests2[i]->in_line2-start_time1)<<" & "
                //         <<duration(requests2[i]->out_line2-start_time1)<<"\n";
                //     cout<<"Line 3       Request "<<i+1<<"       "<<duration(requests2[i]->in_line3-start_time1)<<" & "
                //         <<duration(requests2[i]->out_line3-start_time1)<<"\n";
                //     cout<<"---------------------------------------------------------\n";

                // }
//                for (int i = 9; i < k; ++i) {
//                    cout<<"Line 1       Request "<<i+1<<"      "<<duration(requests2[i]->in_line1-start_time1)<<" & "
//                        <<duration(requests2[i]->out_line1-start_time1)<<"\n";
//                    cout<<"Line 2       Request "<<i+1<<"      "<<duration(requests2[i]->in_line2-start_time1)<<" & "
//                        <<duration(requests2[i]->out_line2-start_time1)<<"\n";
//                    cout<<"Line 3       Request "<<i+1<<"      "<<duration(requests2[i]->in_line3-start_time1)<<" & "
//                        <<duration(requests2[i]->out_line3-start_time1)<<"\n";
//                    cout<<"----------------------------------------------------------\n";
//                }
                for (int i = 0; i < k; i++)
                {
                    delete requests1[i];
                    delete requests2[i];
                }
                delete[] requests1;
                delete[] requests2;
            }
        }
        else
            to_continue = false;
    }
    return 0;
}
