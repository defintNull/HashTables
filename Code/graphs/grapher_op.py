import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

reps = 5

# -----------------------------
# Leggi CSV
# -----------------------------
path = "../../../intTest/op_data.csv"
double = False
df = pd.read_csv(path)

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
# LATENCY / OPERATION & OWERFLOW
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Statistiche
# -----------------------------
stats = (
    df.groupby(["Elements", "Operazione", "OVERFLOW"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

operations = sorted(stats["Operazione"].unique())
overflows = sorted(stats["OVERFLOW"].unique())
elements_values = sorted(stats["Elements"].unique())

# funzione per abbreviare overflow
def short_overflow(name):
    return name.replace("OPEN_ADDRESSING_", "").replace("_PROBING","").replace("_"," ")

# -----------------------------
# Costruzione asse X
# -----------------------------
x_labels = []
x_ticks = []
x_positions = {}

pos = 0
gap = 1

for op in operations:
    for of in overflows:

        x_labels.append(short_overflow(of))
        x_ticks.append(pos)

        x_positions[(op, of)] = pos
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

        base_x = x_positions[(row["Operazione"], row["OVERFLOW"])]
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
    # Aggiungi etichette della media SOLO per primo e ultimo gruppo, vicino al punto e leggibili
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)  # moltiplica per 1.05 o 0.95
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
ax.set_xlabel("Overflow Strategy")
ax.set_title("Latency by Operation and Overflow Strategy")

ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra operazioni
# -----------------------------
pos = 0
for _ in operations[:-1]:

    pos += len(overflows)
    line_pos = pos + (gap / 2) - 0.5   # centro del gap

    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)

    pos += gap

# -----------------------------
# Etichette operazioni
# -----------------------------
pos = 0
for op in operations:

    center = pos + (len(overflows)-1)/2

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

    pos += len(overflows) + gap

# -----------------------------
# Legenda
# -----------------------------
ax.legend(title="Elements")

# opzionale
ax.set_yscale("log")

# -----------------------------
# Layout
# -----------------------------
plt.tight_layout()
plt.subplots_adjust(bottom=0.38)

plt.savefig("latency_by_operations_&_overflow_strategy.pdf")
plt.close()



# -----------------------------
# LATENCY / OPERATION & RESIZE
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Statistiche
# -----------------------------
stats = (
    df.groupby(["Elements", "Operazione", "RESIZE"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

operations = sorted(stats["Operazione"].unique())
overflows = sorted(stats["RESIZE"].unique())
elements_values = sorted(stats["Elements"].unique())

# funzione per abbreviare overflow
def short_overflow(name):
    return name.replace("OPEN_ADDRESSING_", "").replace("_PROBING","").replace("_"," ")

# -----------------------------
# Costruzione asse X
# -----------------------------
x_labels = []
x_ticks = []
x_positions = {}

pos = 0
gap = 1

for op in operations:
    for of in overflows:

        x_labels.append(short_overflow(of))
        x_ticks.append(pos)

        x_positions[(op, of)] = pos
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

        base_x = x_positions[(row["Operazione"], row["RESIZE"])]
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
    # Aggiungi etichette della media SOLO per primo e ultimo gruppo, vicino al punto e leggibili
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)  # moltiplica per 1.05 o 0.95
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
ax.set_xlabel("RESIZE Strategy")
ax.set_title("Latency by Operation and Resize Strategy")

ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra operazioni
# -----------------------------
pos = 0
for _ in operations[:-1]:

    pos += len(overflows)
    line_pos = pos + (gap / 2) - 0.5   # centro del gap

    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)

    pos += gap

# -----------------------------
# Etichette operazioni
# -----------------------------
pos = 0
for op in operations:

    center = pos + (len(overflows)-1)/2

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

    pos += len(overflows) + gap

# -----------------------------
# Legenda
# -----------------------------
ax.legend(title="Elements")

# opzionale
ax.set_yscale("log")

# -----------------------------
# Layout
# -----------------------------
plt.tight_layout()
plt.subplots_adjust(bottom=0.38)

plt.savefig("latency_by_operations_&_resize_strategy.pdf")
plt.close()



# -----------------------------
# LATENCY / OPERATION & MAX_LOAD_FACTOR
# -----------------------------
df = pd.read_csv(path)

n = len(df)

# -----------------------------
# Dividi in 5 blocchi uguali (per posizione)
# -----------------------------
df["block"] = np.floor(np.arange(n) / (n / reps)).astype(int)

# evita che l'ultimo vada fuori range (per arrotondamenti)
df["block"] = df["block"].clip(upper=4)

# -----------------------------
# 2. Dentro ogni blocco, dividi in 3 parti uguali
# -----------------------------
def split_in_three(group):
    m = len(group)
    idx = np.arange(m)

    group = group.copy()
    group["category"] = np.floor(idx / (m / 3)).astype(int)
    group["category"] = group["category"].clip(upper=2)

    # mappa numeri → etichette
    mapping = {0: "Standard", 1: "Low", 2: "High"}
    group["category"] = group["category"].map(mapping)

    return group

df = df.groupby("block", group_keys=False).apply(split_in_three)

# -----------------------------
# Statistiche
# -----------------------------
stats = (
    df.groupby(["Elements", "Operazione", "category"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

operations = sorted(stats["Operazione"].unique())
load_factors = ["Standard", "Low", "High"]  # ordine corretto
elements_values = sorted(stats["Elements"].unique())

# -----------------------------
# Plot
# -----------------------------
plt.figure(figsize=(14,6))
ax = plt.gca()

colors = plt.cm.tab10.colors
width = 0.15
gap = 1  # spazio tra operazioni

# costruzione asse X
x_labels = []
x_ticks = []
x_positions = {}
pos = 0
for op in operations:
    for lf in load_factors:
        x_labels.append(lf)
        x_ticks.append(pos)
        x_positions[(op, lf)] = pos
        pos += 1
    pos += gap

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
for i, elements in enumerate(elements_values):
    subset_elements = stats[stats["Elements"] == elements]

    x_vals = []
    y_vals = []
    err_low = []
    err_high = []

    for _, row in subset_elements.iterrows():
        base_x = x_positions[(row["Operazione"], row["category"])]
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
    # Aggiungi etichette della media SOLO per primo e ultimo gruppo, vicino al punto e leggibili
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)  # moltiplica per 1.05 o 0.95
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
ax.set_xticklabels(x_labels)

ax.set_ylabel("Average Latency [s]")
ax.set_xlabel("Max Load Factor")
ax.set_title("Latency by Operation and Max Load Factor")

ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra operazioni
# -----------------------------
pos = 0
for _ in operations[:-1]:
    pos += len(load_factors)
    line_pos = pos + (gap / 2) - 0.5
    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)
    pos += gap

# -----------------------------
# Etichette operazioni sotto asse X
# -----------------------------
pos = 0
for op in operations:
    center = pos + (len(load_factors)-1)/2
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
    pos += len(load_factors) + gap

# -----------------------------
# Legenda
# -----------------------------
ax.legend(title="Elements")

# opzionale
ax.set_yscale("log")

# -----------------------------
# Layout
# -----------------------------
plt.tight_layout()
plt.subplots_adjust(bottom=0.38)
plt.savefig("latency_by_operations_&_max_load_factor.pdf")
plt.close()




# -----------------------------
# MEMORY / OPERATION & OWERFLOW
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Statistiche
# -----------------------------
stats = (
    df.groupby(["Elements", "Operazione", "OVERFLOW"])["Memoria"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

operations = sorted(stats["Operazione"].unique())
overflows = sorted(stats["OVERFLOW"].unique())
elements_values = sorted(stats["Elements"].unique())

# funzione per abbreviare overflow
def short_overflow(name):
    return name.replace("OPEN_ADDRESSING_", "").replace("_PROBING","").replace("_"," ")

# -----------------------------
# Costruzione asse X
# -----------------------------
x_labels = []
x_ticks = []
x_positions = {}

pos = 0
gap = 1

for op in operations:
    for of in overflows:

        x_labels.append(short_overflow(of))
        x_ticks.append(pos)

        x_positions[(op, of)] = pos
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

        base_x = x_positions[(row["Operazione"], row["OVERFLOW"])]
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
    # Aggiungi etichette della media SOLO per primo e ultimo gruppo, vicino al punto e leggibili
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)  # moltiplica per 1.05 o 0.95
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

ax.set_ylabel("Average Memory [B]")
ax.set_xlabel("Overflow Strategy")
ax.set_title("Memory by Operation and Overflow Strategy")

ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra operazioni
# -----------------------------
pos = 0
for _ in operations[:-1]:

    pos += len(overflows)
    line_pos = pos + (gap / 2) - 0.5   # centro del gap

    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)

    pos += gap

# -----------------------------
# Etichette operazioni
# -----------------------------
pos = 0
for op in operations:

    center = pos + (len(overflows)-1)/2

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

    pos += len(overflows) + gap

# -----------------------------
# Legenda
# -----------------------------
ax.legend(title="Elements")

# opzionale
ax.set_yscale("log")

# -----------------------------
# Layout
# -----------------------------
plt.tight_layout()
plt.subplots_adjust(bottom=0.38)

plt.savefig("memory_by_operations_&_overflow_strategy.pdf")
plt.close()



# -----------------------------
# MEMORY / OPERATION & RESIZE
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Statistiche
# -----------------------------
stats = (
    df.groupby(["Elements", "Operazione", "RESIZE"])["Memoria"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

operations = sorted(stats["Operazione"].unique())
overflows = sorted(stats["RESIZE"].unique())
elements_values = sorted(stats["Elements"].unique())

# funzione per abbreviare overflow
def short_overflow(name):
    return name.replace("OPEN_ADDRESSING_", "").replace("_PROBING","").replace("_"," ")

# -----------------------------
# Costruzione asse X
# -----------------------------
x_labels = []
x_ticks = []
x_positions = {}

pos = 0
gap = 1

for op in operations:
    for of in overflows:

        x_labels.append(short_overflow(of))
        x_ticks.append(pos)

        x_positions[(op, of)] = pos
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

        base_x = x_positions[(row["Operazione"], row["RESIZE"])]
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
    # Aggiungi etichette della media SOLO per primo e ultimo gruppo, vicino al punto e leggibili
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)  # moltiplica per 1.05 o 0.95
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

ax.set_ylabel("Average Memory [B]")
ax.set_xlabel("RESIZE Strategy")
ax.set_title("Memory by Operation and Resize Strategy")

ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra operazioni
# -----------------------------
pos = 0
for _ in operations[:-1]:

    pos += len(overflows)
    line_pos = pos + (gap / 2) - 0.5   # centro del gap

    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)

    pos += gap

# -----------------------------
# Etichette operazioni
# -----------------------------
pos = 0
for op in operations:

    center = pos + (len(overflows)-1)/2

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

    pos += len(overflows) + gap

# -----------------------------
# Legenda
# -----------------------------
ax.legend(title="Elements")

# opzionale
ax.set_yscale("log")

# -----------------------------
# Layout
# -----------------------------
plt.tight_layout()
plt.subplots_adjust(bottom=0.38)

plt.savefig("memory_by_operations_&_resize_strategy.pdf")
plt.close()



# -----------------------------
# MEMORY / OPERATION & MAX_LOAD_FACTOR
# -----------------------------
df = pd.read_csv(path)

n = len(df)

# -----------------------------
# Dividi in 5 blocchi uguali (per posizione)
# -----------------------------
df["block"] = np.floor(np.arange(n) / (n / reps)).astype(int)

# evita che l'ultimo vada fuori range (per arrotondamenti)
df["block"] = df["block"].clip(upper=4)

# -----------------------------
# 2. Dentro ogni blocco, dividi in 3 parti uguali
# -----------------------------
def split_in_three(group):
    m = len(group)
    idx = np.arange(m)

    group = group.copy()
    group["category"] = np.floor(idx / (m / 3)).astype(int)
    group["category"] = group["category"].clip(upper=2)

    # mappa numeri → etichette
    mapping = {0: "Standard", 1: "Low", 2: "High"}
    group["category"] = group["category"].map(mapping)

    return group

df = df.groupby("block", group_keys=False).apply(split_in_three)

# -----------------------------
# Statistiche
# -----------------------------
stats = (
    df.groupby(["Elements", "Operazione", "category"])["Memoria"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

operations = sorted(stats["Operazione"].unique())
load_factors = ["Standard", "Low", "High"]  # ordine corretto
elements_values = sorted(stats["Elements"].unique())

# -----------------------------
# Plot
# -----------------------------
plt.figure(figsize=(14,6))
ax = plt.gca()

colors = plt.cm.tab10.colors
width = 0.15
gap = 1  # spazio tra operazioni

# costruzione asse X
x_labels = []
x_ticks = []
x_positions = {}
pos = 0
for op in operations:
    for lf in load_factors:
        x_labels.append(lf)
        x_ticks.append(pos)
        x_positions[(op, lf)] = pos
        pos += 1
    pos += gap

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
for i, elements in enumerate(elements_values):
    subset_elements = stats[stats["Elements"] == elements]

    x_vals = []
    y_vals = []
    err_low = []
    err_high = []

    for _, row in subset_elements.iterrows():
        base_x = x_positions[(row["Operazione"], row["category"])]
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
    # Aggiungi etichette della media SOLO per primo e ultimo gruppo, vicino al punto e leggibili
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)  # moltiplica per 1.05 o 0.95
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
ax.set_xticklabels(x_labels)

ax.set_ylabel("Average Memory [B]")
ax.set_xlabel("Max Load Factor")
ax.set_title("Memory by Operation and Max Load Factor")

ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra operazioni
# -----------------------------
pos = 0
for _ in operations[:-1]:
    pos += len(load_factors)
    line_pos = pos + (gap / 2) - 0.5
    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)
    pos += gap

# -----------------------------
# Etichette operazioni sotto asse X
# -----------------------------
pos = 0
for op in operations:
    center = pos + (len(load_factors)-1)/2
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
    pos += len(load_factors) + gap

# -----------------------------
# Legenda
# -----------------------------
ax.legend(title="Elements")

# opzionale
ax.set_yscale("log")

# -----------------------------
# Layout
# -----------------------------
plt.tight_layout()
plt.subplots_adjust(bottom=0.38)
plt.savefig("memory_by_operations_&_max_load_factor.pdf")
plt.close()



# -----------------------------
# Latency / OPERATION & LOAD_FACTOR
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Operazioni: solo INSERT e EXIST
# Elements = 10000
# Tutti gli overflow
# -----------------------------
df = df[
    (df["Operazione"].isin(["INSERT", "EXIST"])) &
    (df["Elements"] == (16000 if double else 10000))
]

elements_values = sorted(df["Elements"].unique())  # qui è solo [10000]
operations = sorted(df["Operazione"].unique())
overflows = sorted(df["OVERFLOW"].unique())
colors = plt.cm.tab10.colors

# -----------------------------
# Crea bin per Load_factor sull'intervallo completo
# -----------------------------
n_bins = 32  # più punti per maggiore granularità
df["Load_bin"] = pd.cut(df["Load_factor"], bins=n_bins, include_lowest=True)

# -----------------------------
# Calcola stats per bin
# -----------------------------
stats = (
    df.groupby(["Operazione", "OVERFLOW", "Load_bin"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Plot: due subplot per INSERT ed EXIST
# -----------------------------
fig, axes = plt.subplots(1, 2, figsize=(14,6), sharey=True)

for ax, op in zip(axes, operations):
    subset_op = stats[stats["Operazione"] == op]
    
    for j, overflow in enumerate(overflows):
        subset_overflow = subset_op[subset_op["OVERFLOW"] == overflow]
        
        x_vals = [interval.mid for interval in subset_overflow["Load_bin"]]
        y_vals = subset_overflow["mean"]
        err_low = y_vals - subset_overflow["min"]
        err_high = subset_overflow["max"] - y_vals

        ax.errorbar(
            x_vals,
            y_vals,
            yerr=[err_low, err_high],
            fmt="-o",
            color=colors[j % len(colors)],
            alpha=0.8,
            capsize=3,
            label=f"{overflow}"
        )
    
    ax.set_xlim(df["Load_factor"].min(), df["Load_factor"].max())
    ax.set_xlabel("Load Factor")
    ax.set_title(f"Operation: {op}")
    ax.grid(True, axis="y")
    ax.legend(fontsize=9)

axes[0].set_ylabel("Latency [s]")

plt.tight_layout()
plt.yscale("log")
plt.savefig("latency_insert_exist_by_load_factor_&_all_overflows.pdf")
plt.close()



# -----------------------------
# Latency / Key Distribution
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Filtra Elements = 10000
# -----------------------------
df = df[df["Elements"] == (16000 if double else 10000)]

# -----------------------------
# Calcola media, min, max di Key_distribution per Overflow
# -----------------------------
stats = (
    df.groupby("OVERFLOW")["Key_distribution"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Prepara grafico
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

x = range(len(stats))
y = stats["mean"]
err_low = y - stats["min"]
err_high = stats["max"] - y

plt.bar(x, y, yerr=[err_low, err_high], color=colors[:len(x)], capsize=5)
plt.xticks(x, stats["OVERFLOW"].str.replace("OPEN_ADDRESSING_", "").str.replace("_", " "), rotation=30, ha="right")
plt.ylabel("Key Distribution")
plt.title("Key Distribution by Overflow Method (Elements = " + ("16000)" if double else "10000)"))
plt.grid(axis="y", linestyle="--", alpha=0.5)

plt.tight_layout()
plt.savefig("key_distribution_by_overflow.pdf")
plt.close()




# -----------------------------
# LATENCY / HASH & OPERAZIONE
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Statistiche
# -----------------------------
stats = (
    pd.concat([
        df[(df["HASH"] == "MODE_1") & (df["OVERFLOW"] == "OPEN_ADDRESSING_LINEAR_PROBING")],
        df[df["HASH"] == "MODE_2"]
    ])
    .groupby(["Elements", "HASH", "Operazione"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

hashes = sorted(stats["HASH"].unique())
operations = sorted(stats["Operazione"].unique())
elements_values = sorted(stats["Elements"].unique())

# -----------------------------
# Costruzione asse X
# -----------------------------
x_labels = []
x_ticks = []
x_positions = {}
gap = 1  # gap tra HASH

pos = 0
for h in hashes:
    ops_in_hash = stats.loc[stats["HASH"] == h, "Operazione"].unique()
    for op in ops_in_hash:
        x_labels.append(op)
        x_ticks.append(pos)
        x_positions[(h, op)] = pos
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
        base_x = x_positions[(row["HASH"], row["Operazione"])]
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
    # Aggiungi etichette della media SOLO per primo e ultimo gruppo, vicino al punto e leggibili
    # -----------------------------
    log_offset = 0.1  # 5% in termini di log
    if elements in [elements_values[0], elements_values[-1]]:
        for xi, yi in zip(x_vals, y_vals):
            sign = -1 if elements == elements_values[0] else 1
            yi_log_offset = yi * (10**(sign * log_offset) - 1)  # moltiplica per 1.05 o 0.95
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
ax.set_xlabel("Operation")
ax.set_title("Latency by Operation and Hash Strategy")
ax.grid(True, axis="y")

# -----------------------------
# Linee verticali centrate tra HASH
# -----------------------------
pos = 0
for h in hashes[:-1]:
    ops_in_hash = stats.loc[stats["HASH"] == h, "Operazione"].unique()
    block_width = len(ops_in_hash)
    pos += block_width
    line_pos = pos + (gap/2) - 0.5
    ax.axvline(line_pos, color="gray", linestyle="--", alpha=0.5)
    pos += gap

# -----------------------------
# Etichette HASH sotto tick
# -----------------------------
pos = 0
for h in hashes:
    ops_in_hash = stats.loc[stats["HASH"] == h, "Operazione"].unique()
    block_width = len(ops_in_hash)
    center = pos + (block_width - 1)/2
    ax.text(
        center,
        -0.30,  # puoi regolare se vuoi più sotto
        h,
        transform=ax.get_xaxis_transform(),
        ha="center",
        va="top",
        fontsize=11,
        fontweight="bold"
    )
    pos += block_width + gap

# -----------------------------
# Legenda e layout
# -----------------------------
ax.legend(title="Elements")
ax.set_yscale("log")
plt.tight_layout()
plt.subplots_adjust(bottom=0.38)
plt.savefig("latency_by_operation_hash_strategy.pdf")
plt.close()