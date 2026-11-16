# Cortex-M3/M4: fra null til hero

Et lite kurs i embedded-programmering, bygget rundt pensum i Embedded Systems (Lecture 3-7).
Du lærer både *hvordan koden settes opp* (startup, linker, vektortabell, drivere) og *det du skal
kunne til eksamen og innleveringen*. Alt kjører i en emulator (QEMU), så du trenger ikke noe kort.

> Viktig: kurset er skrevet uten at jeg har kunnet kjøre det selv (verktøyene finnes ikke der det
> ble laget). Hvis noe ikke kompilerer eller gir en annen utskrift enn forventet, er det en feil i
> kurset, ikke i deg. Si fra, så rettes det.

## Slik jobber du i hver leksjon

1. Les `README.md` i leksjonsmappen (kort teori med lysbildehenvisninger).
2. Kjør startkoden: `make run`. Se hva den gjør (noen leksjoner viser FAIL eller henger med vilje).
3. Åpne `main.c` (i leksjon 3 også `startup.c`) og gjør `TODO`-ene i rekkefølge.
4. Bygg og kjør igjen etter hver TODO. Se at utskriften endrer seg.
5. Svar på kontrollspørsmålene *før* du ser på fasiten nederst.
6. Står du fast: `make solution` kjører modelløsningen. Kikk i `solution/`.

Avslutt emulatoren med **Ctrl+A**, slipp, og trykk **X**.

## Oppsett (én gang, Mac)

```
brew install arm-none-eabi-gcc qemu
```

Åpne denne mappen i VS Code (File → Open Folder), åpne terminalen, og gå til en leksjon:

```
cd lessons/00_build
make run
```

## Kommandoer som finnes i alle leksjoner

| Kommando | Hva den gjør |
|---|---|
| `make` | bygger `main.elf` |
| `make run` | bygger og kjører i QEMU |
| `make solution` | bygger og kjører modelløsningen |
| `make sections` | viser størrelsen på `.text`, `.data`, `.bss` |
| `make symbols` | viser alle symboler og adressene deres |
| `make vectors` | viser vektortabellen (første ord i flash) |
| `make disasm` | viser maskinkoden blandet med C-koden |
| `make clean` | sletter bygde filer |

## Kursoversikt

| # | Leksjon | Pensum | Innleveringsoppgave |
|---|---|---|---|
| 0 | Byggeprosessen | Lecture 7, lysbilde 3-12 | (grunnlag) |
| 1 | Minnekart og minnetilgang | Lecture 5, lysbilde 2, 7-12; Lecture 3, lysbilde 8 | Task 2, Task 5 |
| 2 | Registre og bits | Lecture 4 | (grunnlag) |
| 3 | Oppstart: vektortabell og Reset Handler | Lecture 7, lysbilde 6-7 og 29; Lecture 6 | Task 10 |
| 4 | Stakken (full descending) | Lecture 4, Lecture 5, lysbilde 13-17 | Task 4 |
| 5 | Exceptions og interrupts | Lecture 6 | Task 6, Task 7 (og ICI i Task 5) |
| 6 | Privilegert og uprivilegert tilgang | Lecture 4, lysbilde 3; Lecture 5, lysbilde 18 | Task 3 |
| 7 | Programvarestruktur: lag, polling mot interrupt | Lecture 7, lysbilde 13-23 | Task 8, Task 9 |

Leksjonene bygger på hverandre. Ta dem i rekkefølge.

## Teori uten kode (les selv)

Disse delene av pensum har ikke noen kodeleksjon, men du må kunne dem. Fra Lecture 3, lysbilde 8:

- M3 kom i 2005, M4 i 2010. Begge er 32-bit RISC-prosessorer med 32-bit registre, 32-bit intern databuss
  og 32-bit busgrensesnitt. 32-bit adressering gir 4 GB adresserom.
- **Tre-stegs pipeline**: instruction fetch, decode, execute (Task 1).
- **Harvard bus architecture with unified memory space**: egne busser for instruksjoner og data, men ett felles
  adresserom (Task 2). Du ser det i leksjon 1: flash, RAM og UART er bare adresser i samme kart.
- **Load-store-arkitektur** (Task 5). Du ser det i leksjon 1 med `make disasm`.
- NVIC støtter opptil 240 interrupt-forespørsler og 8 til 256 prioritetsnivåer (Lecture 3, lysbilde 10).
- Cortex-familien har tre profiler: A (applikasjoner med OS), R (sanntid), M (mindre applikasjoner) (lysbilde 5).

Lecture 1 og 2 og ECSS-håndboken er ikke dekket av dette kurset.

## Mappestruktur

```
common/            delt kode: uart, startup, linker-script, Makefile-regler
lessons/NN_navn/   én mappe per leksjon (README.md, main.c, solution/)
```
