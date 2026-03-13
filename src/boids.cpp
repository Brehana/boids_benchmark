#include "boids.h"

#include <cmath>
#include <random>
#include <thread>
#include <algorithm>

namespace
{
    constexpr float NEIGHBOR_RADIUS = 50.0f;
    constexpr float COHESION_FACTOR = 0.001f;
    constexpr float MAX_SPEED = 2.0f;
    constexpr float WORLD_SIZE = 1000.0f;

    void clamp_speed(float& vx, float& vy)
    {
        float speed = std::sqrt(vx * vx + vy * vy);
        if (speed > MAX_SPEED && speed > 0.0f)
        {
            float scale = MAX_SPEED / speed;
            vx *= scale;
            vy *= scale;
        }
    }

    void wrap_position(float& x, float& y)
    {
        if (x < 0.0f) x += WORLD_SIZE;
        if (x >= WORLD_SIZE) x -= WORLD_SIZE;
        if (y < 0.0f) y += WORLD_SIZE;
        if (y >= WORLD_SIZE) y -= WORLD_SIZE;
    }

    void update_range(const std::vector<Boid>& current,
                      std::vector<Boid>& next,
                      int start,
                      int end)
    {
        int n = static_cast<int>(current.size());

        for (int i = start; i < end; ++i)
        {
            float cx = 0.0f;
            float cy = 0.0f;
            int count = 0;

            const Boid& self = current[i];

            for (int j = 0; j < n; ++j)
            {
                if (i == j) continue;

                float dx = current[j].x - self.x;
                float dy = current[j].y - self.y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < NEIGHBOR_RADIUS)
                {
                    cx += current[j].x;
                    cy += current[j].y;
                    ++count;
                }
            }

            Boid updated = self;

            if (count > 0)
            {
                cx /= static_cast<float>(count);
                cy /= static_cast<float>(count);

                updated.vx += (cx - self.x) * COHESION_FACTOR;
                updated.vy += (cy - self.y) * COHESION_FACTOR;
            }

            clamp_speed(updated.vx, updated.vy);

            updated.x += updated.vx;
            updated.y += updated.vy;

            wrap_position(updated.x, updated.y);

            next[i] = updated;
        }
    }
}

std::vector<Boid> create_boids(int count)
{
    std::vector<Boid> boids(count);

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> pos(0.0f, WORLD_SIZE);
    std::uniform_real_distribution<float> vel(-1.0f, 1.0f);

    for (auto& b : boids)
    {
        b.x = pos(rng);
        b.y = pos(rng);
        b.vx = vel(rng);
        b.vy = vel(rng);
    }

    return boids;
}

void update_boids_single(const std::vector<Boid>& current,
                         std::vector<Boid>& next)
{
    update_range(current, next, 0, static_cast<int>(current.size()));
}

void update_boids_multithreaded(const std::vector<Boid>& current,
                                std::vector<Boid>& next,
                                int thread_count)
{
    if (thread_count <= 1)
    {
        update_boids_single(current, next);
        return;
    }

    int n = static_cast<int>(current.size());
    int chunk = n / thread_count;
    int remainder = n % thread_count;

    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    int start = 0;
    for (int t = 0; t < thread_count; ++t)
    {
        int size = chunk + (t < remainder ? 1 : 0);
        int end = start + size;

        workers.emplace_back(update_range,
                             std::cref(current),
                             std::ref(next),
                             start,
                             end);

        start = end;
    }

    for (auto& worker : workers)
    {
        worker.join();
    }
}