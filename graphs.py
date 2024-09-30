import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# File paths for the three cases
files = ["output/Case_1.csv", "output/Case_2.csv", "output/Case_3.csv"]

# Create separate graphs for each case
for i, file in enumerate(files):
    # Read the data from the CSV file
    df = pd.read_csv(file)

    # Filter the necessary columns
    test_types = df['Test Type']
    avg_runtimes = df[' Average Runtime (ms)']

    # Prepare the thread counts and associated runtimes
    thread_counts = [1, 2, 4, 8]
    serial_times = [0] * len(thread_counts)
    mutex_times = [0] * len(thread_counts)
    rwlock_times = [0] * len(thread_counts)

    for j, test in enumerate(test_types):
        if 'T1' in test:
            thread_counts[0] = 1
            if 'Mutex' in test:
                mutex_times[0] = avg_runtimes[j]
            elif 'RW Lock' in test:
                rwlock_times[0] = avg_runtimes[j]
        elif 'T2' in test:
            thread_counts[1] = 2
            if 'Mutex' in test:
                mutex_times[1] = avg_runtimes[j]
            elif 'RW Lock' in test:
                rwlock_times[1] = avg_runtimes[j]
        elif 'T4' in test:
            thread_counts[2] = 4
            if 'Mutex' in test:
                mutex_times[2] = avg_runtimes[j]
            elif 'RW Lock' in test:
                rwlock_times[2] = avg_runtimes[j]
        elif 'T8' in test:
            thread_counts[3] = 8
            if 'Mutex' in test:
                mutex_times[3] = avg_runtimes[j]
            elif 'RW Lock' in test:
                rwlock_times[3] = avg_runtimes[j]
        elif 'Serial' in test:
            serial_times[0] = avg_runtimes[j]  # Serial time is only for T1

    # Plotting the data
    bar_width = 0.25
    x = np.arange(len(thread_counts))  # Thread counts as x-axis

    # Create the figure
    plt.figure(figsize=(10, 6))

    # Plot bars for thread count 1 (three bars: Serial, Mutex, RW Lock)
    plt.bar(x[0], serial_times[0], width=bar_width,
            label='Serial', color='blue')
    plt.bar(x[0] + bar_width, mutex_times[0],
            width=bar_width, label='Mutex', color='orange')
    plt.bar(x[0] + 2 * bar_width, rwlock_times[0],
            width=bar_width, label='RW Lock', color='gray')

    # Plot bars for thread counts 2, 4, and 8 (two bars: Mutex, RW Lock)
    plt.bar(x[1] + bar_width, mutex_times[1], width=bar_width, color='orange')
    plt.bar(x[1] + 2 * bar_width, rwlock_times[1],
            width=bar_width, color='gray')

    plt.bar(x[2] + bar_width, mutex_times[2], width=bar_width, color='orange')
    plt.bar(x[2] + 2 * bar_width, rwlock_times[2],
            width=bar_width, color='gray')

    plt.bar(x[3] + bar_width, mutex_times[3], width=bar_width, color='orange')
    plt.bar(x[3] + 2 * bar_width, rwlock_times[3],
            width=bar_width, color='gray')

    # Add titles and labels
    plt.xlabel('Thread Count')
    plt.ylabel('Execution Time (ms)')
    plt.title(f'Execution Time Comparison - Case {i + 1}')
    # Correct position of x-ticks for clarity
    plt.xticks(x + bar_width, thread_counts)
    plt.legend()

    # Show the plot
    plt.tight_layout()
    plt.show()
