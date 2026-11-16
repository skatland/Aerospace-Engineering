# Leksjon 2: registre og bits

**Pensum:** Lecture 4 (Programmer's Model, lysbilde 2-6).
**Mål:** kunne lese og endre enkeltbiter, og lese prosessorens spesialregistre. Det trenger du i alle følgende leksjoner.

## Teori

**Programmer's model** er den delen av prosessoren du som programmerer må kjenne (lysbilde 2): registre,
moduser og tilstander, instruksjoner, adresseringsmodi og datatyper.

**Registrene (lysbilde 4):** seksten 32-bit registre, R0-R15.

| Register | Bruk |
|---|---|
| R0-R7 | lave registre (alle instruksjoner kan bruke dem) |
| R8-R12 | høye registre (bare 32-bit-instruksjoner) |
| R13 | stakkpeker (SP). To kopier: **MSP** (main) og **PSP** (process) |
| R14 | link-register (LR): returadresse ved funksjonskall |
| R15 | programtelleren (PC) |

**Spesialregistre (lysbilde 5-6).** De ligger ikke i R0-R15, og leses og skrives med to egne instruksjoner:
`MRS` (spesialregister inn i vanlig register) og `MSR` (motsatt vei). Eksempler:

| Register | Innhold |
|---|---|
| xPSR (APSR, IPSR, EPSR) | flagg (N, Z, C, V, Q), exception-nummer, Thumb-bit |
| PRIMASK, FAULTMASK, BASEPRI | maskering av exceptions |
| CONTROL | privilegert/uprivilegert, valg av stakkpeker |

**Inline assembly** lar deg skrive én instruksjon midt i C-koden:

```c
__asm volatile ("mrs %0, control" : "=r" (r));
//              instruksjon        utdata    (ev. : inndata : "clobbers")
```
`%0` er første operand, `"=r"` betyr "plasser resultatet i et vilkårlig register og legg det i `r`".

**Bitoperasjoner** (bit nr. `n` har verdien `1u << n`):

| Operasjon | C |
|---|---|
| sett bit til 1 | `value \| (1u << n)` |
| nullstill bit | `value & ~(1u << n)` |
| snu bit | `value ^ (1u << n)` |
| les bit | `(value >> n) & 1u` |

**Flaggene (APSR):** bit 31 = N (negativt), 30 = Z (null), 29 = C (carry/ikke-borrow), 28 = V (overflow).
`cmp a, b` regner `a - b`, kaster svaret og beholder bare flaggene.

## Kjør startkoden

```
cd lessons/02_registers
make run
```
Du ser mange `FAIL`. Målet er at alle blir `PASS`.

## Oppgaver

1. **TODO 1-4:** fyll ut `set_bit`, `clear_bit`, `toggle_bit` og `test_bit`.
2. **TODO 5:** fyll ut `read_ipsr`, `read_msp` og `read_primask` etter mønsteret i `read_control`.
3. **TODO 6:** fyll ut `apsr_after_cmp`.
4. Sjekk at alle linjer viser `PASS`. Hvis du står fast, se `solution/main.c`.

## Kontrollspørsmål

1. Hvorfor kan du ikke skrive `x = CONTROL;` i C?
2. Hva er IPSR når du er i `main`? Og inne i en interrupt-rutine?
3. Hva er forskjellen på MSP og PSP?
4. Hvilket flagg settes når to like tall sammenlignes?
5. Hvordan snur du bit 7 i en variabel `v`?

## Fasit

1. Spesialregistre er ikke minneadresser. De nås bare med `MRS`/`MSR`, derfor trengs inline assembly eller en funksjon.
2. 0 i `main` (thread mode). Inne i en handler er det exception-nummeret (for eksempel 11 for SVCall, 15 for SysTick).
3. To kopier av R13. Operativsystemer bruker MSP i kjernen og PSP i oppgavene (Lecture 3 kaller det "shadowed stack pointer").
4. Z (zero), og C (ingen lån).
5. `v ^= (1u << 7);`
