# Leksjon 7: programvarestruktur (lag, polling mot interrupt)

**Pensum:** Lecture 7, lysbilde 13-23 (Software Structure, HAL, Device Drivers, Polling, Interrupt-metodene, Event-Action).
**Innlevering:** Task 8 (software layering) og Task 9 (polling mot interrupt-basert flyt).
**Mål:** bygge et lite lagdelt program og *måle* forskjellen på polling og interrupt.

## Teori

**Lagdeling (lysbilde 14-16).** Embedded-programvare bygges vanligvis i lag, der de nederste lagene forenkler utviklingen av de øverste:

```
applikasjon          (main.c)      kjenner bare hal_*-funksjoner
device drivers       (eksterne enheter, f.eks. en sensor)
HAL                  (hal.c)       skjuler registerne i brikken
maskinvare           (SysTick, UART ...)
```

Produsentene leverer ofte HAL (TivaWare, STM32Cube HAL, MCUXpresso SDK ...) og driverne kommer fra enhetsprodusentene. Poenget er at
applikasjonen ikke skal være avhengig av akkurat denne brikken, slik at den er enklere å flytte.

**Polling / superloop (lysbilde 17-18, Task 9a).** Hovedløkka spør hele tiden om noe trenger behandling. Enkelt å lage, men ikke egnet for store
programmer, ikke energieffektivt, og rekkefølgen definerer prioritet.

**Interrupt-metoden (lysbilde 19-21, Task 9b).** "Hollywood-prinsippet": *ikke ring oss, vi ringer deg*. Perifer enhet melder fra, og prosessoren
kan sove imellom. Den *forbedrede* varianten holder ISR-ene korte og enkle: de setter bare et flagg, og hovedløkka gjør jobben.

Worst-case latency for både polling og den forbedrede interrupt-metoden er (lysbilde 17 og 20):
`max(B + C, A + B, A + C)`. Kombinert med en prioritetskø kan snittlatencyen bli bedre.

**Hva vi måler:** hvor mange ganger hovedløkka kjører mens vi venter på 5 ticks. Polling kjører løkka igjen og igjen. Interrupt-varianten sover
(`wfi`) og våkner bare når noe skjer.

## Kjør startkoden

```
cd lessons/07_structure
make run
```
Begge tellerne er 0 og du ser `FAIL`.

## Oppgaver

1. **TODO 1:** `run_polling`: løkke til `TICKS_WANTED` ticks er sett. Tell `iterations` og bruk `hal_tick_elapsed()`.
2. **TODO 2:** `run_interrupt`: løkke der du teller `iterations`, kaller `hal_sleep()` og sjekker `tick_flag`.
3. **TODO 3:** `SysTick_Handler` skal bare sette `tick_flag = 1`.
4. Kjør og sammenlign tallene. Hvor mange ganger oftere kjører polling-løkka enn interrupt-løkka?
5. Diskusjon (godt til Task 9): hva er fordeler og ulemper med hver? Hva skjer med *energibruken* når du sover?
6. Utvidelse (lagdeling): lag `timer.h`/`timer.c` med `timer_start()` og `timer_wait_tick()` som bruker `hal_*` internt, slik at `main.c` ikke
   kjenner til flagget eller `wfi`. Hvilket lag eier flagget nå?
7. Utvidelse (delt variabel): `tick_flag` deles mellom ISR og hovedløkke. Hva kan gå galt mellom `if (tick_flag)` og `tick_flag = 0`?
   (Tips: et tick som kommer midt imellom.)

## Kontrollspørsmål

1. Hva er software layering, og hvorfor brukes det?
2. Hva gjør en HAL?
3. Nevn to ulemper med polling.
4. Hvorfor skal en ISR være kort?
5. Hva betyr "Hollywood-prinsippet"?

## Fasit

1. Programvaren deles i lag der hvert lag bruker laget under. Det gir enklere utvikling, bedre portabilitet og gjenbruk (Lecture 7, lysbilde 14-16).
2. Skjuler maskinvaredetaljene i brikken bak et fast sett funksjoner, slik at applikasjonen kan skrives uten å kjenne registrene.
3. Ikke energieffektiv; rekkefølgen definerer prioritet; passer dårlig for store programmer.
4. Mens en ISR kjører, kan andre exceptions med lik eller lavere prioritet måtte vente, så lang ISR øker latency.
5. "Don't call us, we'll call you": enheten melder fra når den trenger behandling, i stedet for at prosessoren spør hele tiden.
