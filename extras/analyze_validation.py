#!/usr/bin/env python3
"""Agreement statistics for the PulseNet validation log.

    python3 extras/analyze_validation.py my_log.csv

Reads a CSV shaped like extras/validation_template.csv (blank cells are skipped),
prints the tables and writes validation_results.md next to the CSV.
Standard library only.
"""
import csv
import math
import statistics as st
import sys
from pathlib import Path

PAIRS = [
    ("ECG vs reference (bpm)", "ecg_bpm", "ref_bpm", 5),
    ("Finger pulse vs reference (bpm)", "ppg_bpm", "ref_bpm", 5),
    ("ECG vs finger pulse (bpm)", "ecg_bpm", "ppg_bpm", 5),
    ("SpO2 vs reference (points)", "spo2", "ref_spo2", 2),
]


def num(v):
    try:
        return float(v)
    except (TypeError, ValueError):
        return None


def pearson(x, y):
    mx, my = st.mean(x), st.mean(y)
    sx = math.sqrt(sum((a - mx) ** 2 for a in x))
    sy = math.sqrt(sum((b - my) ** 2 for b in y))
    if sx == 0 or sy == 0:
        return float("nan")
    return sum((a - mx) * (b - my) for a, b in zip(x, y)) / (sx * sy)


def stats(rows, a, b, tol):
    xs, ys = [], []
    for r in rows:
        x, y = num(r.get(a)), num(r.get(b))
        if x is not None and y is not None:
            xs.append(x)
            ys.append(y)
    n = len(xs)
    if n < 2:
        return None
    d = [x - y for x, y in zip(xs, ys)]
    bias = st.mean(d)
    sd = st.stdev(d)
    return {
        "n": n,
        "mae": st.mean(abs(v) for v in d),
        "bias": bias,
        "sd": sd,
        "lo": bias - 1.96 * sd,
        "hi": bias + 1.96 * sd,
        "within": 100 * sum(abs(v) <= tol for v in d) / n,
        "r": pearson(xs, ys),
        "tol": tol,
    }


def table(rows, title):
    out = [f"### {title}", "",
           "| Comparison | n | MAE | Bias | Limits of agreement | Within tolerance | r |",
           "|---|---|---|---|---|---|---|"]
    for label, a, b, tol in PAIRS:
        s = stats(rows, a, b, tol)
        if s is None:
            out.append(f"| {label} | <2 | - | - | - | - | - |")
        else:
            out.append(
                f"| {label} | {s['n']} | {s['mae']:.2f} | {s['bias']:+.2f} | "
                f"{s['lo']:+.1f} to {s['hi']:+.1f} | {s['within']:.0f}% (±{s['tol']}) | {s['r']:.2f} |"
            )
    out.append("")
    return "\n".join(out)


def main():
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    path = Path(sys.argv[1])
    with path.open(newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))

    parts = [f"# PulseNet validation results\n\nSource: `{path.name}`, {len(rows)} rows.\n",
             table(rows, "All trials")]
    for cond in sorted({r.get("condition", "").strip() for r in rows if r.get("condition", "").strip()}):
        parts.append(table([r for r in rows if r.get("condition", "").strip() == cond], f"Condition: {cond}"))
    parts.append("MAE = mean absolute error. Bias = mean of (first minus second). "
                 "Limits of agreement = bias ± 1.96 SD of the differences (Bland-Altman). "
                 "Reference devices are consumer-grade, so this is agreement, not clinical accuracy.\n")
    text = "\n".join(parts)
    print(text)
    out = path.with_name("validation_results.md")
    out.write_text(text, encoding="utf-8")
    print(f"Wrote {out}")


if __name__ == "__main__":
    main()
