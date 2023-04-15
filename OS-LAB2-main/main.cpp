#include <iostream>
#include <Windows.h>

using namespace std;

DWORD WINAPI min_max(LPVOID lpParam);
DWORD WINAPI average(LPVOID lpParam);

int *arr; 
int s = 0; 
int array_max = 0,array_min = 0;
double avg;


int main()
{
    cout << "Enter the size of the array: ";
    cin >> s;
    arr = new int[s];
    cout << "Enter the elements of the array: ";
    for (int i = 0; i < s; i++)
    {
        cin >> arr[i];
    }

    // Создание потоков min_max и average
    HANDLE hMinMax = CreateThread(NULL, 0, min_max, NULL, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, average, NULL, 0, NULL);

    // Ожидание завершения работы потоков
    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    // Замена максимального и минимального значения на среднее
    for (int i = 0; i < s; i++)
    {
        if (arr[i] == array_max || arr[i] == array_min)
        {
            arr[i] = avg;
        }
    }

    // Вывод полученных результатов
    cout << "Array after replacing max and min with average:\n";
    for (int i = 0; i < s; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Освобождение памяти
    delete[] arr;
    system("pause");
    return 0;
}

// Функция потока min_max
DWORD WINAPI min_max(LPVOID lpParam)
{
    array_max = arr[0];
    array_min = arr[0];
    for (int i = 1; i < s; i++)
    {
        if (arr[i] > array_max)
        {
            array_max = arr[i];
        }
        if (arr[i] < array_min)
        {
            array_min = arr[i];
        }
        Sleep(7);
    }
    cout << "Max element: " << array_max << endl;
    cout << "Min element: " << array_min << endl;
    return 0;
}


DWORD WINAPI average(LPVOID lpParam)
{
    double sum = 0;
    for (int i = 0; i < s; i++)
    {
        sum += arr[i];
        Sleep(12);  
    }
    avg = sum/s;
    cout << "Avg " << avg << endl;
    return 0;
}