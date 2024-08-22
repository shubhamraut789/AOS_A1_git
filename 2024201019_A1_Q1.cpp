#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

//Function to reverse chunk size char array
void reverseStore(char* arr, long long size) {
    char temp;
    for (long long i = 0; i < size / 2; ++i) {
        temp = arr[i];
        arr[i] = arr[size - i - 1];
        arr[size - i - 1] = temp;
    }
}

//Function to give progess percentage in real time on terminal
void ProgressPercentage(long long curr, long long total) {
    int percent_ = (int)((double)curr / total * 100);
    cout << "\rPercentage file written: " << percent_ << "%";
}


//Function to check Chunk_size in the bound
void ChunkMultiply(long long Chunk__size){
    Chunk__size *= 2;
}

int main(int argc, char* argv[]) {

    int flag = atoi(argv[2]);

    if(flag != 0 && flag != 1){
        perror("Flag entered is not 0 or 1");
        return 1;
    }

    if ((flag == 0 && argc != 3) || (flag == 1 && argc != 5)) {
        perror("Error as input format not as expected");
        return 1;
    }

    const char* input_path = argv[1];
    
    long long start_index = 0, end_index = 0;

    if (flag == 1) {
        start_index = stoll(argv[3]);
        end_index = stoll(argv[4]);
    }

    int input_fd = open(input_path, O_RDONLY);
    if (input_fd < 0) {
        perror("Error opening input file");
        return 1;
    }

    const char* folder_dir = "Assignment1";

    int dir_fd = mkdir(folder_dir, S_IRWXU);

    string out_file;
    if(flag == 0) out_file = "Assignment1/0_" + (string)argv[1];
    else if(flag == 1) out_file = "Assignment1/1_" + (string)argv[1];

    char c[out_file.length() + 1];
    
    for(int i = 0; i < out_file.length(); i++)
        c[i] = out_file[i];

    c[out_file.length()] = '\0';

    int output_fd = open(c, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    if (output_fd < 0) {
        perror("Error while opening the output file");
        return 1;
    }

    long long file_size = lseek(input_fd, 0, SEEK_END);

    if (file_size < 0) {
        perror("Error while getting the file size");
        return 1;
    }

    lseek(input_fd, 0, SEEK_SET);

    long long chunk_size = 10000;

    char org[chunk_size];

    if(flag == 0){
        // Reverse the entire file in flag 0
        long long window = file_size, window_read;

        while (window > 0) {
            if (window < chunk_size) {
                window_read = window;
            } else {
                window_read = chunk_size;
            }

            lseek(input_fd, window - window_read, SEEK_SET);

            long long Chunk_read = read(input_fd, org, window_read);

            if (Chunk_read < 0) {
                perror("Error while reading the file");
                return 1;
            }

            reverseStore(org, Chunk_read);

            long long Chunk_wrote = write(output_fd, org, Chunk_read);

            if (Chunk_wrote < 0) {
                perror("Error while writing to output file");
                return 1;
            }

            window -= window_read;
            ProgressPercentage(file_size - window, file_size);
            
        }
    }
    else if(flag == 1){
        // Reverse from beginning to start_index
        long long window = start_index, window_read;

        while (window > 0) {
            if (window < chunk_size) {
                window_read = window;
            } else {
                window_read = chunk_size;
            }

            lseek(input_fd, window - window_read, SEEK_SET);

            long long Chunk_read = read(input_fd, org, window_read);

            if (Chunk_read < 0) {
                perror("Error while reading the file");
                return 1;
            }

            reverseStore(org, Chunk_read);

            long long Chunk_wrote = write(output_fd, org, Chunk_read);

            if (Chunk_wrote < 0) {
                perror("Error while writing to output file");
                return 1;
            }

            window -= window_read;
            ProgressPercentage(start_index - window, file_size);
        }

        // Write the middle section without modification
        long long middle_section_size = end_index - start_index+1;
        lseek(input_fd, start_index, SEEK_SET);

        while (middle_section_size > 0) {
            if (middle_section_size < chunk_size) {
                window_read = middle_section_size;
            } else {
                window_read = chunk_size;
            }

            long long Chunk_read = read(input_fd, org, window_read);

            if (Chunk_read < 0) {
                perror("Error while reading the middle section");
                return 1;
            }

            long long Chunk_wrote = write(output_fd, org, Chunk_read);

            if (Chunk_wrote < 0) {
                perror("Error while writing the middle section");
                return 1;
            }

            middle_section_size -= window_read;
            ProgressPercentage(file_size - middle_section_size, file_size);
        }

        // Reverse from end_index to end of the file
        long long end_section_size = file_size - end_index-1;
        window = end_section_size;

        while (window > 0) {
            if (window < chunk_size) {
                window_read = window;
            } else {
                window_read = chunk_size;
            }

            lseek(input_fd, file_size - window, SEEK_SET);

            long long Chunk_read = read(input_fd, org, window_read);

            if (Chunk_read < 0) {
                perror("Error while reading the file");
                return 1;
            }

            reverseStore(org, Chunk_read);

            long long Chunk_wrote = write(output_fd, org, Chunk_read);

            if (Chunk_wrote < 0) {
                perror("Error while writing to output file");
                return 1;
            }

            window -= window_read;
            ProgressPercentage(file_size - window, file_size);
        }
    }

    cout << endl; // Newline after progress

    return 0;
}