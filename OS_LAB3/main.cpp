#include <iostream>
#include <windows.h>
#include "ThreadInfo.h"


HANDLE* fromMain;
HANDLE* fromThread;
int* array;
int arr_size;
CRITICAL_SECTION cs;


void printArray() {
    for (int i = 0; i < arr_size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}


DWORD WINAPI Marker(LPVOID v) {
    auto cur = (ThreadInfo*)v; 
    EnterCriticalSection(&cs);
    srand(cur->num);
    
    bool* marked = new bool[arr_size]; 
    for (int i = 0; i < arr_size; i++) {
        marked[i] = false;
    }
    LeaveCriticalSection(&cs);

    while(true) {
        int num = rand();
        num %= (arr_size); 
        EnterCriticalSection(&cs);
        if (array[num] == 0) { 
            Sleep(5);
            array[num] = cur->num;
            marked[num] = true;
            ++cur->marked_num;
            LeaveCriticalSection(&cs);
            Sleep(5);
        }
        else if (array[num] != 0) { 
            std::cout << "\nMarker №" << cur->num << "\nCan't modify the element with index: " 
                        << num << "\nNumber of marked elements: " << cur->marked_num << std::endl << std::endl;
            LeaveCriticalSection(&cs);

            SetEvent(fromThread[cur->num - 1]);
            WaitForSingleObject(fromMain[cur->num - 1], INFINITE);
            ResetEvent(fromThread[cur->num - 1]);
            ResetEvent(fromMain[cur->num - 1]);
			WaitForSingleObject(fromMain[cur->num - 1], INFINITE);

            if (cur->flag == true) {
                EnterCriticalSection(&cs);
                for (int i = 0; i < arr_size; i++) {
                    if (marked[i]) {
                        array[i] = 0;
                    }
                }
                cur->marked_num = 0;
                LeaveCriticalSection(&cs);
                break;

            }
        }
    }
    delete[] marked;

    SetEvent(fromThread[cur->num - 1]);
    
    EnterCriticalSection(&cs);
    std::cout << "Thread №" << cur->num << " is finished\n";
    LeaveCriticalSection(&cs);
    return 0;
}


int main() {

    std::cout << "Array size: " << std::endl;
    std::cin >> arr_size;

    array = new int[arr_size];
    for (int i = 0; i < arr_size; i++) {
        array[i] = 0;
    }

    std::cout << "Thread number: " << std::endl;
    int tnum;
    std::cin >> tnum;
    ThreadInfo** tarr = new ThreadInfo*[tnum];
    DWORD dword;
    InitializeCriticalSection(&cs);

    fromMain = new HANDLE[tnum];
    fromThread = new HANDLE[tnum];
    for (int i = 0; i < tnum; i++) {
        tarr[i] = new ThreadInfo; 
        fromMain[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        fromThread[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        (*tarr[i]).num = i + 1;
        (*tarr[i]).h = CreateThread(NULL, 0, Marker, (LPVOID)tarr[i], 0, &dword);
    }

    int n;

    for (int i = 0; i < tnum; i++) {

        WaitForMultipleObjects(tnum, fromThread, TRUE, INFINITE);


        for (int j = 0; j < tnum; j++)
			SetEvent(fromMain[j]);

	    EnterCriticalSection(&cs);
        printArray();
        do {
            std::cout << "Enter the num of thread to stop: ";
            std::cin >> n;
            if (tarr[n-1]->flag) {
                std::cout << "This thread is already finished\n";
            }
        } while (tarr[n-1]->flag);

        (*tarr[n-1]).flag = true;
        LeaveCriticalSection(&cs);

        SetEvent(fromMain[n-1]);
        WaitForSingleObject(fromThread[n-1], INFINITE);

        for (int j = 0; j < tnum; j++) {
            SetEvent(fromMain[j]);
            ResetEvent(fromMain[j]);
        }
    }
    std::cout << "All threads are finished";
    for (int i = 0; i < tnum; i++) {
        CloseHandle(tarr[i]->h);
        CloseHandle(fromThread[i]);
        CloseHandle(fromMain[i]);
    }
    delete[] tarr;
    delete[] fromThread;
    delete[] fromMain;
    DeleteCriticalSection(&cs);
    delete[] array;
    return 0;
}