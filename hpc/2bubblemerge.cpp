#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// ============================
// Sequential Bubble Sort
// ============================
void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ============================
// Parallel Bubble Sort
// ============================
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ============================
// Merge Function
// ============================
void merge(vector<int>& arr, int l, int m, int r) {

    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < left.size()) {
        arr[k++] = left[i++];
    }

    while (j < right.size()) {
        arr[k++] = right[j++];
    }
}

// ============================
// Sequential Merge Sort
// ============================
void mergeSortSeq(vector<int>& arr, int l, int r) {

    if (l < r) {

        int m = (l + r) / 2;

        mergeSortSeq(arr, l, m);
        mergeSortSeq(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// ============================
// Parallel Merge Sort
// ============================
void mergeSortParallel(vector<int>& arr, int l, int r, int depth) {

    if (l < r) {

        int m = (l + r) / 2;

        if (depth <= 0) {

            mergeSortSeq(arr, l, m);
            mergeSortSeq(arr, m + 1, r);

        } else {

            #pragma omp parallel sections
            {

                #pragma omp section
                mergeSortParallel(arr, l, m, depth - 1);

                #pragma omp section
                mergeSortParallel(arr, m + 1, r, depth - 1);
            }
        }

        merge(arr, l, m, r);
    }
}

// ============================
// Display Array
// ============================
void printArray(vector<int>& arr) {

    for (int x : arr) {
        cout << x << " ";
    }

    cout << endl;
}

// ============================
// Main Function
// ============================
int main() {

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), temp;

    cout << "Enter elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    double start, end;

    // =====================================
    // Sequential Bubble Sort
    // =====================================
    temp = arr;

    start = omp_get_wtime();

    bubbleSortSeq(temp);

    end = omp_get_wtime();

    cout << "\nSequential Bubble Sort:\n";
    printArray(temp);

    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    // =====================================
    // Parallel Bubble Sort
    // =====================================
    temp = arr;

    start = omp_get_wtime();

    bubbleSortParallel(temp);

    end = omp_get_wtime();

    cout << "\nParallel Bubble Sort:\n";
    printArray(temp);

    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    // =====================================
    // Sequential Merge Sort
    // =====================================
    temp = arr;

    start = omp_get_wtime();

    mergeSortSeq(temp, 0, n - 1);

    end = omp_get_wtime();

    cout << "\nSequential Merge Sort:\n";
    printArray(temp);

    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    // =====================================
    // Parallel Merge Sort
    // =====================================
    temp = arr;

    start = omp_get_wtime();

    mergeSortParallel(temp, 0, n - 1, 4);

    end = omp_get_wtime();

    cout << "\nParallel Merge Sort:\n";
    printArray(temp);

    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    return 0;
}

// Enter number of elements: 8
// Enter elements:
// 64 34 25 12 22 11 90 5