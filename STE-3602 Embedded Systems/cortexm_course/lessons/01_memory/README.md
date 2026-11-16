# Leksjon 1: minnekart og minnetilgang

**Pensum:** Lecture 5, lysbilde 2 (Pre-Defined Memory Map), 7 (endianness og alignment), 8-12 (bit-band);
Lecture 3, lysbilde 8 (Harvard bus architecture with unified memory space, load-store).
**Innlevering:** Task 2 (Harvard mot von Neumann) og Task 5 (load-store).

## Teori

**Ett adresserom.** Cortex-M bruker 32-bit adresser, altså 4 GB. Programkode, data, periferienheter og
systemregistre ligger i samme adresserom, bare på forskjellige adresser. Typiske områder (sjekk mot figuren på
lysbilde 2):

| Fra adresse | Hva |
|---|---|
| `0x0000_0000` | kode (flash) |
| `0x2000_0000` | SRAM |
| `0x4000_0000` | periferienheter |
| `0xE000_0000` | systemregistre (SysTick, NVIC, SCB) |

**Minnestyrte registre.** Å styre maskinvare betyr å lese og skrive bestemte adresser. UART-en vi skriver til
ligger på `0x4000C000`. Det er derfor `uart_putc` bare er en skriving til en adresse.

**Makroen `REG32`:**

```c
#define REG32(addr) (*(volatile uint32_t *)(addr))
```
Les fra innsiden og ut: `(addr)` er et tall. `(volatile uint32_t *)` sier "se på tallet som adressen til et
32-bit ord". `*` foran sier "verdien på den adressen". Så `REG32(0x20008000) = 5;` skriver 5 til den adressen.
`volatile` forteller kompilatoren at hver lesing og skriving må gjøres akkurat som skrevet (den kan ikke
hoppe over eller slå sammen dem, for maskinvaren kan reagere på hver av dem).

**Endianness (lysbilde 7).** Et 32-bit ord er 4 byte. Cortex-M støtter begge rekkefølger, men vanlig bruk er
*little endian*: laveste byte ligger på laveste adresse. `0xDEADBEEF` blir i minnet `EF BE AD DE`.

**Bit-band (lysbilde 8-12).** To områder (SRAM fra `0x2000_0000` og periferi fra `0x4000_0000`, 1 MB hver) har
en "alias-region" der hvert *bit* har sitt eget ord. Skriver du 1 eller 0 til minst signifikante bit i aliaset,
endres akkurat det ene bitet i det vanlige området, med én store-instruksjon. Formelen for SRAM:

```
alias = 0x22000000 + (adresse - 0x20000000) * 32 + bitnummer * 4
```

**Load-store (Task 5).** Aritmetikk skjer bare på registre. Å endre en verdi i minnet krever: les (`ldr`),
regn, skriv (`str`). Du ser det selv med `make disasm`.

## Kjør startkoden

```
cd lessons/01_memory
make run
```
Den skriver bare overskriften. Resten er dine TODO-er.

## Oppgaver

1. **TODO 1:** skriv `0xDEADBEEF` til `RAM_TEST`, les tilbake og skriv ut.
2. **TODO 2:** les de fire bytene enkeltvis (`uint8_t`-peker). Hvilken kommer først?
3. **TODO 3:** skriv ut de to første ordene i flash (adresse 0 og 4). Hva er de? (Svar kommer i leksjon 3.)
4. **TODO 4:** sett bit 4 i ordet på `RAM_TEST` med *én* skriving til bit-band-aliaset. Skriv ut ordet.
5. **Disassembly:** kjør `make disasm`, finn linjen med `REG32(RAM_TEST) = 0xDEADBEEF` (etter at du har skrevet den).
   Finn `ldr` og `str`. Hvilke instruksjoner henter adressen og verdien inn i registre før `str`?

## Kontrollspørsmål

1. Hvorfor trenger `REG32` `volatile`?
2. Hva er `0xDEADBEEF` som fire byte i minnet, fra laveste adresse?
3. Hva er alias-adressen for bit 4 i ordet på `0x20008000`?
4. Hva betyr "unified memory space" når bussene er Harvard?
5. Hva er load-store-arkitektur, og hvordan ser du det i disassemblyen?

## Fasit

1. Skriving til maskinvareregistre har bivirkninger og kan ikke fjernes eller omorganiseres av kompilatoren.
2. `EF BE AD DE`.
3. `0x22000000 + 0x8000*32 + 4*4 = 0x22100010`.
4. Instruksjoner og data har hver sin buss (kan hentes samtidig), men begge bruker samme adresser. Det finnes ikke
   to separate adresserom (Lecture 3, lysbilde 8).
5. Bare `ldr`/`str` (og lignende) snakker med minnet. Regneinstruksjoner jobber på registre. Du ser at verdien først
   flyttes til et register før den lagres.

## Hvis det ikke virker

- `flash word 0` viser ikke `0x20010000`: vektortabellen er ikke først i flash (se leksjon 3).
- Bit-band-skrivingen endrer ingenting: emulatoren din støtter kanskje ikke bit-band. Hopp over TODO 4 og
  les teorien i stedet.
