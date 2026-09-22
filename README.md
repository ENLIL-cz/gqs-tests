# gqs-tests

Ověření chování statického skenu **IBM Guardium Quantum Safe Explorer** na kontrolovaném vzorku.

Cílem bylo zodpovědět konkrétní otázku: **je detekce založená na skutečném volání funkce, nebo stačí knihovnu naimportovat?** A navazující otázku, kde je hranice statické analýzy oproti runtime pohledu.

| | |
|---|---|
| Verze nástroje | IBM Quantum Safe Explorer 2.3.2.2 |
| Režim skenu | API Discovery scan |
| Datum | 2026-09-22 |
| Rozsah | 6 souborů, 65 řádků, jazyk C (OpenSSL) |

---

## Obsah repozitáře

| Složka | Popis |
|---|---|
| `qse-detection-test/` | Zdrojové soubory testu — 6 minimálních C souborů, každý izoluje jednu proměnnou detekce |
| `qs_scan_result/` | Surové výstupy skenu (findings, CBOM, crypto inventory, report) |
| `qs_explorer_result/` | Výstupy z Explorer / VS Code rozhraní |

---

## Metodika

Šest souborů se liší vždy jen v jedné věci. Všechny používají OpenSSL, všechny mají stejnou strukturu, takže rozdíl ve výsledku jde přiřadit jedné příčině.

| # | Soubor | Co testuje |
|---|---|---|
| 01 | `01_include_only.c` | Tři `#include` z OpenSSL, **nula volání** |
| 02 | `02_real_call.c` | Reálné RSA-2048 keygen volání — kontrolní vzorek |
| 03 | `03_dead_code.c` | RSA-1024 keygen ve funkci, kterou nikdo nevolá |
| 04 | `04_ifdef_disabled.c` | 3DES ve větvi `#ifdef`, která se nikdy nekompiluje |
| 05 | `05_dynamic_alg.c` | Šifra určená až za běhu z `argv[1]` |
| 06 | `06_strings_only.c` | „RSA-2048", „SHA-1", „TLS_RSA_WITH_..." pouze v komentářích a log stringech |

---

## Výsledky

| # | Soubor | Nálezy | Poznámka |
|---|---|---|---|
| 01 | include_only | **0** | Soubor prošel parserem (`Files_scanned: 6`), přesto nula nálezů |
| 02 | real_call | 3 | `EVP_PKEY_keygen_init`, `EVP_PKEY_CTX_set_rsa_keygen_bits`, `EVP_PKEY_keygen` |
| 03 | dead_code | 3 | Stejná trojice, přestože funkce není odnikud volána |
| 04 | ifdef_disabled | 4 | Včetně `EVP_des_ede3_cbc` označeného jako CWE-327, severity high |
| 05 | dynamic_alg | 3 | Volání zachycena, ale algoritmus neurčen — FIPS 140-3 i Quantum-safe `Unknown` |
| 06 | strings_only | **0** | Žádná shoda na řetězcích v komentářích |

Celkem 13 kryptografických artefaktů, všechny správně přiřazené ke knihovně `openSSL`, každý s číslem řádku a sloupce.

---

## Závěry

### 1. Detekce je na úrovni volání, ne importu

Soubor 01 má naincludované `evp.h`, `rsa.h` i `sha.h` a nevygeneroval jediný nález — přitom statistika potvrzuje, že soubor byl skutečně naparsován, ne přeskočen. Soubor 06 neuhnul ani na krypto názvy v komentářích a log stringech.

**Naimportovaná knihovna, ze které se nic nevolá, nálezy negeneruje.**

### 2. Reachability nástroj neřeší

Případy 03 a 04 nálezy vyprodukovaly, přestože se ten kód za běhu nikdy nespustí:

- mrtvý kód (nevolaná funkce) → 3 nálezy
- vypnutá `#ifdef` větev → 4 nálezy, včetně 3DES se severity high

To je systémová vlastnost statické analýzy, ne vada. Nástroj umí říct *„na tomto řádku se volá 3DES"*, neumí říct *„tento řádek se do binárky nedostane"*. Prakticky to znamená, že u projektů s více krypto backendy pod podmíněnou kompilací (typicky OpenVPN a `crypto_openssl.c` vs `crypto_mbedtls.c`) je potřeba počítat s nálezy z obou větví bez ohledu na build konfiguraci.

### 3. Runtime hodnoty zůstávají neurčené — a nástroj to přiznává

V případě 05 se šifra bere z `argv[1]`. Volání se zachytila, ale samotné `EVP_get_cipherbyname()` mezi artefakty není a algoritmus zůstal `Unknown` místo aby se něco domýšlelo. Férové chování, ale pro aplikace beroucí šifru z konfigurace platí, že statika neřekne, co se reálně používá. Tuhle mezeru zavírá až runtime vrstva (network a endpoint skeny v Guardium Quantum Safe), ta zase neřekne, kterou funkci vývojář zavolal.

---

## Otevřené body

- **Hlavička výsledků ukazuje „13 Quantum-safe" při 13 artefaktech**, přestože mezi nimi je RSA keygen a 3 vulnerabilities, a detail jednotlivé položky uvádí Quantum-safe `Unknown`. Chování k ověření u IBM.
- **Cryptography Analysis scan zatím neproběhl.** Aktuální data jsou pouze z API Discovery. Ve výstupech chybí délky klíčů — soubor 02 používá RSA-2048, soubor 03 RSA-1024 a ani jeden výstup to nerozlišuje. Parameter tracing, který by to měl doplnit, je součástí druhého režimu skenu. Doplníme.

---

## Reprodukce

```bash
git clone https://github.com/ENLIL-cz/gqs-tests.git
cd gqs-tests
```

Adresář `qse-detection-test/` naskenujte jako **samostatný projekt**. Nemíchejte ho s větším repozitářem — celý test stojí na tom, že nálezy jdou jednoznačně přiřadit k jednotlivým souborům.

Ve výstupu sledujte jedinou věc: **která z těch šesti jmen souborů se objeví v nálezech.** Severity, skóre ani počty řádků pro tenhle test roli nehrají.

Soubory nejsou kompilovatelné bez OpenSSL hlaviček. Statický sken je kompilovat nepotřebuje; pokud by sken hlásil chyby parsování, doinstalujte `openssl-devel` (RHEL) nebo `libssl-dev` (Ubuntu).
