# Multithreaded Boids Benchmark --- User Guide

This project is a **C++ boids simulation benchmark** with two modes:

1.  **Benchmark mode** --- runs the simulation headlessly and prints
    performance results
2.  **Visualization mode** --- opens a window and shows the boids moving
    in real time

This guide walks you through using the program from scratch on Linux.

[<video src="preview/preview.webm" width="998" height="701" controls></video>](https://github.com/user-attachments/assets/968ed43d-d87b-445b-9366-45e96eb6007b)

------------------------------------------------------------------------

# 1. Install Dependencies

You need:

-   a C++ compiler
-   CMake
-   pkg-config
-   SDL2
-   SDL2_ttf
-   Python 3
-   matplotlib

## Ubuntu / Debian

``` bash
sudo apt update
sudo apt install \
    build-essential \
    cmake \
    pkg-config \
    libsdl2-dev \
    libsdl2-ttf-dev \
    python3 \
    python3-matplotlib
```
------------------------------------------------------------------------

# 3. Build the Program

Navigate to build directory:

``` bash
cd build
```

Configure the project with CMake:

``` bash
cmake ..
```

Compile:

``` bash
make -j
```

This will create the executable:

``` bash
./boids
```

------------------------------------------------------------------------

# 4. Confirm the Program Runs

From inside the `build/` directory:

``` bash
./boids --help
```

------------------------------------------------------------------------

# 5. Run Benchmark Mode

Benchmark mode runs the simulation without graphics and measures
performance.

Example:

``` bash
./boids --boids 2000 --steps 200 --repeats 3 --threads 1,2,4
```

Arguments:

-   `--boids` number of boids
-   `--steps` number of simulation steps
-   `--repeats` number of repeated benchmark runs
-   `--threads` comma‑separated thread counts to test

------------------------------------------------------------------------

# 6. Save Benchmark Results to CSV

Example:

``` bash
./boids --boids 4000 --steps 300 --repeats 5 --threads 1,2,4,8 --csv ../data/results.csv
```

This creates:

    data/results.csv

------------------------------------------------------------------------

# 7. Generate Performance Graphs

Return to the project root:

``` bash
cd ..
```

Run the plotting script:

``` bash
python3 scripts/plot_results.py data/results.csv
```

Graphs will be generated in:

    plots/

------------------------------------------------------------------------

# 8. Run Visualization Mode

Visualization mode runs the simulation in a window.

Example:

``` bash
cd build
./boids --visual --boids 2000 --thread-count 4
```

Live metrics displayed:

-   Thread count
-   FPS

------------------------------------------------------------------------

# 9. Customize Visualization

Example:

``` bash
./boids --visual --boids 5000 --thread-count 8 --width 1280 --height 720
```

Arguments:

  ------------------ -----------------------------
  `--visual`         Enable visualization
  `--boids`          Number of boids
  `--thread-count`   Threads used for simulation
  `--width`          Window width
  `--height`         Window height

Visualization runs until the window is closed.

------------------------------------------------------------------------

# License

MIT License
