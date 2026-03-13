#include "benchmark.h"
#include "visualizer.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    void print_usage(const char* program)
    {
        std::cout
            << "Usage: " << program << " [options]\n"
            << "Options:\n"
            << "  --boids N           Number of boids (default: 2000)\n"
            << "  --steps N           Number of simulation steps (default: 200)\n"
            << "  --repeats N         Number of repeated runs (default: 5)\n"
            << "  --threads a,b,c     Thread counts for benchmark mode (default: 1,2,4,8)\n"
            << "  --thread-count N    Thread count for visual mode (default: 1)\n"
            << "  --csv PATH          Write results to CSV\n"
            << "  --visual            Run interactive visualization mode\n"
            << "  --width N           Visualizer window width (default: 1000)\n"
            << "  --height N          Visualizer window height (default: 1000)\n"
            << "  --help              Show this help message\n";
    }

    std::vector<int> parse_thread_list(const std::string& s)
    {
        std::vector<int> values;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, ','))
        {
            int value = std::stoi(item);
            if (value > 0)
            {
                values.push_back(value);
            }
        }

        return values;
    }
}

int main(int argc, char* argv[])
{
    BenchmarkConfig benchmark_config;
    VisualizerConfig visualizer_config;

    bool visual_mode = false;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--help")
        {
            print_usage(argv[0]);
            return 0;
        }
        else if (arg == "--visual")
        {
            visual_mode = true;
        }
        else if (arg == "--boids" && i + 1 < argc)
        {
            int value = std::stoi(argv[++i]);
            benchmark_config.boid_count = value;
            visualizer_config.boid_count = value;
        }
        else if (arg == "--steps" && i + 1 < argc)
        {
            benchmark_config.steps = std::stoi(argv[++i]);
        }
        else if (arg == "--repeats" && i + 1 < argc)
        {
            benchmark_config.repeats = std::stoi(argv[++i]);
        }
        else if (arg == "--threads" && i + 1 < argc)
        {
            benchmark_config.thread_counts = parse_thread_list(argv[++i]);
        }
        else if (arg == "--thread-count" && i + 1 < argc)
        {
            visualizer_config.thread_count = std::stoi(argv[++i]);
        }
        else if (arg == "--csv" && i + 1 < argc)
        {
            benchmark_config.csv_path = argv[++i];
        }
        else if (arg == "--width" && i + 1 < argc)
        {
            visualizer_config.window_width = std::stoi(argv[++i]);
        }
        else if (arg == "--height" && i + 1 < argc)
        {
            visualizer_config.window_height = std::stoi(argv[++i]);
        }
        else
        {
            std::cerr << "Unknown or incomplete argument: " << arg << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    if (visual_mode)
    {
        return run_visualizer(visualizer_config);
    }

    auto results = run_benchmark(benchmark_config);
    print_summary(results);

    if (!benchmark_config.csv_path.empty())
    {
        if (write_csv(benchmark_config.csv_path, results))
        {
            std::cout << "\nWrote CSV: " << benchmark_config.csv_path << "\n";
        }
        else
        {
            std::cerr << "\nFailed to write CSV: " << benchmark_config.csv_path << "\n";
            return 1;
        }
    }

    return 0;
}