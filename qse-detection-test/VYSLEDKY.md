# Vysledky testu detekce - Guardium Quantum Safe Explorer

Skenovano: verze Explorer ......  datum ......
Rezim skenu: [ ] Cryptography Analysis   [ ] API Discovery   (spustit oba zvlast)

| Soubor | Nalez? | Algoritmus | Severity | Compliance | Poznamka |
|---|---|---|---|---|---|
| 01_include_only.c    |  |  |  |  | ocekavano: zadny nalez |
| 02_real_call.c       |  |  |  |  | kontrolni vzorek |
| 03_dead_code.c       |  |  |  |  | reachability |
| 04_ifdef_disabled.c  |  |  |  |  | build config |
| 05_dynamic_alg.c     |  |  |  |  | ocekavano: Unknown |
| 06_strings_only.c    |  |  |  |  | ocekavano: zadny nalez |

## Kontrola CBOM zvlast
- Objevila se OpenSSL jako *komponenta* v CBOM i pro 01_include_only.c?  ANO / NE
- Pokud ano: komponenta v CBOM != nalez (occurrence). Pri demu odlisit.

## Zaver pro zakaznika
(doplnit)
