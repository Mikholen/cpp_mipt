#include <iostream>
#include <cmath>

#define ARRAY_LEN 100

double get_min_value (double *arr, int length);
double get_max_value (double *arr, int length);
double get_average_value (double *arr, int length);
double get_std_value (double *arr, int length);

int main () {

    double arr[ARRAY_LEN];
    int i = 0;

    while (std::cin >> arr[i]) {
        
        i++;
    }
    
    std::cout << "\nMIN : " << get_min_value (arr, i) <<
                 "\nMAX : " << get_max_value (arr, i) <<
                 "\nAVG : " << get_average_value (arr, i) <<
                 "\nSTD : " << get_std_value (arr, i) << '\n';

    return 0;
}

double get_max_value (double *arr, int length) {

    double max_value = arr[0];

    for (int i = 1; i < length; i++) {

        if (arr[i] > max_value) {

            max_value = arr[i];
        }
    }

    return max_value;
}

double get_min_value (double *arr, int length) {

    double min_value = arr[0];

    for (int i = 1; i < length; i++) {

        if (arr[i] < min_value) {

            min_value = arr[i];
        }
    }

    return min_value;
}

double get_average_value (double *arr, int length) {

    double average = 0.0;

    for (int i = 0; i < length; i++) {

        average += arr[i];
    }

    average /= length;

    return average;
}

double get_std_value (double *arr, int length) {

    double average = get_average_value (arr, length);

    double std = 0.0;

    for (int i = 0; i < length; i++) {

        std += std::pow((arr[i] - average), 2);
    }

    std = std::sqrt(std / length);

    return std;
}