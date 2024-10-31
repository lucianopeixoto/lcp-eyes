#include <Servo.h>
#include <EEPROM.h>

Servo eyeServo;
const int servoPin = 9;
int minPosition = 45;   // Default min position for the first configuration
int maxPosition = 135;  // Default max position for the first configuration
int currentMode = 1;    // Default mode is static at the center
bool mode1Centered = false; // Track if center position was set in mode 1

void setup() {
  Serial.begin(9600);
  eyeServo.attach(servoPin);
  loadSettings();
  
  // Move servo to center position at startup
  int centerPosition = (minPosition + maxPosition) / 2;
  eyeServo.write(centerPosition);
  delay(500); // Small delay to ensure servo reaches center position
  
  Serial.println("=== LCP Animatronic Eyes v0.1 ===");
  Serial.println("Proof of concept with left-right eye movement. More DoF and features coming soon!");
  displayMenu();
}

void loop() {
  if (Serial.available()) {
    handleSerialCommands();
  }

  switch (currentMode) {
    case 1:
      if (!mode1Centered) {
        moveToCenter();
        mode1Centered = true;
      }
      break;
    case 2:
      lookLeftAndRight();
      break;
    case 3:
      randomLooking();
      break;
    case 4:
      requestSpecificPosition();
      break;
    case 5:
      randomLookingWithVariedSpeed();
      break;
  }
}

void handleSerialCommands() {
  String command = Serial.readStringUntil('\n');

  if (command == "set_min") {
    Serial.println("Enter minimum position (0-180): ");
    while (!Serial.available());
    minPosition = Serial.parseInt();
    if (minPosition < 0 || minPosition > 180) {
      Serial.println("Invalid input. Please enter a value between 0 and 180.");
      minPosition = 45;
    } else {
      EEPROM.write(0, minPosition);
      Serial.println("Minimum position saved.");
      eyeServo.write(minPosition); // Move servo to minimum position for feedback
      Serial.print("Servo moved to minimum position: ");
      Serial.println(minPosition);
    }
  } 
  else if (command == "set_max") {
    Serial.println("Enter maximum position (0-180): ");
    while (!Serial.available());
    maxPosition = Serial.parseInt();
    if (maxPosition < 0 || maxPosition > 180) {
      Serial.println("Invalid input. Please enter a value between 0 and 180.");
      maxPosition = 135;
    } else {
      EEPROM.write(1, maxPosition);
      Serial.println("Maximum position saved.");
      eyeServo.write(maxPosition); // Move servo to maximum position for feedback
      Serial.print("Servo moved to maximum position: ");
      Serial.println(maxPosition);
    }
  }
  else if (command == "mode 1") {
    setMode(1);
    Serial.println("Mode set to Static at Center.");
  }
  else if (command == "mode 2") {
    setMode(2);
    Serial.println("Mode set to Looking Left and Right Slowly.");
  }
  else if (command == "mode 3") {
    setMode(3);
    Serial.println("Mode set to Random Looking.");
  }
  else if (command == "mode 4") {
    setMode(4);
    Serial.println("Mode set to Request Specific Position.");
  }
  else if (command == "mode 5") {
    setMode(5);
    Serial.println("Mode set to Random Looking with Varied Speed.");
  }
  else if (command == "display_menu") {
    displayMenu();
  }
  else if (command == "show_settings") {
    showSettings();
  }
  else if (command == "exit") {
    setMode(1);
    Serial.println("Exited current mode. Returned to Mode 1 (Static at Center).");
  } else {
    Serial.println("Invalid command. Type 'display_menu' to see all commands.");
  }
}

void displayMenu() {
  Serial.println("=== LCP Animatronic Eyes v0.1 Control Menu ===");
  Serial.println("Commands:");
  Serial.println("  set_min - Set minimum servo position");
  Serial.println("  set_max - Set maximum servo position");
  Serial.println("  mode 1  - Static at center");
  Serial.println("  mode 2  - Looking left and right slowly");
  Serial.println("  mode 3  - Random looking around");
  Serial.println("  mode 4  - Set specific position");
  Serial.println("  mode 5  - Random looking with varied speed");
  Serial.println("  exit - Exit current mode and return to Mode 1");
  Serial.println("  show_settings - Display current settings");
  Serial.println("  display_menu - Show this menu again");
}

void showSettings() {
  Serial.println("=== Current Settings ===");
  Serial.print("Minimum Position: ");
  Serial.println(minPosition);
  Serial.print("Maximum Position: ");
  Serial.println(maxPosition);
  Serial.print("Current Mode: ");
  Serial.print(currentMode);
  switch (currentMode) {
    case 1: Serial.println(" (Static at Center)"); break;
    case 2: Serial.println(" (Looking Left and Right Slowly)"); break;
    case 3: Serial.println(" (Random Looking)"); break;
    case 4: Serial.println(" (Request Specific Position)"); break;
    case 5: Serial.println(" (Random Looking with Varied Speed)"); break;
  }
}

void moveToCenter() {
  int centerPosition = (minPosition + maxPosition) / 2;
  eyeServo.write(centerPosition);
}

void lookLeftAndRight() {
  for (int pos = minPosition; pos <= maxPosition; pos++) {
    eyeServo.write(pos);
    delay(20);
    if (Serial.available()) return; // Check for new serial commands
  }
  for (int pos = maxPosition; pos >= minPosition; pos--) {
    eyeServo.write(pos);
    delay(20);
    if (Serial.available()) return; // Check for new serial commands
  }
}

void randomLooking() {
  int newPosition = random(minPosition, maxPosition);
  eyeServo.write(newPosition);
  delay(random(500, 1500));
}

void randomLookingWithVariedSpeed() {
  int newPosition = random(minPosition, maxPosition);
  
  if (random(10) < 7) { // 70% chance for slow movement
    int currentPosition = eyeServo.read();
    int step = (newPosition > currentPosition) ? 1 : -1;
    while (currentPosition != newPosition) {
      currentPosition += step;
      eyeServo.write(currentPosition);
      delay(20); // Slow movement
      if (Serial.available()) return; // Check for new commands
    }
  } else {
    // Quick movement
    eyeServo.write(newPosition);
  }
  
  delay(random(500, 1500));
}

void requestSpecificPosition() {
  Serial.println("Enter a specific position within set limits (or type 'exit' to leave):");
  while (true) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      
      if (input == "exit") {
        Serial.println("Exiting Specific Position mode.");
        setMode(1);
        displayMenu();
        break;
      }

      int position = input.toInt();

      if (position < minPosition || position > maxPosition) {
        Serial.print("Invalid position. Please enter a value between ");
        Serial.print(minPosition);
        Serial.print(" and ");
        Serial.print(maxPosition);
        Serial.println(".");
      } else {
        eyeServo.write(position);
        Serial.print("Moved to position: ");
        Serial.println(position);
      }
    }
  }
}

void loadSettings() {
  minPosition = EEPROM.read(0);
  maxPosition = EEPROM.read(1);
  currentMode = EEPROM.read(2);

  // If EEPROM values are out of range, reset to default
  if (minPosition < 0 || minPosition > 180) minPosition = 45;
  if (maxPosition < 0 || maxPosition > 180) maxPosition = 135;
  if (currentMode < 1 || currentMode > 5) currentMode = 1;
}

void setMode(int mode) {
  currentMode = mode;
  EEPROM.write(2, currentMode); // Save mode to EEPROM
  mode1Centered = false; // Reset center flag so it moves to center when back in mode 1
}
