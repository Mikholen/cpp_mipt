#include <iostream>
#include <cmath>

double get_min_value(double *arr, int length);
double get_max_value(double *arr, int length);
double get_average_value(double *arr, int length);
double get_std_value(double *arr, int length);

int main() {

    int capacity = 10;
    int size = 0;
    double* arr = new double[capacity];

    double value;

    while (std::cin >> value) {
        
        if (size == capacity) {
            
            capacity *= 2;
            double* new_arr = new double[capacity];

            for (int i = 0; i < size; i++) {

                new_arr[i] = arr[i];
            }

            delete[] arr;

            arr = new_arr;
        }

        arr[size++] = value;
    }

    if (size > 0) {

        std::cout << "\nMIN : " << get_min_value(arr, size)
                  << "\nMAX : " << get_max_value(arr, size)
                  << "\nAVG : " << get_average_value(arr, size)
                  << "\nSTD : " << get_std_value(arr, size) << '\n';
    } 
    
    else {

        std::cout << "No input data.\n";
    }

    delete[] arr;

    return 0;
}

double get_max_value(double *arr, int length) {

    double max_value = arr[0];

    for (int i = 1; i < length; i++) {

        if (arr[i] > max_value) {

            max_value = arr[i];
        }
    }

    return max_value;
}

double get_min_value(double *arr, int length) {

    double min_value = arr[0];

    for (int i = 1; i < length; i++) {

        if (arr[i] < min_value) {

            min_value = arr[i];
        }
    }

    return min_value;
}

double get_average_value(double *arr, int length) {

    double average = 0.0;

    for (int i = 0; i < length; i++) {

        average += arr[i];
    }

    average /= length;

    return average;
}

double get_std_value(double *arr, int length) {

    double average = get_average_value(arr, length);

    double std = 0.0;

    for (int i = 0; i < length; i++) {

        std += std::pow((arr[i] - average), 2);
    }

    std = std::sqrt(std / length);

    return std;
}
