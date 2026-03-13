Multithreaded Boids Simulation Benchmark

A C++ systems performance project that benchmarks a multithreaded implementation of the classic Boids flocking simulation using std::thread.

The project includes:

A headless benchmarking mode for measuring performance scaling

A real-time visualization mode for observing simulation behavior

CSV export for benchmark results

Python scripts for generating performance graphs

This project is designed to demonstrate multithreading, workload design, and performance analysis on modern CPUs.

Demo
Visualization Mode

Run the simulation interactively and observe the boids in real time.

The overlay shows:

Thread count

Live FPS

Example:

./boids --visual --boids 2000 --thread-count 4
Features
Multithreaded Simulation

Parallel boid updates using std::thread

Work partitioned across boid ranges

Deterministic double-buffered simulation state

Performance Benchmarking

Thread scaling tests

Repeated runs for stable measurements

Reports:

Average frame time

Throughput (boids/sec)

Parallel speedup

Parallel efficiency

Data Analysis

CSV export of benchmark results

Python script for generating performance graphs

Visualization Mode

Real-time boid simulation

Adjustable boid counts and thread counts

Live FPS overlay

Runs indefinitely until the window closes

Repository Structure
boids_benchmark/
│
├── CMakeLists.txt
├── README.md
│
├── src/
│   ├── main.cpp
│   ├── boids.cpp
│   ├── boids.h
│   ├── benchmark.cpp
│   ├── benchmark.h
│   ├── visualizer.cpp
│   └── visualizer.h
│
├── scripts/
│   └── plot_results.py
│
├── data/        # CSV benchmark results
├── plots/       # Generated graphs
└── build/
Requirements

Linux environment with:

C++17 compiler

CMake ≥ 3.10

SDL2

SDL2_ttf

Python 3

matplotlib

Install dependencies (Ubuntu/Debian):

sudo apt update
sudo apt install \
    build-essential \
    cmake \
    libsdl2-dev \
    libsdl2-ttf-dev \
    python3 \
    python3-matplotlib \
    pkg-config
Building

Clone the repo and build using CMake.

git clone <repo-url>
cd boids_benchmark

mkdir build
cd build

cmake ..
make -j

This produces the executable:

./boids
Benchmark Mode

Runs a headless simulation and measures performance.

Example:

./boids \
  --boids 4000 \
  --steps 300 \
  --repeats 5 \
  --threads 1,2,4,8 \
  --csv ../data/results.csv

Example output:

Benchmark summary
=================
Threads: 1 | avg frame: 18.4 ms | throughput: 217k boids/sec | speedup: 1.0x
Threads: 2 | avg frame: 10.1 ms | throughput: 395k boids/sec | speedup: 1.8x
Threads: 4 | avg frame: 6.2 ms  | throughput: 641k boids/sec | speedup: 2.9x
Threads: 8 | avg frame: 5.8 ms  | throughput: 682k boids/sec | speedup: 3.1x

Metrics reported:

Metric	Description
Frame Time	Average simulation step time
Throughput	Boids simulated per second
Speedup	Relative to single-thread performance
Efficiency	Speedup / thread count
Visualization Mode

Launch an interactive simulation window.

Example:

./boids --visual --boids 2000 --thread-count 4

Options:

Option	Description
--visual	Run real-time visualization
--boids	Number of boids
--thread-count	Threads used for simulation
--width	Window width
--height	Window height

Notes:

Visualization runs indefinitely

--steps and CSV output are ignored in this mode

Generating Performance Graphs

After running benchmarks with CSV output:

data/results.csv

Generate graphs:

python3 scripts/plot_results.py data/results.csv

Output graphs appear in:

plots/

Graphs include:

Frame time vs thread count

Throughput vs thread count

Speedup scaling

Parallel efficiency

Simulation Model

This project implements a simplified boids model using the cohesion rule:

Each boid steers toward the average position of nearby neighbors.

Complexity:

O(N²)

This intentionally heavy workload makes it useful for CPU scaling experiments.

The simulation uses:

Double-buffered state updates

Parallel partitioning across boid ranges

Read-only shared state per frame

This ensures deterministic updates and avoids data races.

Example Benchmark Workflow

Run benchmark:

./boids \
  --boids 8000 \
  --steps 200 \
  --repeats 5 \
  --threads 1,2,4,8,16 \
  --csv ../data/scaling.csv

Generate plots:

python3 scripts/plot_results.py data/scaling.csv

Analyze scaling behavior:

diminishing returns after core count

thread overhead

memory bandwidth limits

Future Improvements

Potential extensions:

Simulation

alignment and separation rules

spatial partitioning grid

SIMD vectorization

SoA memory layout

Performance

thread pools

work stealing

NUMA-aware partitioning

Visualization

directional boid triangles

keyboard controls (pause/reset)

real-time performance graphs

Educational Value

This project demonstrates key systems topics:

multithreaded workload design

CPU scaling analysis

deterministic parallel simulation

benchmark methodology

visualization of parallel systems

License

MIT License