#include <iostream>
#include <cstddef>
#include <vector>

size_t get_collatz_sequence_len (unsigned long long int value, std::vector<size_t> &sequence_len);

int main () {

    unsigned long long int stop = 100;
    std::vector<size_t> sequence_len (stop, 0);
    size_t max_seq_len = 0;
    unsigned long long int value_max_seq_len = 1;

    for (unsigned long long int i = 1 ; i <= stop; i++) {

        sequence_len[i - 1] = get_collatz_sequence_len (i, sequence_len);

        if (max_seq_len < sequence_len[i - 1]) {

            max_seq_len = sequence_len[i - 1];
            value_max_seq_len = i;
        }
    }

    std::cout << "Start : " << value_max_seq_len 
              << ", Sequence length : " << max_seq_len << '\n';

    return 0;
}

size_t get_collatz_sequence_len (unsigned long long int value, std::vector<size_t> &sequence_len) {

    size_t size = 1;
    unsigned long long int initial_val = value;

    while (value != 1) {

        if (sequence_len[initial_val - 1] != 0) {

            size += sequence_len[initial_val - 1];
            break;
        }

        if (value % 2 == 0) {

            value /= 2;
        }

        else {

            value = value * 3 + 1;
        }

        size++;
    }

    return size;
}

