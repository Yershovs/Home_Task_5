#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono> // Для измерения времени
#include <omp.h> // Для работы с OpenMP
#include <cstdlib> // Для setenv
#include "My_Structurs.h"
using namespace std;

// Функция для слияния двух упорядоченных частей массива с использованием временного массива
template <typename T>
void merge(vector<T>& A, int start, int mid, int end, vector<T>& temp) {
    int i = start, j = mid, k = start;

    while (i < mid && j < end) {
        if (A[i] < A[j]) {
            temp[k++] = A[i++];
        } else {
            temp[k++] = A[j++];
        }
    }

    while (i < mid) {
        temp[k++] = A[i++];
    }

    while (j < end) {
        temp[k++] = A[j++];
    }

    // Копируем результат обратно в исходный массив
    for (int i = start; i < end; ++i) {
        A[i] = temp[i];
    }
}

// Параллельная сортировка слиянием с использованием индексов
template <typename T>
void parallelMergeSort(vector<T>& A, int start, int end, vector<T>& temp, int depth = 0) {
    int N = end - start;
    if (N <= 1000) { // Для небольших массивов используем std::sort
        sort(A.begin() + start, A.begin() + end);
        return;
    }

    int mid = start + N / 2;

    // Максимальная глубина параллелизма
    int maxDepth = omp_get_max_threads();

    if (depth < maxDepth) {
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(A, start, mid, temp, depth + 1);

            #pragma omp section
            parallelMergeSort(A, mid, end, temp, depth + 1);
        }
    } else {
        parallelMergeSort(A, start, mid, temp, depth + 1);
        parallelMergeSort(A, mid, end, temp, depth + 1);
    }

    // Слияние двух отсортированных половин
    merge(A, start, mid, end, temp);
}

int main() {
    // Устанавливаем количество потоков для OpenMP
    int numThreads = 8; // Количество ядер процессора
    omp_set_num_threads(numThreads);

    // Проверяем количество потоков
    cout << "Using up to " << omp_get_max_threads() << " threads.\n";

    // Размер массива
    const int size_1 = 100000000;
    const int size_2 = 10000;    

    // Генерация случайного массива
    vector<int> vector_1(size_1);
    vector<int> vector_2(size_1);

    // Инициализация случайных чисел
    srand(time(nullptr));
    for (int i = 0; i < size_1; i++) {
        vector_1[i] = generate();
    }
    vector_2 = vector_1;

    // Параллельная сортировка массива
    vector<int> temp(size_1); // Временный массив для слияния
    auto start = chrono::high_resolution_clock::now();
    parallelMergeSort(vector_1, 0, vector_1.size(), temp);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Parallel Merge Sort time: " << elapsed.count() << " seconds" << endl;

    // Сортировка с использованием std::sort
    auto start_1 = chrono::high_resolution_clock::now();
    sort(vector_2.begin(), vector_2.end());
    auto end_1 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_1 = end_1 - start_1;

    cout << "std::sort time: " << elapsed_1.count() << " seconds" << endl;

    vector<ListNode*> linkedLists(size_2);
    vector<ListNode*> linkedListsCopy(size_2);
    // Генерация случайных связных списков
    for (int i = 0; i < size_2; ++i) { // Создаём 1000 связных списков
        ListNode* head = new ListNode(generate());
        ListNode* current = head;
        for (int j = 1; j < 1000; ++j) {
            current->next = new ListNode(generate());
            current = current->next;
        }
        linkedLists.push_back(head);
    }

  // Копия для std::sort
  linkedListsCopy = linkedLists;

    // Параллельная сортировка связных списков
    auto start_list = chrono::high_resolution_clock::now();
    vector<ListNode*> tempLists(linkedLists.size());
    parallelMergeSort(linkedLists, 0, linkedLists.size(), tempLists);
    auto end_list = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_list = end_list - start_list;

    cout << "Parallel Linked List Sort time: " << elapsed_list.count() << " seconds" << endl;

    // Сортировка связных списков с использованием std::sort
    auto start_list_std = chrono::high_resolution_clock::now();
    sort(linkedListsCopy.begin(), linkedListsCopy.end(), [](ListNode* a, ListNode* b) { return *a < *b; });
    auto end_list_std = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_list_std = end_list_std - start_list_std;

    cout << "std::sort Linked List time: " << elapsed_list_std.count() << " seconds" << endl;

    return 0;
}
