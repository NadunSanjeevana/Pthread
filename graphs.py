import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# File paths
files = ["output/Case_1.csv", "output/Case_2.csv", "output/Case_3.csv"]

# Initialize a list to hold DataFrames
dfs = []

# Read each CSV file into a DataFrame
for file in files:
    df = pd.read_csv(file)
    dfs.append(df)

# Create separate graphs for each case
for i, df in enumerate(dfs):
    combined_data = {}

    # Process each DataFrame
    for index, row in df.iterrows():
        test_type = row['Test Type']
        avg_runtime = row[' Average Runtime (ms)']

        # Extract the number of threads for Mutex and RW Lock
        if "Mutex" in test_type:
            # Get the number of threads (T1, T2, etc.)
            threads = test_type.split()[-1]
            test_name = f'Mutex {threads}'
        elif "RW Lock" in test_type:
            # Get the number of threads (T1, T2, etc.)
            threads = test_type.split()[-1]
            test_name = f'RW Lock {threads}'
        else:
            threads = '1'  # Serial case
            test_name = 'Serial'

        # Store the runtime in the combined_data dictionary
        if test_name not in combined_data:
            combined_data[test_name] = [avg_runtime]
        else:
            combined_data[test_name].append(avg_runtime)

    # Prepare data for plotting
    labels = list(combined_data.keys())
    bar_data = [combined_data[label] for label in labels]

    # Set the number of bars and their positions
    bar_width = 0.25
    x = np.arange(len(bar_data[0]))  # Number of samples per test type

    # Create a multibar chart for the current case
    plt.figure(figsize=(12, 6))
    for j, data in enumerate(bar_data):
        plt.bar(x + (j - 1) * bar_width, data,
                width=bar_width, label=labels[j])

    # Adding labels and title
    plt.xlabel('Number of Threads')
    plt.ylabel('Average Runtime (ms)')
    plt.title(f'Performance Comparison for Case {i + 1}')
    # Assuming consistent thread counts
    plt.xticks(x, [f'T{i + 1}' for i in range(len(bar_data[0]))])
    plt.legend()

    # Show the plot
    plt.tight_layout()
    plt.show()
