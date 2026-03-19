import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv("final_average.csv")


# =========================
# Q3: QUICK SORT (3 VERSIONS)
# =========================

def plot_quick(case, title):
    plt.figure()

    for algo in ["Quick_First", "Quick_Random", "Quick_Median"]:
        data = df[(df["Algo"] == algo) & (df["Case"] == case)].sort_values("n")
        plt.plot(data["n"], data["Time"], marker='o', label=algo)

    plt.xlabel("n")
    plt.ylabel("Time (microseconds)")
    plt.title(title)
    plt.legend()
    plt.yscale('log')
    plt.grid()
    plt.show()


plot_quick("Best", "Quick Sort - Best Case")
plot_quick("Worst", "Quick Sort - Worst Case")
plot_quick("Random", "Quick Sort - Average Case")


# =========================
# Q4: ALL 7 ALGORITHMS
# =========================

def plot_all(case, title):
    subset = df[df["Case"] == case]

    plt.figure()

    for algo in sorted(subset["Algo"].unique()):
        # Use only best Quick version
        if algo.startswith("Quick_") and algo != "Quick_Median":
            continue

        data = subset[subset["Algo"] == algo].sort_values("n")
        plt.plot(data["n"], data["Time"], marker='o', label=algo)

    plt.xlabel("n")
    plt.ylabel("Time (microseconds)")
    plt.title(title)
    plt.legend()
    plt.yscale('log')
    plt.grid()
    plt.show()


plot_all("Best", "All Algorithms - Best Case")
plot_all("Worst", "All Algorithms - Worst Case")
plot_all("Random", "All Algorithms - Average Case")


# =========================
# Q5: TIME vs n
# =========================

df_rand = df[df["Case"] == "Random"]

plt.figure()
for algo in sorted(df_rand["Algo"].unique()):
    if algo.startswith("Quick_") and algo != "Quick_Median":
        continue

    data = df_rand[df_rand["Algo"] == algo].sort_values("n")
    plt.plot(data["n"], data["Time"], marker='o', label=algo)

plt.xlabel("n")
plt.ylabel("Time (microseconds)")
plt.title("Time vs n (Random Input)")
plt.legend()
plt.yscale('log')
plt.grid()
plt.show()


# =========================
# Q5: COMPARISONS vs n
# =========================

plt.figure()
for algo in sorted(df_rand["Algo"].unique()):
    if algo.startswith("Quick_") and algo != "Quick_Median":
        continue

    data = df_rand[df_rand["Algo"] == algo].sort_values("n")
    plt.plot(data["n"], data["Comparisons"], marker='o', label=algo)

plt.xlabel("n")
plt.ylabel("Comparisons")
plt.title("Comparisons vs n (Random Input)")
plt.legend()
plt.yscale('log')
plt.grid()
plt.show()


# =========================
# 🔥 NEW: TIME vs COMPARISONS (CORRELATION)
# =========================

plt.figure()

for algo in sorted(df_rand["Algo"].unique()):
    if algo.startswith("Quick_") and algo != "Quick_Median":
        continue

    data = df_rand[df_rand["Algo"] == algo]
    plt.scatter(data["Comparisons"], data["Time"], label=algo)

plt.xlabel("Comparisons")
plt.ylabel("Time (microseconds)")
plt.title("Time vs Comparisons (Correlation)")
plt.xscale('log')
plt.yscale('log')
plt.legend()
plt.grid()
plt.show()