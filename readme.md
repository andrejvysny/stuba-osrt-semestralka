# Operácie s množinou čísel

---

Program má ako vstup súbor s 10 000 číslami, server a klienti sa vytvoria pomocov `fork()` funkcie a bežia ako procesy, následne si klienti od servera cez **sockety** vypýtajú vstupný súbor, načítajú vstupné údaje, spravia operácie s číslami, výsledok pošlú serveru cez **sockety**, ktorý ho zapíše do výstupu vo vlákne. Pred ukončením klient pošle serveru signál o ukončení, server následne aktualizuje **Shared Memory Segment** s počtom klientov, ktorí bežia. Ak sa počet klientov rovná 0 server sa vypne. 

### Operácie: 
- Minimálna hodnota
- Maximálna hodnota
- Preimerná hodnota
- Súčet hodnôt

---

## Kompilácia
```bash
make 
```

## Spustenie
```bash
make run
```

---
## Dodatočné funkcie:
### Generovanie vstupu
Vygeneruje súbor input.txt s 10 000 číslami v rozmedzí -50 000 až 50 000
```bash
make input
```

### Vyčistenie 
Odstránenie output.txt a spustiteľných súborov
```bash
make clean
```
