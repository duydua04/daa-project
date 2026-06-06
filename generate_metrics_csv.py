#!/usr/bin/env python3
import csv
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent
BINARIES = {
    "Greedy": ROOT / "greedy" / "cloud_computing",
    "Dynamic Programming": ROOT / "dynamic-programing" / "cloud_computing",
    "Brute Force": ROOT / "brute-force" / "cloud_computing",
}

OUTPUTS = {
    "maximum_value": "Maximum Value",
    "memory_usage": "Memory Usage",
    "execution_time": "Execution Time",
    "cpu_usage": "CPU",
    "ram_usage": "RAM",
    "bandwidth_usage": "Bandwidth",
}

RESULT_FIELDS = [
    "maximum_value",
    "memory_usage",
    "execution_time",
    "cpu_usage",
    "ram_usage",
    "bandwidth_usage",
]

FILE_PATTERN = re.compile(r"Results for:\s*(.+)")
N_PATTERN = re.compile(r"n(\d+)\.txt")
MAX_PATTERN = re.compile(r"Maximum Value\s*:\s*(\d+)")
MEM_PATTERN = re.compile(r"Memory Usage\s*:\s*([0-9]+)\s*bytes")
TIME_PATTERN = re.compile(r"Execution Time\s*:\s*([0-9]+(?:\.[0-9]+)?)\s*us")
CPU_PATTERN = re.compile(r"CPU\s*:\s*([0-9]+)\s*/\s*([0-9]+)")
RAM_PATTERN = re.compile(r"RAM\s*:\s*([0-9]+)\s*/\s*([0-9]+)")
BW_PATTERN = re.compile(r"Bandwidth\s*:\s*([0-9]+)\s*/\s*([0-9]+)")


def parse_output(text):
    sections = []
    current = None
    for raw in text.splitlines():
        line = raw.strip()
        if not line:
            continue
        m = FILE_PATTERN.match(line)
        if m:
            if current is not None:
                sections.append(current)
            current = {
                "file": m.group(1),
                "maximum_value": None,
                "memory_usage": None,
                "execution_time": None,
                "cpu_usage": None,
                "ram_usage": None,
                "bandwidth_usage": None,
            }
            continue

        if current is None:
            continue

        if m := MAX_PATTERN.search(line):
            current["maximum_value"] = int(m.group(1))
            continue
        if m := MEM_PATTERN.search(line):
            current["memory_usage"] = int(m.group(1))
            continue
        if m := TIME_PATTERN.search(line):
            current["execution_time"] = float(m.group(1))
            continue
        if m := CPU_PATTERN.search(line):
            current["cpu_usage"] = int(m.group(1))
            continue
        if m := RAM_PATTERN.search(line):
            current["ram_usage"] = int(m.group(1))
            continue
        if m := BW_PATTERN.search(line):
            current["bandwidth_usage"] = int(m.group(1))
            continue

    if current is not None:
        sections.append(current)
    return sections


def read_results():
    results = {method: {} for method in BINARIES}
    sizes = set()

    for method, binary in BINARIES.items():
        if not binary.exists():
            raise FileNotFoundError(f"Binary not found: {binary}")

        print(f"Running {method}...", flush=True)
        completed = subprocess.run([str(binary)], capture_output=True, text=True)
        if completed.returncode != 0:
            raise RuntimeError(
                f"Execution failed for {method}: {completed.returncode}\n{completed.stderr}"
            )

        for section in parse_output(completed.stdout):
            file_match = N_PATTERN.search(section["file"])
            if not file_match:
                continue
            n = int(file_match.group(1))
            sizes.add(n)
            results[method][n] = section

    return results, sorted(sizes)


def write_csv(metric, results, sizes):
    output_dir = ROOT / "statistics"
    output_dir.mkdir(exist_ok=True)
    path = output_dir / f"{metric}.csv"
    methods = list(BINARIES.keys())
    header = ["n"] + methods
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(header)
        for n in sizes:
            row = [n]
            for method in methods:
                metric_value = results[method].get(n, {}).get(metric)
                row.append(metric_value if metric_value is not None else "")
            writer.writerow(row)
    print(f"Wrote {path}")


def main():
    results, sizes = read_results()
    for metric in RESULT_FIELDS:
        write_csv(metric, results, sizes)
    print("CSV export finished.")


if __name__ == "__main__":
    main()