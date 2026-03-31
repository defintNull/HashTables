import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# -----------------------------
# Leggi CSV di tutti i KeyType
# -----------------------------
files = {
    "Integer": "../../../intTest/hash_data.csv",
    "String": "../../../stringTest/hash_data.csv",
    "Real Word": "../../../realWordTest/hash_data.csv",
    "Hybrid": "../../../hybridTest/hash_data.csv"
}

dfs = []
for key_type, path in files.items():
    temp = pd.read_csv(path)
    temp["Type"] = key_type
    dfs.append(temp)

df = pd.concat(dfs, ignore_index=True)

# -----------------------------
# Stile globale
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
# Funzione abbreviazione Overflow
# -----------------------------
def short_overflow(name):
    return name.replace("OPEN_ADDRESSING_", "").replace("_PROBING","").replace("_"," ")

# -----------------------------
# Etichette alternate TRA serie
# -----------------------------
def add_alternating_labels(ax, x_vals, y_vals, base_offset, series_index):
    direction = -1 if series_index % 2 == 0 else 1  # pari sotto, dispari sopra

    for xi, yi in zip(x_vals, y_vals):
        ax.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# LATENCY / OVERFLOW
# -----------------------------
stats = df.groupby(["Type", "OVERFLOW", "Elements"])["Latenza"].agg(["mean","min","max"]).reset_index()
key_types = sorted(df["Type"].unique())
elements_values = sorted(df["Elements"].unique())

x_labels, x_ticks, x_positions = [], [], {}
gap_type = 1
pos = 0

for kt in key_types:
    ofs = stats.loc[stats["Type"] == kt, "OVERFLOW"].unique()
    for of in ofs:
        x_labels.append(short_overflow(of))
        x_ticks.append(pos)
        x_positions[(kt, of)] = pos
        pos += 1
    pos += gap_type

plt.figure(figsize=(18,6))
ax = plt.gca()
colors = plt.cm.tab10.colors
width = 0.15

for i, elements in enumerate(elements_values):
    subset = stats[stats["Elements"] == elements]
    x_vals = subset.apply(lambda r: x_positions[(r["Type"], r["OVERFLOW"])], axis=1)
    offsets = (i - (len(elements_values)-1)/2) * width
    x_vals = x_vals + offsets

    y_vals = subset["mean"]
    err_low = y_vals - subset["min"]
    err_high = subset["max"] - y_vals

    ax.errorbar(x_vals, y_vals, yerr=[err_low, err_high],
                fmt="o", label=f"Elements = {elements}",
                color=colors[i % len(colors)], capsize=4)

    if elements in [elements_values[0], elements_values[-1]]:
        y_range = stats["max"].max() - stats["min"].min()
        base_offset = y_range * 0.02
        add_alternating_labels(ax, x_vals, y_vals, base_offset, i)

ax.set_xticks(x_ticks)
ax.set_xticklabels(x_labels, rotation=30, ha="right")

# separatori
pos = 0
for kt in key_types[:-1]:
    block = len(stats.loc[stats["Type"] == kt, "OVERFLOW"].unique())
    pos += block
    ax.axvline(pos + (gap_type/2) - 0.5, linestyle="--", alpha=0.5)
    pos += gap_type

# etichette KeyType
pos = 0
for kt in key_types:
    block = len(stats.loc[stats["Type"] == kt, "OVERFLOW"].unique())
    center = pos + (block - 1)/2
    ax.annotate(kt, xy=(center, 0), xytext=(0, -70),
                textcoords="offset points", ha="center",
                va="top", fontsize=12, fontweight="bold")
    pos += block + gap_type

ax.set_xlabel("Overflow Strategy")
ax.set_ylabel("Average Latency [s]")
ax.set_title("Latency by Key Type and Overflow Strategy")
ax.grid(True, axis="y", linestyle="--", alpha=0.7)
ax.legend(title="Elements")

plt.subplots_adjust(bottom=0.45)
plt.tight_layout()
plt.savefig("latency_by_keytype_overflow.pdf")
plt.close()

# -----------------------------
# FUNZIONE GENERICA
# -----------------------------
def plot_grouped(stats_df, category, filename, xlabel, title):
    x_labels, x_ticks, x_positions = [], [], {}
    pos = 0

    for kt in key_types:
        vals = stats_df.loc[stats_df["Type"] == kt, category].unique()
        for v in vals:
            x_labels.append(v)
            x_ticks.append(pos)
            x_positions[(kt, v)] = pos
            pos += 1
        pos += gap_type

    plt.figure(figsize=(18,6))
    ax = plt.gca()

    for i, elements in enumerate(elements_values):
        subset = stats_df[stats_df["Elements"] == elements]
        x_vals = subset.apply(lambda r: x_positions[(r["Type"], r[category])], axis=1)
        offsets = (i - (len(elements_values)-1)/2) * width
        x_vals = x_vals + offsets

        y_vals = subset["mean"]
        err_low = y_vals - subset["min"]
        err_high = subset["max"] - y_vals

        ax.errorbar(x_vals, y_vals, yerr=[err_low, err_high],
                    fmt="o", label=f"Elements = {elements}",
                    color=colors[i % len(colors)], capsize=4)

        if elements in [elements_values[0], elements_values[-1]]:
            y_range = stats_df["max"].max() - stats_df["min"].min()
            base_offset = y_range * 0.02
            add_alternating_labels(ax, x_vals, y_vals, base_offset, i)

    ax.set_xticks(x_ticks)
    ax.set_xticklabels(x_labels, rotation=30, ha="right")

    pos = 0
    for kt in key_types[:-1]:
        block = len(stats_df.loc[stats_df["Type"] == kt, category].unique())
        pos += block
        ax.axvline(pos + (gap_type/2) - 0.5, linestyle="--", alpha=0.5)
        pos += gap_type

    pos = 0
    for kt in key_types:
        block = len(stats_df.loc[stats_df["Type"] == kt, category].unique())
        center = pos + (block - 1)/2
        ax.annotate(kt, xy=(center, 0), xytext=(0, -70),
                    textcoords="offset points", ha="center",
                    va="top", fontsize=12, fontweight="bold")
        pos += block + gap_type

    ax.set_xlabel(xlabel)
    ax.set_ylabel("Average Latency [s]")
    ax.set_title(title)
    ax.grid(True, axis="y", linestyle="--", alpha=0.7)
    ax.legend()

    plt.subplots_adjust(bottom=0.45)
    plt.tight_layout()
    plt.savefig(filename)
    plt.close()

# -----------------------------
# RESIZE
# -----------------------------
stats_resize = df.groupby(["Type", "RESIZE", "Elements"])["Latenza"].agg(["mean","min","max"]).reset_index()
plot_grouped(stats_resize, "RESIZE",
             "latency_by_keytype_resize.pdf",
             "Resize Strategy",
             "Latency by Key Type and Resize Strategy")

# -----------------------------
# HASH
# -----------------------------
stats_hash = df.groupby(["Type", "HASH", "Elements"])["Latenza"].agg(["mean","min","max"]).reset_index()
plot_grouped(stats_hash, "HASH",
             "latency_by_keytype_hash.pdf",
             "Hash Mode",
             "Latency by Key Type and Hash Mode")

# -----------------------------
# MEMORY
# -----------------------------
stats_mem = df.groupby(["Type", "Elements"])["Memoria"].agg(["mean","min","max"]).reset_index()

plt.figure(figsize=(12,6))

for i, elements in enumerate(elements_values):
    subset = stats_mem[stats_mem["Elements"] == elements]
    x_num = [key_types.index(x) for x in subset["Type"]]
    offset = (i - (len(elements_values)-1)/2) * width
    x_vals = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(x_vals, y, yerr=[err_low, err_high],
                 fmt="o", label=f"Elements = {elements}",
                 color=colors[i % len(colors)], capsize=4)

    y_range = stats_mem["max"].max() - stats_mem["min"].min()
    base_offset = y_range * 0.02
    add_alternating_labels(plt.gca(), x_vals, y, base_offset, i)

plt.xticks(range(len(key_types)), key_types, rotation=30, ha="right")
plt.xlabel("Key Type")
plt.ylabel("Average Memory [B]")
plt.title("Memory by Key Type")
plt.grid(True, axis="y")
plt.legend()

plt.tight_layout()
plt.savefig("memory_by_key_type.pdf")
plt.close()