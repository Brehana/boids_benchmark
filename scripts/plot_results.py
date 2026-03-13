import csv
import os
import sys
from collections import defaultdict

import matplotlib.pyplot as plt


def load_results(csv_path):
    grouped = defaultdict(list)

    with open(csv_path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            threads = int(row["threads"])
            grouped[threads].append({
                "avg_frame_ms": float(row["avg_frame_ms"]),
                "throughput": float(row["throughput_boids_per_sec"]),
                "speedup": float(row["speedup"]),
                "efficiency": float(row["efficiency"]),
            })

    summary = {}
    for threads, rows in grouped.items():
        summary[threads] = {
            "avg_frame_ms": sum(r["avg_frame_ms"] for r in rows) / len(rows),
            "throughput": sum(r["throughput"] for r in rows) / len(rows),
            "speedup": sum(r["speedup"] for r in rows) / len(rows),
            "efficiency": sum(r["efficiency"] for r in rows) / len(rows),
        }

    return dict(sorted(summary.items()))


def make_plot(x, y, xlabel, ylabel, title, out_path):
    plt.figure(figsize=(8, 5))
    plt.plot(x, y, marker="o")
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(out_path, dpi=150)
    print(f"wrote {out_path}")


def main():
    if len(sys.argv) != 2:
        print("usage: python3 scripts/plot_results.py data/results.csv")
        sys.exit(1)

    csv_path = sys.argv[1]
    results = load_results(csv_path)

    threads = list(results.keys())
    frame_ms = [results[t]["avg_frame_ms"] for t in threads]
    throughput = [results[t]["throughput"] for t in threads]
    speedup = [results[t]["speedup"] for t in threads]
    efficiency = [results[t]["efficiency"] * 100.0 for t in threads]

    os.makedirs("plots", exist_ok=True)

    make_plot(threads, frame_ms,
              "Threads", "Average Frame Time (ms)",
              "Boids Benchmark: Frame Time vs Threads",
              "plots/frame_time.png")

    make_plot(threads, throughput,
              "Threads", "Throughput (boids/sec)",
              "Boids Benchmark: Throughput vs Threads",
              "plots/throughput.png")

    make_plot(threads, speedup,
              "Threads", "Speedup (x)",
              "Boids Benchmark: Speedup vs Threads",
              "plots/speedup.png")

    make_plot(threads, efficiency,
              "Threads", "Efficiency (%)",
              "Boids Benchmark: Efficiency vs Threads",
              "plots/efficiency.png")


if __name__ == "__main__":
    main()