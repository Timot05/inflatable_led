# A portable light printed using Rapid Liquid Printing

## Specs

- 2700K
- About 400 lumens
- Dimmable
- Rechargeable
- Flashes when battery low

## Implementation

- Micro: RP2040
- LED Driver: TPS61165
- LED: OSRAM GW JTLPS1.EM-KKLL-XX58-1
- Batteries: TBD
- Battery charger: TBD

## TODOs for V1 order:

- Check order of LEDs
- Check the caps footprint
- C15, C16 are wrong size
- Remove todo from schematic

## TODOs for v2:

- [ ] Add charging status to micro
- [ ] fix the led circuit switching
- [ ] enable low power mode with reset switch (no long press. How did we switch all circuits on mopi?)
- [x] Remove the cap on qspi_ss
- [x] DNP for the 3.3V led
- [ ] Add a buck boost instead of ldo
- [ ] Check the bat sense feature
- [ ] White circuit for reflection
- [ ] Add gnd, vref, 3.3V to the debug port

Nice to have:

- i2c connected charger
- Protrude the button more
- Nice feeling button