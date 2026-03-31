import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

reps = 5

# -----------------------------
# Leggi il CSV
# -----------------------------
path = "../../../intTest/hash_data.csv"
df = pd.read_csv(path)

# -----------------------------
# Imposta stile
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
# Latenza / Overflow
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    df.groupby(["OVERFLOW", "Elements"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = sorted(stats["OVERFLOW"].unique())
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["OVERFLOW"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Overflow Strategy")
plt.ylabel("Average Latency [s]")
plt.title("Latency by Overflow Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("latency_by_overflow_strategy.pdf")
plt.close()



# -----------------------------
# Latenza / Resize
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    df.groupby(["RESIZE", "Elements"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = sorted(stats["RESIZE"].unique())
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["RESIZE"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Resize Strategy")
plt.ylabel("Average Latency [s]")
plt.title("Latency by Resize Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("latency_by_resize_strategy.pdf")
plt.close()



# -----------------------------
# Latenza / Max_Load_Factor
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
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    df.groupby(["category", "Elements"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = ["Low", "Standard", "High"]
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["category"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Max Load Factor")
plt.ylabel("Average Latency [s]")
plt.title("Latency by Max Load Factor Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("latency_by_max_load_factor_strategy.pdf")
plt.close()



# -----------------------------
# Memoria / Overflow
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    df.groupby(["OVERFLOW", "Elements"])["Memoria"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = sorted(stats["OVERFLOW"].unique())
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["OVERFLOW"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Overflow Strategy")
plt.ylabel("Average memory [B]")
plt.title("Memory by Overflow Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("memory_by_overflow_strategy.pdf")
plt.close()



# -----------------------------
# Memoria / Resize
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    df.groupby(["RESIZE", "Elements"])["Memoria"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = sorted(stats["RESIZE"].unique())
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["RESIZE"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Resize Strategy")
plt.ylabel("Average memory [B]")
plt.title("Memory by Resize Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("memory_by_resize_strategy.pdf")
plt.close()



# -----------------------------
# Memory / Max_Load_Factor
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
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    df.groupby(["category", "Elements"])["Memoria"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = ["Low", "Standard", "High"]
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["category"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Max Load Factor")
plt.ylabel("Average Memory [B]")
plt.title("Memory by Max Load Factor Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("memory_by_max_load_factor_strategy.pdf")
plt.close()




# -----------------------------
# Latenza / Hash
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    pd.concat([
        df[(df["HASH"] == "MODE_1") & (df["OVERFLOW"] == "OPEN_ADDRESSING_LINEAR_PROBING")],
        df[df["HASH"] == "MODE_2"]
    ])
    .groupby(["HASH", "Elements"])["Latenza"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = sorted(stats["HASH"].unique())
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["HASH"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Hash Strategy")
plt.ylabel("Average Latency [s]")
plt.title("Latency by Hash Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("latency_by_hash_strategy.pdf")
plt.close()



# -----------------------------
# Memory / Hash
# -----------------------------
df = pd.read_csv(path)

# -----------------------------
# Calcola media, min e max per gruppo
# -----------------------------
stats = (
    pd.concat([
        df[(df["HASH"] == "MODE_1") & (df["OVERFLOW"] == "OPEN_ADDRESSING_LINEAR_PROBING")],
        df[df["HASH"] == "MODE_2"]
    ])
    .groupby(["HASH", "Elements"])["Memoria"]
    .agg(["mean", "min", "max"])
    .reset_index()
)

# -----------------------------
# Crea la figura
# -----------------------------
plt.figure(figsize=(10,6))
colors = plt.cm.tab10.colors

# -----------------------------
# Plotta ogni gruppo di 'Elements' con spostamento
# -----------------------------
x_labels = sorted(stats["HASH"].unique())
n_groups = len(stats["Elements"].unique())
width = 0.15  # quanto spostare a destra/sinistra

for i, elements in enumerate(sorted(stats["Elements"].unique())):
    subset = stats[stats["Elements"] == elements]

    # trasforma nomi in numeri
    x_num = [x_labels.index(x_val) for x_val in subset["HASH"]]

    # applica lo spostamento (dodge)
    offset = (i - (n_groups-1)/2) * width
    x_dodge = [x + offset for x in x_num]

    y = subset["mean"]
    err_low = y - subset["min"]
    err_high = subset["max"] - y

    plt.errorbar(
        x_dodge,
        y,
        yerr=[err_low, err_high],
        fmt="o",
        label=f"Elements = {elements}",
        color=colors[i % len(colors)],
        capsize=4
    )

    # -----------------------------
    # Aggiungi etichette della media
    # -----------------------------
    y_range = stats["max"].max() - stats["min"].min()
    base_offset = y_range * 0.02

    # direzione per questo gruppo (alterna tra gruppi)
    direction = 1 if i % 2 == 0 else -1

    for xi, yi in zip(x_dodge, y):
        plt.text(
            xi,
            yi + direction * base_offset,
            f"{yi:.3g}",
            ha="center",
            va="bottom" if direction > 0 else "top",
            fontsize=8
        )

# -----------------------------
# Migliora leggibilità
# -----------------------------
plt.xticks(range(len(x_labels)), x_labels, rotation=30, ha="right")
plt.xlabel("Hash Strategy")
plt.ylabel("Average Memory [B]")
plt.title("Memory by Hash Strategy")
plt.grid(True, axis="y")
plt.legend()

# -----------------------------
# Salva il grafico
# -----------------------------
plt.tight_layout()
plt.savefig("memory_by_hash_strategy.pdf")
plt.close()



print("Grafico salvato!")