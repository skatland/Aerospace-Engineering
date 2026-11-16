# Leksjon 0: byggeprosessen

**Pensum:** Lecture 7, lysbilde 3-12 (Building Process, GNU/ARM toolchain, Flasher).
**Mål:** forstå hva som skjer mellom at du skriver C-kode og at den kjører på brikken.

## Teori

Prosessoren forstår bare maskinkode. Veien dit har faste steg (Lecture 7):

```
main.c ──preprosessor──► kompilator ──► assembler ──► main.o ─┐
startup.c ─────────────────────────────────────────► startup.o ├─ linker ──► main.elf ──► binærfil ──► flasher ──► flash
uart.c ────────────────────────────────────────────► uart.o   ┘     ▲
                                                              linker-script (hvor ting skal ligge)
```

- **Preprosessor:** behandler `#include` og `#define`.
- **Kompilator:** C blir assembly. **Assembler:** assembly blir maskinkode i en objektfil (`.o`).
- **Linker:** slår sammen alle objektfiler og legger til oppstartskoden. **Linker-scriptet** bestemmer minnekartet.
- **ELF** er filformatet. Typiske deler (sections): `.text` (instruksjoner), `.data` (globale variabler med startverdi),
  `.bss` (globale variabler uten startverdi).
- **Flasher:** skriver binærfilen inn i brikkens flash. Her slipper vi det, QEMU laster ELF-filen direkte.

Verktøynavnet `arm-none-eabi-gcc` leses: `arm` = arkitektur, `none` = ingen operativsystem (bare-metal),
`eabi` = ABI, `gcc` = verktøyet. `gcc` kjører alle stegene over i én kommando.

## Kjør startkoden

```
cd lessons/00_build
make run
make sections
make symbols
```

I `make sections` ser du `.text`, `.data` og `.bss` med størrelse og adresse. I `make symbols` finner du
`answer`, `empty` og `msg`, og adressen hver av dem har.

## Oppgaver

1. **TODO 1** i `main.c`: skriv ut verdien til `answer`. (Husk `uart_put_dec()` og en ny linje.)
2. **TODO 2:** legg til `int big[100];` og kjør `make sections` før og etter. Hvilken del vokser, og med hvor mye?
3. Se hvert steg for seg (valgfritt, men lærerikt):
   ```
   arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -I../../common -E main.c | tail -20     # etter preprosessoren
   arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -I../../common -S main.c -o main.s      # assembly
   arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -I../../common -c main.c -o main.o      # objektfil
   arm-none-eabi-objdump -t main.o                                                  # symboler (adresser er ikke satt ennå)
   ```
4. Åpne `main.map` etter bygging. Finn hvor linkeren la `.text`, `.data` og `.bss`.

## Kontrollspørsmål

1. I hvilken del havner `int answer = 42;`? `int empty;`? `const char msg[] = "..."`?
2. Hva bestemmer linker-scriptet?
3. Hva er forskjellen på en objektfil og en ELF-fil som kan kjøres?
4. Hvorfor må en flasher brukes på ekte maskinvare?

## Fasit

1. `answer` i `.data`, `empty` i `.bss`, `msg` i flash (skrivebeskyttet data, i vårt script lagt sammen med `.text`).
2. Hvor i minnet hver del skal ligge (flash eller RAM) og start-/sluttadresser. Det er hele minnekartet.
3. Objektfilen er én kompilert kildefil med adresser som ikke er satt ennå (relocatable). Den kjørbare filen er
   resultatet etter linking, med faste adresser.
4. Flash er permanent minne som ikke kan skrives som vanlig RAM. En flasher-programvare (lastet inn i SRAM først)
   skriver binærbildet inn i flash (Lecture 7, lysbilde 12).
