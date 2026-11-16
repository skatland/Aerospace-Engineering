# Leksjon 5: exceptions og interrupts

**Pensum:** Lecture 6 (alle lysbilder: Exception Types, NVIC, Priority, Using Interrupts, Accepting Exceptions, Exception Sequence, Interrupt Latency).
**Innlevering:** Task 6 (betingelser for å akseptere en exception) og Task 7 (typisk exception-sekvens). Også Task 5 (ICI).
**Mål:** se en exception skje, se hva maskinvaren legger på stakken, og styre en interrupt selv.

## Teori

**Exception** = hendelse som endrer programflyten. Kilder: systemexceptions (Reset, NMI, HardFault, SVCall, PendSV, SysTick ...),
maskerbare interrupts og NMI. Hver har et nummer og en prioritet. **NVIC** (Nested Vectored Interrupt Controller) styrer
konfigurasjon, prioritering og maskering. Startadressen til handleren hentes fra **vektortabellen** (leksjon 3).

**Slik bruker du et interrupt (lysbilde 7):**
1. Skriv adressen til handleren i vektortabellen (hos oss: skriv en funksjon med riktig navn som overstyrer `weak`-versjonen i `startup.c`).
2. Sett prioritetsnivå (valgfritt).
3. Slå på interrupt-generering i perifer enheten som utløser det (SysTick: bit `TICKINT`).
4. Slå på interruptet i NVIC. (SysTick er en systemexception og styres bare av eget kontrollregister, så dette steget gjelder vanlige IRQ-er.)

**Betingelser for at prosessoren skal akseptere en exception (lysbilde 8, Task 6):**
prosessoren kjører, exception er enablet, har høyere prioritet enn nåværende nivå, og er ikke blokkert av et masking-register
(PRIMASK, FAULTMASK, BASEPRI).

**Exception-sekvensen (lysbilde 9, Task 7):**

| Fase | Hva skjer |
|---|---|
| Entrance | registre legges på stakken, vektoren hentes, handlerens instruksjoner hentes, NVIC oppdateres |
| Handler | kjører i handler mode, privilegert, med MSP |
| Return | registrene hentes tilbake, NVIC oppdateres, det avbrutte programmet fortsetter |

Registrene som legges på stakken er (fra lavest adresse): **R0, R1, R2, R3, R12, LR, PC, xPSR**. Det står ikke i teksten på
lysbildene, men figuren "Exception Entrance" (lysbilde 10) viser det, og du ser det selv i Del A.

**Trampolinen i `SVC_Handler`.** Når handleren starter inneholder LR en spesialverdi (EXC_RETURN) der bit 2 forteller hvilken stakk
som var i bruk. Hovedkoden gjør:

```
tst lr, #4        // test bit 2 av LR
ite eq            // "if-then-else": gjør neste to instruksjoner betinget
mrseq r0, msp     // bit 2 = 0: MSP var i bruk, legg MSP i R0
mrsne r0, psp     // ellers: PSP
b svc_c_handler   // hopp til C-funksjonen, R0 = første argument = peker til stakkrammen
```
`naked` betyr at kompilatoren ikke legger til egen inngangs- og utgangskode, så registrene ikke blir ødelagt før vi leser dem.

**SysTick-registre:** `CSR` (bit 0 `ENABLE`, bit 1 `TICKINT`, bit 2 `CLKSOURCE`, bit 16 `COUNTFLAG`), `RVR` (reload), `CVR` (nåverdi).
Telleren teller ned fra reload-verdien til 0, setter flagg og/eller exception, og starter på nytt.

**ICI (Task 5):** `LDM`, `STM`, `PUSH` og `POP` kan avbrytes midt i. Nummeret på neste register lagres i ICI-bitene i den stackede
xPSR, og instruksjonen fortsetter etter handleren. Du kan ikke enkelt vise det i koden, men se xPSR-verdien i Del A.

## Kjør startkoden

```
cd lessons/05_exceptions
make run
```
Del A skriver en overskrift men ingen verdier. Del B **henger med vilje** til du har gjort TODO 2 og 3. Avslutt med Ctrl+A, X.

## Oppgaver

1. **TODO 1:** skriv ut IPSR og de 8 stackede ordene i `svc_c_handler`. Kjør. R0-R3 skal bli `0x11, 0x22, 0x33, 0x44`.
   IPSR skal være 11. xPSR skal ha bit 24 satt (Thumb).
2. **TODO 2:** sett opp SysTick (`RVR = 1000000 - 1`, `CVR = 0`, `CSR` = bit 2, 1 og 0).
3. **TODO 3:** tell opp `tick_count` i `SysTick_Handler`. Nå skal programmet komme forbi løkka og skrive "got 5 ticks".
4. **TODO 4:** mask interrupts med `cpsid i`, vent med en løkke, og se hva som skjer med `tick_count` mens de er maskert og
   etter at de er enablet igjen. Forventet: uendret mens maskert, og én mer etterpå. Hvis den ikke øker i det hele tatt,
   gjør ventesløyfen lengre. Hvorfor bare én mer, selv om mange ticks skulle kommet?
5. Valgfritt: sett `PRIMASK` i starten av en handler og se at den ikke blir avbrutt (slide 8 sier at dette hindrer nested exceptions).

## Kontrollspørsmål

1. Hvilke registre legger maskinvaren på stakken ved exception-inngang?
2. Hva er IPSR inne i SVC-handleren og i SysTick-handleren?
3. Nevn de fire betingelsene for at en pending exception aksepteres.
4. Hva er forskjellen på exception-fase "entrance" og "return"?
5. Hvorfor skal en ISR være kort?

## Fasit

1. R0, R1, R2, R3, R12, LR, PC, xPSR (8 ord = 32 byte).
2. 11 for SVCall og 15 for SysTick.
3. Prosessoren kjører, exception er enablet, høyere prioritet enn nåværende nivå, ikke blokkert av masking-register.
4. Entrance: stacking, vektorhenting, handler-instruksjoner hentes. Return: unstacking og at det avbrutte programmet hentes igjen.
5. Andre exceptions med lik eller lavere prioritet må vente mens den kjører, så den øker latency for dem (Lecture 7, lysbilde 20).
