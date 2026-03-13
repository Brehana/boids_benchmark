#pragma once

struct VisualizerConfig
{
    int boid_count = 2000;
    int thread_count = 1;
    int window_width = 1000;
    int window_height = 1000;
};

int run_visualizer(const VisualizerConfig& config);