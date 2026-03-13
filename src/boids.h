#pragma once

#include <vector>

struct Boid
{
    float x, y;
    float vx, vy;
};

std::vector<Boid> create_boids(int count);

void update_boids_single(const std::vector<Boid>& current,
                         std::vector<Boid>& next);

void update_boids_multithreaded(const std::vector<Boid>& current,
                                std::vector<Boid>& next,
                                int thread_count);