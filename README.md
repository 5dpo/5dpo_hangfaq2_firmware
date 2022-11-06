# 5dpo_q2_firmware

**Version 0.0.0**

Nothing.

**With this version, it is possible to do:**

- Serial communication using the library channels
- Motors angular speed control
- Motors PWM control

## Hardware

- Robot Platform Hangfa Discovery Q2
- Faulhaber DC Motor 2342L012CR_M124-Y2002_257
  ([datasheet](https://www.faulhaber.com/fileadmin/Import/Media/EN_2342_CR_DFF.pdf))
- Arduino Mega 2560 Rev3
- Adafruit Motor Shield V2
- Li-Ion 18650 BMS Charger PCB, 3S 12.6V 20A

## Serial Communication (channels)

**Robot > PC**

- `[g]`: encoders total count of motor 0 (ticks)
- `[h]`: encoders total count of motor 1 (ticks)
- `[i]`: encoders total count of motor 2 (ticks)
- `[j]`: encoders total count of motor 3 (ticks)
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
