#include <bits/stdc++.h>

using namespace std;

void heapify(vector<int> &arr, int i) {
    int n = arr.size();
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, largest);
    }
}

void makeHeap(vector<int> &arr){
    for(int i = arr.size()/2 - 1; i >= 0; i--){
        heapify(arr, i);
    }
}

int main(){
    vector<int> arr = {2, 6, 5, 1, 3, 4};
    makeHeap(arr);
    cout << format("{}\n", arr);
    return 0;
}