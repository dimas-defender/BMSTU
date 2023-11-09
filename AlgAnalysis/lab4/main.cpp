#include <cstdlib>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <ctime>
#include <experimental/random>
#include <thread>
#include <memory>
#include <chrono>
#include <utility>
#include <cmath>

using namespace std;

typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() chrono::high_resolution_clock::now()

template<typename F, typename... Args>
double funcTime(F func, Args&&... args)
{
    TimeVar t1 = timeNow();
    func(std::forward<Args>(args)...);
    return duration(timeNow() - t1);
}

void merge(int *array, int begin, int middle, int end)
{
    int i = begin, j = middle + 1, size = end - begin + 1;
    int *temp = (int *)malloc(sizeof(int) * size);
    for (int k = 0; k < size; k++)
    {
        if (i <= middle && (j > end || array[i] < array[j]))
        {
            temp[k] = array[i];
            i++;
        }
        else
        {
            temp[k] = array[j];
            j++;
        }
    }
    for (int l = begin, k = 0; l <= end; l++, k++)
        array[l] = temp[k];
    free(temp);
}

void mergeSort(int *array, int begin, int end)
{
    if (begin < end)
    {
        int middle = (end + begin) / 2;
        mergeSort(array, begin, middle);
        mergeSort(array, middle + 1, end);
        merge(array, begin, middle, end);
    }
}

void paralMergeSort(int *array, int begin, int end, int level, int threads)
{
    if (begin < end)
    {
        int middle = (end + begin) / 2, levels = (int)(log(((double)threads) / log(2.0)));
        if (level < levels)
        {
            thread th(paralMergeSort, array, begin, middle, level + 1, threads);
            paralMergeSort(array, middle + 1, end, level + 1, threads);
            th.join();
        }
        else
        {
            mergeSort(array, begin, middle);
            mergeSort(array, middle + 1, end);
        }
        merge(array, begin, middle, end);
    }
}

int main()
{
    srand(time(0));
    bool endflag = false;
    int choice;
    while (!endflag)
    {
        cout << "Menu:\n\
    1. Sort array by merge sort\n\
    2. Sort array by parallel merge sort\n\
    3. Time measurements\n\n\
Your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            cout << "\nInput size of array: ";
            int size;
            cin >> size;
            cout << "Input elements of array: ";
            int *arr = (int *)malloc(sizeof(int) * size);
            for (int i = 0; i < size; i++)
                cin >> arr[i];
            mergeSort(arr, 0, size - 1);
            cout << "Sorted array: ";
            for (int i = 0; i < size; i++)
                cout << arr[i] << ' ';
            cout << "\n\n";
            free(arr);
        }
        else if (choice == 2)
        {
            cout << "\nInput count of threads(2, 4, 8, or 16): ";
            int threads;
            cin >> threads;
            cout << "Input size of array: ";
            int size;
            cin >> size;
            cout << "Input elements of array: ";
            int *arr = (int *)malloc(sizeof(int) * size);
            for (int i = 0; i < size; i++)
                cin >> arr[i];
            paralMergeSort(arr, 0, size - 1, 0, threads);
            cout << "Sorted array: ";
            for (int i = 0; i < size; i++)
                cout << arr[i] << ' ';
            cout << "\n\n";
            free(arr);
        }
        else if (choice == 3)
        {
            cout << "N   Simple Par2 Par4 Par8 Par16" << endl;
            long double simpletime, paraltime2, paraltime4, paraltime8, paraltime16;
            int *sizes = (int *)malloc(sizeof(int) * 7);
            sizes[0] = 1000;
            sizes[1] = 5000;
            sizes[2] = 10000;
            sizes[3] = 100000;
            sizes[4] = 200000;
            sizes[5] = 500000;
            sizes[6] = 1000000;
            for (int i = 0; i < 7; i++)
            {
                simpletime = 0.0;
                paraltime2 = 0.0;
                paraltime4 = 0.0;
                paraltime8 = 0.0;
                paraltime16 = 0.0;
                int *arr = (int *)malloc(sizeof(int) * sizes[i]);
                int *copy = (int *)malloc(sizeof(int) * sizes[i]);
                for (int k = 0; k < 10; k++)
                {
                    for (int j = 0; j < sizes[i]; j++)
                    {
                        arr[j] = experimental::randint(-2 * sizes[i], 2 * sizes[i]);
                        copy[j] = arr[j];
                    }
                    simpletime += funcTime(mergeSort, arr, 0, sizes[i] - 1);
                    for (int j = 0; j < sizes[i]; j++)
                        arr[j] = copy[j];
                    paraltime2 += funcTime(paralMergeSort, arr, 0, sizes[i] - 1, 0, 2);
                    for (int j = 0; j < sizes[i]; j++)
                        arr[j] = copy[j];
                    paraltime4 += funcTime(paralMergeSort, arr, 0, sizes[i] - 1, 0, 4);
                    for (int j = 0; j < sizes[i]; j++)
                        arr[j] = copy[j];
                    paraltime8 += funcTime(paralMergeSort, arr, 0, sizes[i] - 1, 0, 8);
                    for (int j = 0; j < sizes[i]; j++)
                        arr[j] = copy[j];
                    paraltime16 += funcTime(paralMergeSort, arr, 0, sizes[i] - 1, 0, 16);
                }
                simpletime /= 10000000;
                paraltime2 /= 10000000;
                paraltime4 /= 10000000;
                paraltime8 /= 10000000;
                paraltime16 /= 10000000;
                cout << sizes[i] << ' ';
                cout << simpletime << ' ' << paraltime2 << ' ' << paraltime4 << ' ' << paraltime8 << ' ' << paraltime16 << endl;
                free(arr);
                free(copy);
            }
            free(sizes);
        }
        else
            endflag = true;
    }
    return 0;
}
