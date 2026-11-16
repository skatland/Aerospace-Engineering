# Leksjon 4: stakken (full descending)

**Pensum:** Lecture 5, lysbilde 13-17 (Stack Memory, MSP og PSP); Lecture 4, lysbilde 4 (R13 er banked).
**Innlevering:** Task 4.
**Mål:** *se* hvordan SP endrer seg ved push og pop, og hva funksjonskall gjør med stakken.

## Teori

- **Descending:** stakken vokser mot lavere adresser. **Full:** SP peker på det siste *lagrede* ordet.
- **PUSH:** først SP = SP - 4 per register, så lagres verdien. **POP:** først leses verdien, så SP = SP + 4 per register.
- Ved `PUSH {R4-R6}` ligger registeret med lavest nummer på lavest adresse, uansett rekkefølge du skriver dem i.
- Data som er popet blir stående i minnet, men regnes som ledig og overskrives av neste push.
- Hvert funksjonskall lager en **stakkramme**: returadresse (LR), lokale variabler og lagrede registre. Dypere kall gir
  lavere SP. Rekursjon uten stopp fører til at stakken vokser ned i `.bss` og `.data` (stack overflow).
- **MSP og PSP (lysbilde 17):** to stakkpekere, begge som R13. Operativsystemer bruker MSP for kjernen og PSP for oppgavene.
  I dette kurset bruker vi bare MSP.

## Kjør startkoden

```
cd lessons/04_stack
make run
```
Du ser start-SP, og SP på fire rekursjonsnivåer.

## Oppgaver

1. **TODO 1:** mål SP før push, etter push og etter pop med én `asm`-blokk (skisse i kommentaren). Hvor mange byte flytter
   `PUSH {R4-R6}` SP? Stemmer det med 3 ord · 4 byte?
2. **TODO 2:** push tre registre med kjente verdier, og les deretter ordene i minnet ved SP. Hvilket register ligger hvor?
   Hvorfor står verdiene der etter pop?
3. **TODO 3:** vis hvor mange byte ett funksjonskall bruker i `recurse` (differansen mellom nivåene).
4. Tegn (for hånd) et diagram av stakken etter `PUSH {R4-R6}`, slik som i Task 4, med adressene fra utskriften din.

## Kontrollspørsmål

1. Hva betyr "full descending"?
2. Hva er SP etter `PUSH {R0-R3}` hvis den var `0x20010000`?
3. Hvorfor kan vi lese tilbake verdiene etter en pop?
4. Hvorfor er størrelsen på en stakkramme ofte et multiplum av 8? (Tips: ABI, Lecture 7, lysbilde 8)
5. Hva er forskjellen på MSP og PSP?

## Fasit

1. Stakken vokser mot lavere adresser, og SP peker på øverste *brukte* ord.
2. `0x20010000 - 16 = 0x2000FFF0`.
3. Pop flytter bare SP. Den sletter ikke dataen.
4. Prosedyrekallstandarden (AAPCS, en del av ARM sin ABI) krever at SP er 8-byte-justert ved funksjonskall.
5. To registre for R13. Hvilken som er aktiv styres av CONTROL-registeret. MSP brukes alltid i handler mode.
