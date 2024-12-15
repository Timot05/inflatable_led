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

- [x] Add charging status to micro
- [x] fix the led circuit switching
- [x] enable low power mode with reset switch (no long press. How did we switch all circuits on mopi?)
- [x] Remove the cap on qspi_ss
- [x] DNP for the 3.3V led
- [x] Add a buck boost instead of ldo -> Remain with LDO but remove shotky diode (need to add it in case?)
- [x] Check the bat sense feature - probs ok
- [x] White circuit for reflection - will do
- [ ] Add gnd, vref, 3.3V to the debug port
- [x] Add the 10k resistor to the TS pin on the charger -> changed charger and fixed TS
- [x] Switch to ULDO
- [x] Change the battery holder -> changed to double battery holder

Nice to have:

- i2c connected charger
- Protrude the button more
- Nice feeling button


## TODOs for V3

- [ ] add a label for +/- battery
- [ ] remove the pullup on the on/off bt
- [ ] add a blinky led 