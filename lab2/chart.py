import pandas as pd

import matplotlib.pyplot as plt

# Read the data
data = pd.read_csv('lab2/dados2.csv', skipinitialspace=True)

# Convert columns to appropriate types
data['num_threads'] = data['num_threads'].astype(int)
data['time'] = data['time'].astype(float)
data['num_zeros'] = data['num_zeros'].astype(int)

# Plot time vs num_zeros for each num_threads
plt.figure(figsize=(10, 6))
for num_threads in data['num_threads'].unique():
    subset = data[data['num_threads'] == num_threads]
    plt.plot(subset['num_zeros'], subset['time'], label=f'{num_threads} threads')

plt.xlabel('Number of Zeros')
plt.ylabel('Time (s)')
plt.title('Time vs Number of Zeros')
plt.legend()
plt.grid(True)
plt.savefig('lab2/time_vs_num_zeros.png')
plt.show()

# Calculate speedup and efficiency
single_thread_data = data[data['num_threads'] == 1].set_index('num_zeros')['time']
multi_thread_data = data[data['num_threads'] == 8].set_index('num_zeros')['time']

speedup = single_thread_data / multi_thread_data
efficiency = speedup / 8

# Plot speedup and efficiency
plt.figure(figsize=(10, 6))
plt.plot(speedup.index, speedup, label='Speedup')
plt.plot(efficiency.index, efficiency, label='Efficiency')

plt.xlabel('Number of Zeros')
plt.ylabel('Speedup / Efficiency')
plt.title('Speedup and Efficiency vs Number of Zeros')
plt.legend()
plt.grid(True)
plt.savefig('lab2/speedup_efficiency_vs_num_zeros.png')
plt.show()