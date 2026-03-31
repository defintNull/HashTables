# Hashtable Testing Code Graphs

La cartella contiene il codice in Python responsabile per l'analisi e la realizzazione dei grafici.

---

## Struttura della cartella

    .
    ├── hybridTest/            # Grafici per dati ibridi (chiavi real_word, valori int)
    │   ├── DoubleTest/        # Grafici relativi al test al raddoppio
    │   ├── hashTest/          # Grafici relativi alle singole istanze
    │   └── opTest/            # Grafici relativi alle singole operazioni delle singole istanze
    │
    ├── intTest/               # Grafici per dati interi (chiavi int, valori int)
    │   └── Come per hybridTest
    │
    ├── mixTests/              # Grafici per confronto tra tipi di dato differenti
    │   └── Come per hybridTest
    │
    ├── realWordTest/          # Grafici per real_word (chiavi real_word, valori real_word)
    │   └── Come per hybridTest
    │
    ├── stringTest/            # Grafici per stringhe (chiavi string, valori string)
    │   └── Come per hybridTest
    │
    ├── venv/                  # Virtual environment Python
    │
    ├── grapher_hash.py        # Grafici relativi ai dati delle singole istanze
    │
    ├── grapher_mix_hash.py    # Grafici relativi a dati di tipi diversi (confronto)
    │
    ├── grapher_mix_op.py      # Grafici relativi alle operazioni con istanze diverse
    │
    ├── grapher_op.py          # Grafici relativi alle operazioni per tipo di dato
    │
    ├── requirements.txt       # Requisiti Python
    │
    └── README.md

---

## Utilizzo

### 0. Prerequisiti

È necessario aver installato e configurato:

- Python3 ≥ 3.12.0

---

### 1. Configurare il venv

1. Creare il venv per Python:

```bash
python3 -m venv venv
```

2. Attivare il venv:

```bash
source venv/bin/activate
```

3. Installare i pacchetti necessari:

```bash
pip install -r requirements.txt
```

---

### 2. Realizzare i grafici

La procedura è valida per le 4 tipologie di dato (`intTest`, `realWordTest`, `stringTest`, `hybridTest`) e per la categoria mista `mixTests`.
Procedimento analogo anche per realizzare i grafici per i campioni del test al raddoppio.

1. Attivare il venv:

```bash
source venv/bin/activate
```

2. Posizionarsi nella cartella desiderata, ad esempio:

```bash
cd intTest
```

3. Per i dati relativi all'istanza:  
   Posizionarsi nella cartella `hashTest` ed eseguire il grapher per i grafici di istanza, regolando il path del CSV nel file Python se necessario:

```bash
cd hashTest
python3 ../../grapher_hash.py
```

4. Per i dati relativi alle operazioni dell'istanza:  
   Posizionarsi nella cartella `opTest` ed eseguire il grapher per i grafici di operazione, regolando il path del CSV nel file Python se necessario e il parametro double nel test op:

```bash
cd opTest
python3 ../../grapher_op.py
```

La stessa procedura vale per le cartelle `realWordTest`, `stringTest`, `hybridTest` e `mixTests`, regolando il file Python desiderato e il path dei CSV all'interno del file.

---