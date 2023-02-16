# 5dpo_q2_firmware

This repository implements the firmware relative to the Hangfa Discovery Q2
four-wheeled omnidirectional robotic platform.

**Version 2.1.1**

**With this version, it is possible to do:**

- Serial communication using the library channels
- Read encoders channel A and B for wheeled odometry
- Motors angular speed control with a generic PID controller
- Motors PWM control
  ([TimerOne](https://github.com/PaulStoffregen/TimerOne) and
  [TimerThree](https://github.com/PaulStoffregen/TimerThree) PWM)
- Watchdog timer to disable the motors if the PC does not send anything for a
  certain time

**The next version will add these features:**

- Tune the PID controllers for angular speed control of the motors

**Bugs identified in the current version:**

- None

## Hardware

- Robot Platform Hangfa Discovery Q2
  ([info](http://www.hangfa.com/EN/robot/DiscoveryQ2.html))
- Faulhaber DC Motor 2342L012CR_M124-Y2002_257
  ([datasheet](https://www.faulhaber.com/fileadmin/Import/Media/EN_2342_CR_DFF.pdf))
  - 3K3 Ohms Resistors (pull-up resistor for the encoders channels)
- Arduino Mega 2560 Rev3
  ([store](https://store.arduino.cc/products/arduino-mega-2560-rev3))
- Arduino Mega Proto Shield Rev3
  ([store](https://store.arduino.cc/products/arduino-mega-proto-shield-rev3-pcb))
  - IDC Socket 6Way
    ([store](https://mauser.pt/catalog/product_info.php?products_id=011-0827))
  - IDC Header Straight 6Way
    ([store](https://mauser.pt/catalog/product_info.php?products_id=011-0800))
  - 8-pin stackable header (Arduino Compatible)
    ([store](https://opencircuit.nl/product/female-header-stackable-8-pin-10-stuks))
  - 10-pin stackable header (Arduino Compatible)
    ([store](https://opencircuit.nl/product/female-header-stackable-10-pin-10-stuks))
  - 2x18-pin stackable header (Arduino Compatible)
    ([store](https://opencircuit.nl/product/female-header-stackable-2x18-pin-2-stuks))
- Cytron 13A, 5-30V Single DC Motor Controller
  ([info](https://www.cytron.io/c-motor-and-motor-driver/c-motor-driver/p-10amp-5v-30v-dc-motor-driver),
  [store](https://www.robotshop.com/eu/en/cytron-13a-5-30v-single-dc-motor-controller.html))
  - KK shell connector 3Way 2.54mm (Molex 5051 Housing)
    ([store](https://mauser.pt/catalog/product_info.php?products_id=011-1319))
  - Terminals KK shell connector 3Way 2.54mm
    ([store](https://mauser.pt/catalog/product_info.php?products_id=011-3065))
- Li-Ion 18650 BMS Charger PCB, 3S 12.6V 20A
  ([store](https://www.botnroll.com/pt/acessorios/2558-bms-para-protec-o-baterias-18650-3s-12-6v-20a.html))
- Tattu 11.1V 15C 3S 10000mAh Lipo Battery Pack
  ([store](https://www.gensace.de/tattu-11-1v-15c-3s-10000mah-lipo-battery-pack.html))

## Serial Communication (channels)

**Robot > PC**

- `[g]`: encoders total count of motor 0 (ticks)
- `[h]`: encoders total count of motor 1 (ticks)
- `[i]`: encoders total count of motor 2 (ticks)
- `[j]`: encoders total count of motor 3 (ticks)
- `[k]`: interval time between control cycles (us)
- `[r]`: reset signal

**PC > Robot**

- `[G]`: reference angular speed of motor 0 (rad/s)
- `[H]`: reference angular speed of motor 1 (rad/s)
- `[I]`: reference angular speed of motor 2 (rad/s)
- `[J]`: reference angular speed of motor 3 (rad/s)
- `[K]`: PWM value of motors
  - `(value >> 24) & 0x03`: motor index (0..3)
  - `value & 0xFFFF`: PWM value (0..`kMotPWMmax`)

## Usage

**Requirements**

- PlatformIO extension for VS Code

### Compilation

1. Clone the repository
   ```sh
   # Clone repository
   git clone git@github.com:5dpo/5dpo_q2_firmware.git
   cd 5dpo_q2_firmware

   # Open the folder as a project
   code .
   ```
2. Open `src/main.cpp`
3. Compile the project using the PlatformIO extension in VS Code
   (PlatformIO on left-side navigation bar > Project Tasks > Build)

### Run

1. Open `src/main.cpp`
2. Compile the project using the PlatformIO extension in VS Code
   (PlatformIO on left-side navigation bar > Project Tasks > Upload)
