# /// script
# requires-python = ">=3.12"
# dependencies = [
#   "matplotlib",
# ]
# ///

import os
import subprocess
import time
import matplotlib.pyplot as plt

# Seed to use to generate the files
seed = 123

# Function to generate the grid using Rust binary
def generate_grid(row, col, seed):
    # Run the generator
    start_time = time.time()
    subprocess.run(["./generator", str(row), str(col), str(seed)])
    end_time = time.time()

    # Return the execution time (in seconds)
    return end_time - start_time
    

# Function to run the program and measure the execution time
def benchmark_program(filename, param):
    # Run the program with the given parameter and capture the execution time
    start_time = time.time()
    subprocess.run(["./figsearch", param, filename])
    end_time = time.time()

    # Return the execution time (in seconds)
    return end_time - start_time

# Main function to generate files of increasing sizes and benchmark
def benchmark():
    # Ensure target directory exists
    os.makedirs("testgen", exist_ok=True)

    # List of different grid sizes (larger sizes to slow down the C program)
    sizes = [(500, 500), (1000, 1000), (2000, 2000), (3000, 3000), (4000, 4000), (5000, 5000)]  # Larger grids

    # Time dictionaries for each parameter
    times_square = []
    times_hline = []
    times_vline = []

    # Generate the files and benchmark the C program
    for row, col in sizes:
        filename = f"testgen/gen_{row}_{col}_{seed}.txt"
        generate_grid(row, col, seed)
        print(f"Benchmarking {filename}...")

        # Benchmark for 'square'
        execution_time_square = benchmark_program(filename, "square")
        print(f"Execution time for {filename} (square): {execution_time_square:.4f} seconds")
        times_square.append(execution_time_square)

        # Benchmark for 'hline'
        execution_time_hline = benchmark_program(filename, "hline")
        print(f"Execution time for {filename} (hline): {execution_time_hline:.4f} seconds")
        times_hline.append(execution_time_hline)

        # Benchmark for 'vline'
        execution_time_vline = benchmark_program(filename, "vline")
        print(f"Execution time for {filename} (vline): {execution_time_vline:.4f} seconds")
        times_vline.append(execution_time_vline)

        # Delete the file after use
        os.remove(filename)
        print(f"Deleted {filename}")

    # Plot the results
    grid_sizes = [f"{row}x{col}" for row, col in sizes]

    plt.figure(figsize=(10, 6))

    # Plot all three parameters on the same plot
    plt.plot(grid_sizes, times_square, label='square', marker='o')
    plt.plot(grid_sizes, times_hline, label='hline', marker='x')
    plt.plot(grid_sizes, times_vline, label='vline', marker='^')

    plt.xlabel('Grid Size (row x col)')
    plt.ylabel('Execution Time (seconds)')
    plt.title('Performance of ./figsearch for Different Parameters')
    plt.xticks(rotation=45)
    plt.grid(True)
    plt.legend()

    # Save the plot as a PNG image
    plt.tight_layout()
    plt.savefig("benchmark_plot.png")
    print("Plot saved as benchmark_plot.png")

if __name__ == "__main__":
    benchmark()