#pragma once

#include <string>
#include <vector>

struct BenchmarkConfig
{
    int boid_count = 2000;
    int steps = 200;
    int repeats = 5;
    std::vector<int> thread_counts = {1, 2, 4, 8};
    std::string csv_path;
};

struct BenchmarkResult
{
    int boid_count = 0;
    int steps = 0;
    int threads = 0;
    int repeat = 0;
    double total_seconds = 0.0;
    double avg_frame_ms = 0.0;
    double throughput_boids_per_sec = 0.0;
    double speedup = 0.0;
    double efficiency = 0.0;
};

std::vector<BenchmarkResult> run_benchmark(const BenchmarkConfig& config);

void print_summary(const std::vector<BenchmarkResult>& results);

bool write_csv(const std::string& path,
               const std::vector<BenchmarkResult>& results);