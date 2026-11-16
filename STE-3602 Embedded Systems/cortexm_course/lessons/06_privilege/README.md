# Leksjon 6: privilegert og uprivilegert tilgang

**Pensum:** Lecture 4, lysbilde 3 (Operation Modes and States, Access Levels); Lecture 5, lysbilde 18 (blokkerte regioner i uprivilegert modus).
**Innlevering:** Task 3.
**Mål:** bytte mellom privilegert og uprivilegert tilgang, og forstå hvorfor veien tilbake går via en exception.

## Teori

| | Privilegert | Uprivilegert |
|---|---|---|
| **Thread mode** (vanlig kode) | ja | ja |
| **Handler mode** (exception-handlere) | alltid | finnes ikke |

Etter reset starter prosessoren i thread mode, privilegert. **CONTROL-registeret** styrer:

- Bit 0, **nPRIV**: 0 = privilegert, 1 = uprivilegert i thread mode.
- Bit 1, **SPSEL**: hvilken stakkpeker thread mode bruker (0 = MSP, 1 = PSP).

Uprivilegert kode har begrensninger: den kan ikke endre CONTROL (skriving ignoreres), og tilgang til enkelte regioner er blokkert,
blant annet PPB og SCS (lysbilde 18). Prøver den, får du en fault.

**Hvordan går man tilbake?**

- Privilegert → uprivilegert: skriv `nPRIV = 1`. Det kan privilegert kode.
- Uprivilegert → privilegert: kan ikke gjøres direkte. Du må utløse en exception (for eksempel `SVC`). Handleren er alltid privilegert og
  kan nullstille `nPRIV`. Når handleren returnerer, er tråden privilegert.

Det er dette Task 3 spør om. Etter å ha skrevet til CONTROL bør du bruke `ISB` (instruction synchronization barrier) slik at endringen får virke
før neste instruksjon.

## Kjør startkoden

```
cd lessons/06_privilege
make run
```
Den skriver bare CONTROL ved start.

## Oppgaver

1. **TODO 1:** `write_control(1)`. Skriv ut CONTROL.
2. **TODO 2:** `write_control(0)`. Virket det? Skriv ut CONTROL.
3. **TODO 3:** fyll ut `SVC_Handler` slik at den nullstiller bit 0 i CONTROL og returnerer.
4. **TODO 4:** `svc #0`, og skriv ut CONTROL etterpå.
5. **Fault-demo:** sett `FAULT_DEMO` til 1 i `solution/main.c` (kopier til din `main.c`), bygg og kjør. Uprivilegert kode skriver til SysTick-registeret
   (i SCS). Se HardFault-meldingen.

## Kontrollspørsmål

1. Hvorfor kan ikke uprivilegert kode sette seg selv tilbake til privilegert?
2. Hvilken exception brukes vanligvis for å be om privilegert tjeneste?
3. Hva skjer med en skriving til CONTROL fra uprivilegert modus?
4. Er handler mode noen gang uprivilegert?
5. Hva er forskjellen på operation mode og access level?

## Fasit

1. Da ville beskyttelsen vært verdiløs. Hvem som helst kunne gitt seg selv full tilgang. Derfor kreves en exception, og operativsystemet i handleren kan sjekke hvem som spør.
2. SVC (supervisor call).
3. Den ignoreres, uten feilmelding.
4. Nei, handler mode er alltid privilegert.
5. Mode (thread/handler) sier *hva* prosessoren gjør (vanlig kode eller exception-handler). Access level (privilegert/uprivilegert) sier *hva den får lov til*.
