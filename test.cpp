#include <vector>
#include<iostream>
using namespace std;




int *MultiplyArrayByTwo(int arr[], int size){
    for (int i = 0; i < size; ++i) {
        arr[i] = i;
    }
    return arr;
}

int main(){
    constexpr int size = 10;
    int c_array[10];

    int *ptr = MultiplyArrayByTwo(c_array, size);



    cout<<*(ptr+1)<<endl;

    cout<<*(ptr+4)<<endl;

    cout << "array = [ ";
    for (int i = 0; i < size; ++i) {
        cout << ptr[i] << ", ";
    }
    cout << "]" << endl;

    return EXIT_SUCCESS;
}