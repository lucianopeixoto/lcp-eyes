# LCP Animatronic Eyes v0.1

### Proof of Concept for Realistic Animatronic Eye Movement

**LCP Animatronic Eyes** is an Arduino-based project designed to control an animatronic eye mechanism with a single degree of freedom (DoF), allowing the eyes to look left and right. This initial version serves as a proof of concept, with future releases planned to expand functionality and add additional DoF for more realistic movements.

## Features

- **Configurable Left and Right Limits**: Set the minimum and maximum angles for the eye movement.
- **Multiple Operating Modes**:
  - **Static at Center**: Keeps eyes centered.
  - **Looking Left and Right Slowly**: Continuous side-to-side movement.
  - **Random Looking**: Simulates a natural, random gaze.
  - **Specific Position**: Allows the user to specify an exact eye position within the configured limits.
- **Serial Console Configuration**: Adjust settings via serial commands.
- **Non-Volatile Memory Storage**: Saves configuration settings (min/max positions and selected mode) in EEPROM, so settings persist after reboot.
- **Command to Display Current Settings**: Check current configuration via a serial command.

## Hardware Requirements

- Arduino board
- 1 Servo motor (connected to pin 9)
- Serial communication setup (USB connection for Arduino IDE Serial Monitor)

## Getting Started

### Prerequisites

- Arduino IDE installed
- Basic knowledge of working with servos and the Arduino platform

### Wiring

1. Connect the servo signal wire to **pin 9** of the Arduino.
2. Connect the servo's power and ground to the Arduino’s **5V** and **GND** pins.

### Installation

1. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/yourusername/LCP-Animatronic-Eyes.git
2. Open LCP_Animatronic_Eyes.ino in the Arduino IDE.
3. Compile and upload the sketch to your Arduino board.

### Usage

After uploading, open the Serial Monitor in the Arduino IDE (9600 baud rate) to interact with the animatronic eye system.

#### Commands

| Command            | Description                                                                                     |
|--------------------|-------------------------------------------------------------------------------------------------|
| `set_min`          | Set the minimum angle for eye movement (0-180).                                                 |
| `set_max`          | Set the maximum angle for eye movement (0-180).                                                 |
| `mode 1`           | Set to static at center position.                                                               |
| `mode 2`           | Look left and right slowly.                                                                     |
| `mode 3`           | Random looking.                                                                                 |
| `mode 4`           | Request a specific position for the eye.                                                        |
| `exit`             | Exit the current mode and return to Mode 1 (centered position).                                 |
| `show_settings`    | Display the current settings, including min and max positions and selected mode.                |
| `display_menu`     | Display the command menu for quick reference.                                                   |

### Examples

1. **Set Min and Max Positions**
   - Type `set_min` and enter a value (e.g., 45).
   - Type `set_max` and enter a value (e.g., 135).

2. **Change Modes**
   - Type `mode 2` to set the eyes to look left and right slowly.

3. **Display Settings**
   - Type `show_settings` to see the current configuration.

## Future Improvements

- Adding more degrees of freedom for enhanced realism.
- Introducing speed adjustments for servo movements.
- Creating more complex gaze behaviors, like quick glances and blinks.
- Adding a graphical user interface (GUI) for easier configuration.

## License

This project is licensed under the GNU General Public License v3. See the [LICENSE](LICENSE) file for details.
