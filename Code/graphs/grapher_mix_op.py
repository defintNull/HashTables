import pandas as pd
import matplotlib.pyplot as plt

# -----------------------------
# Leggi CSV di tutti i KeyType
# -----------------------------
files = {
    "Integer": "../../../intTest/op_data.csv",
    "String": "../../../stringTest/op_data.csv",
    "Real Word": "../../../realWordTest/op_data.csv",
    "Hybrid": "../../../hybridTest/op_data.csv"
}

dfs = []
for key_type, path in files.items():
    temp = pd.read_csv(path)
    temp["KeyType"] = key_type
    dfs.append(temp)

df = pd.concat(dfs, ignore_index=True)

# -----------------------------
# Stile
# -----------------------------
plt.rcParams.update({
    "text.usetex": False,
    "font.family": "serif",
    "font.serif": ["Latin Modern Roman"],
    "font.size": 10,
    "axes.labelsize": 10,
    "axes.titlesize": 12,
    "legend.fontsize": 9,
    "xtick.labelsize": 8,
    "ytick.labelsize": 8
})




# -----------------------------
# Latency / Operation by Type
# -----------------------------

# -----------------------------
# Statistiche
# -----------------------------
stats = df.groupby(["Elements", "Operazione", "KeyType"])["Latenza"] \
          .agg(["mean","min","max"]).reset_index()

operations = sorted(stats["Operazione"].unique())
key_types = sorted(stats["KeyType"].unique())
elements_values = sorted(stats["Elements"].unique())

# -----------------------------
# Costruzione asse X
# -----------------------------
x_labels = []
x_ticks = []
x_positions = {}

pos = 0
gap = 1  # gap tra operazioni

for op in operations:
    for key in key_types:
        x_labels.append(key)
        x_ticks.append(pos)
        x_positions[(op, key)] = pos
        pos += 1
    pos += gap

# -----------------------------
# Plot
# -----------------------------
plt.figure(figsize=(14,6))
ax = plt.gca()

colors = plt.cm.tab10.colors
width = 0.15

for i, elements in enumerate(elements_values):
    subset_elements = stats[stats["Elements"] == elements]

    x_vals = []
    y_vals = []
    err_low = []
    err_high = []

    for _, row in subset_elements.iterrows():
        base_x = x_positions[(row["Operazione"], row["KeyType"])]
        offset = (i - (len(elements_values)-1)/2) * width

        x_vals.append(base_x + offset)
        y_vals.append(row["mean"])
        err_low.append(row["mean"] - row["min"])
        err_high.append(row["max"] - row["mean"])

    ax.errorbar(
        x_vals,
        y_vals,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Etichette media solo primo e ultimo gruppo
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)
            ax.text(
                xi,
                yi + yi_log_offset,
                f"{yi:.3g}",
                ha="center",
                va="bottom" if sign > 0 else "top",
                fontsize=8,
                bbox=dict(facecolor="white", alpha=0.7, edgecolor="none", pad=1)
            )

    

# -----------------------------
# Asse X
# -----------------------------
ax.set_xticks(x_ticks)
ax.set_xticklabels(x_labels, rotation=30, ha="right")

ax.set_ylabel("Average Latency [s]")
ax.set_xlabel("Key Type")
ax.set_title("Latency by Operation and Key Type")
ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra operazioni
# -----------------------------
pos = 0
for _ in operations[:-1]:
    pos += len(key_types)
    line_pos = pos + (gap / 2) - 0.5
    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)
    pos += gap

# -----------------------------
# Etichette operazioni
# -----------------------------
pos = 0
for op in operations:
    center = pos + (len(key_types)-1)/2
    ax.text(
        center,
        -0.30,
        op,
        transform=ax.get_xaxis_transform(),
        ha="center",
        va="top",
        fontsize=11,
        fontweight="bold"
    )
    pos += len(key_types) + gap

# -----------------------------
# Legenda
# -----------------------------
ax.legend(title="Elements")
ax.set_yscale("log")

plt.tight_layout()
plt.subplots_adjust(bottom=0.38)
plt.savefig("latency_by_operation_keytype_elements.pdf")
plt.close()