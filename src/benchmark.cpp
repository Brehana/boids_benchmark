#include "benchmark.h"
#include "boids.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>

namespace
{
    double run_one_case(int boid_count, int steps, int threads)
    {
        std::vector<Boid> current = create_boids(boid_count);
        std::vector<Boid> next(boid_count);

        auto start = std::chrono::steady_clock::now();

        for (int step = 0; step < steps; ++step)
        {
            if (threads == 1)
            {
                update_boids_single(current, next);
            }
            else
            {
                update_boids_multithreaded(current, next, threads);
            }

            current.swap(next);
        }

        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }
}

std::vector<BenchmarkResult> run_benchmark(const BenchmarkConfig& config)
{
    std::vector<BenchmarkResult> results;
    results.reserve(config.thread_counts.size() * config.repeats);

    std::map<int, double> baseline_by_repeat;

    for (int repeat = 0; repeat < config.repeats; ++repeat)
    {
        for (int threads : config.thread_counts)
        {
            double total_seconds = run_one_case(config.boid_count,
                                                config.steps,
                                                threads);

            BenchmarkResult result;
            result.boid_count = config.boid_count;
            result.steps = config.steps;
            result.threads = threads;
            result.repeat = repeat;
            result.total_seconds = total_seconds;
            result.avg_frame_ms = (total_seconds / config.steps) * 1000.0;
            result.throughput_boids_per_sec =
                (static_cast<double>(config.boid_count) * config.steps) / total_seconds;

            if (threads == 1)
            {
                baseline_by_repeat[repeat] = total_seconds;
            }

            double baseline = baseline_by_repeat[repeat];
            result.speedup = baseline / total_seconds;
            result.efficiency = result.speedup / static_cast<double>(threads);

            results.push_back(result);
        }
    }

    return results;
}

void print_summary(const std::vector<BenchmarkResult>& results)
{
    std::map<int, std::vector<BenchmarkResult>> grouped;
    for (const auto& r : results)
    {
        grouped[r.threads].push_back(r);
    }

    std::cout << "\nBenchmark summary\n";
    std::cout << "=================\n";
    std::cout << std::fixed << std::setprecision(3);

    for (const auto& [threads, group] : grouped)
    {
        double avg_frame_ms = 0.0;
        double avg_throughput = 0.0;
        double avg_speedup = 0.0;
        double avg_efficiency = 0.0;

        for (const auto& r : group)
        {
            avg_frame_ms += r.avg_frame_ms;
            avg_throughput += r.throughput_boids_per_sec;
            avg_speedup += r.speedup;
            avg_efficiency += r.efficiency;
        }

        double count = static_cast<double>(group.size());
        avg_frame_ms /= count;
        avg_throughput /= count;
        avg_speedup /= count;
        avg_efficiency /= count;

        std::cout << "Threads: " << threads
                  << " | avg frame: " << avg_frame_ms << " ms"
                  << " | throughput: " << avg_throughput
                  << " boids/sec"
                  << " | speedup: " << avg_speedup
                  << "x | efficiency: " << (avg_efficiency * 100.0)
                  << "%\n";
    }
}

bool write_csv(const std::string& path,
               const std::vector<BenchmarkResult>& results)
{
    std::ofstream out(path);
    if (!out)
    {
        return false;
    }

    out << "boid_count,steps,threads,repeat,total_seconds,avg_frame_ms,"
           "throughput_boids_per_sec,speedup,efficiency\n";

    out << std::fixed << std::setprecision(6);

    for (const auto& r : results)
    {
        out << r.boid_count << ','
            << r.steps << ','
            << r.threads << ','
            << r.repeat << ','
            << r.total_seconds << ','
            << r.avg_frame_ms << ','
            << r.throughput_boids_per_sec << ','
            << r.speedup << ','
            << r.efficiency << '\n';
    }

    return true;
}