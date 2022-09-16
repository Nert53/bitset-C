# Bitsets in C

V jazyce C navrhněte strukturu Bitset která bude uchovávat libovolně velké množiny čísel. Při implementaci funkcí smíte použít pouze knihovny ```stdlib.h``` a ```stdio.h```.

#### Alokace
```c
// Alokace prazdné množiny o velikosti size.
// Pokud tedy chceme vytvorit mnozinu pro prvku 0-15 tak size = 16.
Bitset* create_bitset(size_t size);
    
// Alokace množiny o velikosti size a obsahující hodnoty values.
Bitset* create_bitset_with_values(size_t size, const int *values, size_t array_size);
    
// Alokace mnoziny o velikosti size a obsahujici hodnoty 0 az upto - 1
Bitset* create_bitset_with_range(size_t size, int upto);
```
#### Základní operace
```c
// Vlozi prvek do mnoziny
void set_insert(Bitset *bitset, int element);
    
// Odebere prvek v z mnoziny
void set_remove(Bitset *bitset, int element);
    
// Funkce pro test, zda je prvek v mnozine. 
// Funkce vraci 1 pokud prvek v mnozine je a jinak 0
int contains(Bitset *bitset, int element);
```
#### Pokročilé operace
```c
// Prunik dvou mnozin kde výsledek pruniku je ulozen do mnoziny left
void form_intersection(Bitset *left, Bitset *right);
    
// Prunik dvou mnozin kde výsledek pruniku je vracen jako nova mnozina
Bitset* set_intersection(Bitset *left, Bitset *right);
    
// Sjednoceni dvou mnozin kde výsledek sjedniceni je ulozen do mnoziny left
void form_union(Bitset *left, Bitset *right);
    
// Sjednoceni dvou mnozin kde výsledek sjednoceni je vracen jako nova mnozina
Bitset* set_union(Bitset *left, Bitset *right);
    
// Rozdil dvou mnozin kde výsledek rozdilu je ulozen do mnoziny left
void form_difference(Bitset *left, Bitset *right);
    
// Rozdil dvou mnozin kde výsledek rozdilu je vracen jako nova mnozina
Bitset* set_difference(Bitset *left, Bitset *right);
    
// Test podmnoziny vraci 1 pokud je left podmnozinou right jinak 0.
int is_subset(Bitset *left, Bitset *right);
```
Pokud operace obdrží různě velké množiny jako svoje argumenty, tak se funkce zachovají následovně. V případě funkcí `form_intersection`, `form_union` a `form_difference` platí, že velikost množiny left pouze zvětšujeme pokud je v pravidlech, že se má množina zmenšit, tak to platí pouze pro funkce vracející novou množinu.

- ***Průnik*** - Pokud je jedna, množina větší, pak výsledek bude mít velikost menší množiny.
- ***Sjednocení*** - Pokud je jedna množina menší, pak výsledek bude mít velikost větší množiny
- ***Rozdíl*** - Pokud size(left) > size(right), pak vysledek má velikost size(left). Pokud size(left) < size(right), pak je velikost výsledku size(left).

#### Práce se soubory

Funkce `load_bitset_if` musí být schopna zpracovat libovolně velké soubory..


```c
// Mějme množinu A = {1, 3, 5, 7, 9, 20}
// Do souboru ji zapišeme takto:
1 3 5 7 9 2
```

```c
// Ulozi pole mnozin do souboru kde kazda mnozina je na jednom radku
// Funkce vrací 0 pokud se zápis zdařil, jinak vrací jednu z následujících hodnot
// 1 - Soubor se nepovedlo otevřít
// 2 - Nebylo možné zapsat hodnoty do souboru
// 3 - Soubor se nepovedlo uzavřít 
int save_bitsets_to_file(char *file, Bitset **bitsets, size_t bitsets_count)

// Nacte n mnozin ze souboru ulozenych funkci save_bitsets_to_file
Bitset** load_bitsets(char *file)

// Nacte ze souboru pouze mnoziny splnujici predikat condition
// Tato funkce neni soucasti reseni (!)
Bitset** load_bitsets_if(int (*condition)(Bitset*), char *file);
```
