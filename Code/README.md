# Hashtable Testing Code

La cartella contiene il codice responsabile della realizzazione delle hashtable, del test della correttezza, dell'analisi dei dati e della generazione dei grafici.

---

## Struttura della cartella

    .
    ├── build/                  # Cartella per la build dei file C++
    │
    ├── graphs/                 # Generazione dei grafici
    │
    ├── hybridTest/             # Test con dati ibridi (chiavi real_word, valori int)
    │   ├── build/                  # Cartella per build
    │   ├── hash_data.csv           # Raccolta dati relativi a singole esecuzioni con differenti parametri
    │   ├── hybridTest.cpp          # File C++ responsabile del test per il tipo specificato andando a potenze di 10 per la dimensione dell'input
    │   ├── hybridTestDouble.cpp    # File C++ responsabile del test per il tipo specificato andando al raddopio della dimensione dell'input
    │   └── op_data.csv             # Raccolta dati relativi a singole operazioni
    │
    ├── intTest/                # Test con dati interi (chiavi int, valori int)
    │   └── Come per hybridTest
    │
    ├── realWordTest/           # Test con real_word (chiavi real_word, valori real_word)
    │   └── Come per hybridTest
    │
    ├── stringTest/             # Test con stringhe (chiavi string, valori string)
    │   └── Come per hybridTest
    │
    ├── correctness_output.txt  # Output del test di correttezza
    │
    ├── correctness.cpp         # Test di correttezza delle hashtable
    │
    ├── HashTable.h             # Implementazione template della hashtable
    │
    ├── HashTable.tpp           # Implementazione delle funzioni template
    │
    ├── repo.txt                # Repository utilizzate nel progetto
    │
    ├── words.txt               # Lista di parole (real words)
    │
    └── README.md

---

## Utilizzo

### 0. Prerequisiti

È necessario aver installato e configurato:

- GCC ≥ 13.0
- Python3 ≥ 3.12.0

Per la realizzazione dei grafici consultare il README nella cartella `graphs`.

---

### 1. Test di correttezza

1. Compilare il file `correctness.cpp`:

```bash
g++ -O3 -march=native -flto -std=c++20 correctness.cpp -o ./build/correctness
```

2. Eseguire il test di correttezza:

```bash
./build/correctness
```

Al termine dell'esecuzione, sarà possibile analizzare direttamente da terminale eventuali errori riscontrati durante il test.

---

### 2. Test per tipologia di dato

La procedura è valida per ognuna delle 4 tipologie di dato (`intTest`, `realWordTest`, `stringTest`, `hybridTest`).

1. Posizionarsi all'interno della cartella relativa al tipo di dato:

```bash
cd intTest
```

2. Compilare i file di test:

```bash
g++ -O3 -march=native -flto -std=c++20 intTest.cpp -o ./build/intTest
g++ -O3 -march=native -flto -std=c++20 intTestDouble.cpp -o ./build/intTestDouble
```

3. Eseguire i test per ottenere i CSV con i dati:

```bash
./build/intTest
./build/intTestDouble
```

La stessa procedura vale per le cartelle `realWordTest`, `stringTest` e `hybridTest`.

---