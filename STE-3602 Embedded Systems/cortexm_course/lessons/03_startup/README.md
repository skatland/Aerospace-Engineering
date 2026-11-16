# Leksjon 3: oppstart, vektortabell og Reset Handler

**Pensum:** Lecture 7, lysbilde 6-7 (ELF, linker) og lysbilde 29 (Startup Sequence); Lecture 6 (vektortabellen).
**Innlevering:** Task 10.
**Mål:** skrive oppstartskoden selv, slik at du forstår hvert steg i Task 10.

## Teori

Ved reset skjer dette automatisk i maskinvaren (lysbilde 29):

1. Verdien på adresse `0x00000000` lastes inn i **SP**.
2. Verdien på adresse `0x00000004` lastes inn i **PC**.

Dette kalles reset-sekvensen. Adressene 0 og 4 er de to første ordene i **vektortabellen**, en liste i flash
med startadressene til alle exception-handlere (Lecture 6, lysbilde 4). PC peker nå på `Reset_Handler`.

| Plass | Adresse | Innhold |
|---|---|---|
| 0 | 0x00 | startverdi for SP |
| 1 | 0x04 | Reset |
| 2 | 0x08 | NMI |
| 3 | 0x0C | HardFault |
| 11 | 0x2C | SVCall |
| 14 | 0x38 | PendSV |
| 15 | 0x3C | SysTick |

(De andre plassene er fault-handlere og reserverte. Fullstendig liste: *The Definitive Guide*, kapittel 8.)

Adressene til funksjoner i tabellen er **oddetall**. Biten lengst til høyre er Thumb-biten og må være 1 (Cortex-M kjører
bare Thumb-instruksjoner). Det er normalt, ikke en feil.

**Reset Handler** (lysbilde 29) gjør typisk:

1. Sette stakkpekeren på nytt (her allerede gjort av maskinvaren).
2. Kopiere `.data` fra flash til RAM.
3. Nulle `.bss` i RAM.
4. Kalle `SystemInit()` (klokker og grunninnstilling).
5. Kalle `main()`.

**Hvorfor?** C-språket lover at `int x = 5;` starter som 5 og at `int y;` starter som 0. På en brikke er RAM tilfeldig
etter oppstart, og flash holder startverdiene. Så noen må kopiere og nulle før `main()` får kjøre. Linkeren lager
symbolene som markerer områdene: `_sidata` (kilde i flash), `_sdata`/`_edata` (`.data` i RAM), `_sbss`/`_ebss`
(`.bss` i RAM). Se `common/linker.ld`.

## Kjør startkoden

```
cd lessons/03_startup
make run
```

Du får sannsynligvis **ingenting** ut (eller en feilmelding fra QEMU). Det er riktig: vektortabellens plass 1 er 0, så PC får verdien 0.
Kjør `make vectors`:

```
Contents of section .isr_vector:
 0000 00000120 ...
```
Ordene vises byte for byte med laveste byte først (little endian, leksjon 1), så `00 00 01 20` betyr `0x20010000`.
Plass 1 skal ikke være null.

## Oppgaver

1. **TODO 1** (`startup.c`): sett inn `Reset_Handler` på plass 1. Kjør `make vectors` og se at adressen ikke lenger er 0,
   og er et oddetall. Kjør `make run`. Nå skal du få utskrift, men med `FAIL`.
2. **TODO 2:** kopier `.data`. Hvilke `FAIL` forsvinner?
3. **TODO 3:** null `.bss`. Alle skal nå være `PASS`.
4. Eksperimenter: kommenter ut TODO 3 og kjør. Hvorfor ser du `0xA5A5A5A5`-mønstre? (`scramble_ram` simulerer
   tilfeldig RAM, og i emulatoren ville RAM ellers være 0 og skjule feilen.)
5. `make symbols | grep -E "_sidata|_sdata|_edata|_sbss|_ebss|_estack"` og se adressene. Hvorfor er `_sidata` i flash og
   `_sdata` i RAM?

## Kontrollspørsmål

1. Hva er de to første trinnene i reset-sekvensen?
2. Hvorfor må `.data` kopieres, men `.bss` bare nulles?
3. Hva skjer hvis `main()` kalles før kopieringen?
4. Hvorfor er `Reset_Handler`-adressen i tabellen et oddetall?
5. Hvorfor kan ikke `Reset_Handler` returnere?

## Fasit

1. Verdien på 0x0 → SP, verdien på 0x4 → PC.
2. `.data` har startverdier som må hentes fra et sted (flash). `.bss` skal alltid starte som 0, så det er ingenting å kopiere.
3. Variabler med startverdi har tilfeldige verdier, så programmet oppfører seg feil.
4. Thumb-biten (bit 0) er 1.
5. Det finnes ingenting å returnere til, siden ingen kalte den. Etter `main()` bruker vi en evig løkke.
