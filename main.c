#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "linked_list.h"
#include "serial.h"
#include "mutex.h"
#include "rw_lock.h"

// Define confidence level and accuracy for sample size calculation
#define CONFIDENCE_LEVEL 1.96  // 95% confidence level (Z-value for 95%)
#define ACCURACY 0.05          // ±5% accuracy

void run_tests_for_case(int case_num, const char* filename);
unsigned long* run_preliminary_tests(int case_num);
int calculate_sample_size(unsigned long* runtime_arr, int preliminary_samples);
void calculate_statistics(int case_num, int sample_size, const char* label, FILE* fp);

int main() {
    run_tests_for_case(1, "output/Case_1.csv");
    run_tests_for_case(2, "output/Case_2.csv");
    run_tests_for_case(3, "output/Case_3.csv");

    printf("Completed all tests.\n");
    return 0;
}

void run_tests_for_case(int case_num, const char* filename) {
    unsigned long* serial_runtime_arr, *mut_t1_runtime_arr, *mut_t2_runtime_arr, *mut_t4_runtime_arr, *mut_t8_runtime_arr;
    unsigned long* rw_t1_runtime_arr, *rw_t2_runtime_arr, *rw_t4_runtime_arr, *rw_t8_runtime_arr;
    
    FILE* fp = fopen(filename, "w+");
    if (fp == NULL) {
        perror("Failed to open the file.");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "Test Type, Average Runtime (ms), Standard Deviation (ms), Number of Samples\n");

    // Run preliminary tests to estimate standard deviation
    serial_runtime_arr = run_preliminary_tests(1);
    mut_t1_runtime_arr = run_preliminary_tests(2);
    mut_t2_runtime_arr = run_preliminary_tests(3);
    mut_t4_runtime_arr = run_preliminary_tests(4);
    mut_t8_runtime_arr = run_preliminary_tests(5);
    rw_t1_runtime_arr = run_preliminary_tests(6);
    rw_t2_runtime_arr = run_preliminary_tests(7);
    rw_t4_runtime_arr = run_preliminary_tests(8);
    rw_t8_runtime_arr = run_preliminary_tests(9);

    // Calculate sample size based on preliminary standard deviation
    int serial_sample_size = calculate_sample_size(serial_runtime_arr, 100);
    int mut_t1_sample_size = calculate_sample_size(mut_t1_runtime_arr, 100);
    int mut_t2_sample_size = calculate_sample_size(mut_t2_runtime_arr, 100);
    int mut_t4_sample_size = calculate_sample_size(mut_t4_runtime_arr, 100);
    int mut_t8_sample_size = calculate_sample_size(mut_t8_runtime_arr, 100);
    int rw_t1_sample_size = calculate_sample_size(rw_t1_runtime_arr, 100);
    int rw_t2_sample_size = calculate_sample_size(rw_t2_runtime_arr, 100);
    int rw_t4_sample_size = calculate_sample_size(rw_t4_runtime_arr, 100);
    int rw_t8_sample_size = calculate_sample_size(rw_t8_runtime_arr, 100);

    // Re-run tests with calculated sample size and output results to CSV
    calculate_statistics(1, serial_sample_size, "Serial", fp);
    calculate_statistics( 2, mut_t1_sample_size, "Mutex T1", fp);
    calculate_statistics( 3, mut_t2_sample_size, "Mutex T2", fp);
    calculate_statistics( 4, mut_t4_sample_size, "Mutex T4", fp);
    calculate_statistics( 5, mut_t8_sample_size, "Mutex T8", fp);
    calculate_statistics( 6, rw_t1_sample_size, "RW Lock T1", fp);
    calculate_statistics( 7, rw_t2_sample_size, "RW Lock T2", fp);
    calculate_statistics( 8, rw_t4_sample_size, "RW Lock T4", fp);
    calculate_statistics( 9, rw_t8_sample_size, "RW Lock T8", fp);

    fclose(fp);
}

// Run preliminary tests to get runtimes (preliminary 100 runs for estimation)
unsigned long* run_preliminary_tests(int case_num) {
    unsigned long* runtime_arr = (unsigned long*)calloc(100, sizeof(unsigned long));
    if (runtime_arr == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < 100; i++) {
        switch (case_num) {
            case 1:
                runtime_arr[i] = test_serial_run(case_num);
                break;
            case 2:
                runtime_arr[i] = test_mutex_run(case_num, 1);
                break;
            case 3:
                runtime_arr[i] = test_mutex_run(case_num, 2);
                break;
            case 4:
                runtime_arr[i] = test_mutex_run(case_num, 4);
                break;
            case 5:
                runtime_arr[i] = test_mutex_run(case_num, 8);
                break;
            case 6:
                runtime_arr[i] = test_rw_lock_run(case_num, 1);
                break;
            case 7:
                runtime_arr[i] = test_rw_lock_run(case_num, 2);
                break;
            case 8:
                runtime_arr[i] = test_rw_lock_run(case_num, 4);
                break;
            case 9:
                runtime_arr[i] = test_rw_lock_run(case_num, 8);
                break;
            default:
                printf("Invalid case number_run_prem: %d\n", case_num);

                runtime_arr[i] = 0; // Assign a default value to avoid garbage
                break;
        }
    }
    
    return runtime_arr;
}

// Calculate the required sample size for ±5% accuracy and 95% confidence level
int calculate_sample_size(unsigned long* runtime_arr, int preliminary_samples) {
    unsigned long sum = 0, sum_of_squares = 0;
    double avg = 0, stddev = 0, margin_of_error = 0;

    // Calculate average and standard deviation
    for (int i = 0; i < preliminary_samples; i++) {
        sum += runtime_arr[i];
    }
    
    avg = sum / (double)preliminary_samples;

    for (int i = 0; i < preliminary_samples; i++) {
        sum_of_squares += pow(runtime_arr[i] - avg, 2);
    }
    
    stddev = sqrt(sum_of_squares / (double)preliminary_samples);

    // Margin of error formula for 95% confidence level
    margin_of_error = avg * ACCURACY;
    
    // Calculate required sample size
    int sample_size = (int)ceil(pow((CONFIDENCE_LEVEL * stddev) / margin_of_error, 2));

    return sample_size < 30 ? 30 : sample_size; // Ensure minimum sample size of 30
}

// Re-run tests for the final sample size and calculate statistics
void calculate_statistics(int case_num, int sample_size, const char* label, FILE* fp) {
    unsigned long* final_runtime_arr = (unsigned long*)malloc(sample_size * sizeof(unsigned long));
    unsigned long sum = 0, sum_of_squares = 0;
    double avg = 0, stddev = 0;

    if (final_runtime_arr == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Re-run the tests for the given sample size
    for (int i = 0; i < sample_size; i++) {
        switch (case_num) {
            case 1:
                final_runtime_arr[i] = test_serial_run(case_num);
                break;
            case 2:
                final_runtime_arr[i] = test_mutex_run(case_num, 1);
                break;
            case 3:
                final_runtime_arr[i] = test_mutex_run(case_num, 2);
                break;
            case 4:
                final_runtime_arr[i] = test_mutex_run(case_num, 4);
                break;
            case 5:
                final_runtime_arr[i] = test_mutex_run(case_num, 8);
                break;
            case 6:
                final_runtime_arr[i] = test_rw_lock_run(case_num, 1);
                break;
            case 7:
                final_runtime_arr[i] = test_rw_lock_run(case_num, 2);
                break;
            case 8:
                final_runtime_arr[i] = test_rw_lock_run(case_num, 4);
                break;
            case 9:
                final_runtime_arr[i] = test_rw_lock_run(case_num, 8);
                break;
            default:
                printf("Invalid case number_cal_stat: %d\n", case_num);
                final_runtime_arr[i] = 0; // Assign a default value to avoid garbage
                break;
        }
    }

    // Calculate average and standard deviation
    for (int i = 0; i < sample_size; i++) {
        sum += final_runtime_arr[i];
    }
    
    avg = sum / (double)sample_size;

    for (int i = 0; i < sample_size; i++) {
        sum_of_squares += pow(final_runtime_arr[i] - avg, 2);
    }

    stddev = sqrt(sum_of_squares / (double)(sample_size - 1)); // Use (n-1) for sample standard deviation

    // Print to file in microseconds
    fprintf(fp, "%s, %lf, %lf, %d\n", label, avg / 1000.0, stddev / 1000.0, sample_size);

    free(final_runtime_arr);
}









