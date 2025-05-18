#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(2, 3);

// Button pins
const int button_acceleration = 4;
const int button_ATK = 3;
const int button_jump = 2;
const int button_specialSKILL = 5;
int r_x = A3;
int r_y = A4;
int led1 = 8;

int led2 = 9;
int led3 = 10;

int led4 = 11;

const int threshold = 50;

// Variables to store previous states
bool prev_acceleration = HIGH;
bool prev_ATK = HIGH;
bool prev_jump = HIGH;
bool prev_specialSKILL = HIGH;
bool prev_joystick_left = HIGH;
bool prev_joystick_right = HIGH;
bool prev_joystick_up = HIGH;
bool prev_joystick_down = HIGH;

// Text for scrolling
const char *scrollText = "Omnistrike : Rise of the resistance ";

// Timing for scrolling
unsigned long previousMillis = 0;
const long interval = 900; // interval at which to scroll text (milliseconds)

void setup() {
    // Initialize the LCD
    lcd.init(); 
    lcd.backlight();

    // Set button pins as input
    pinMode(button_acceleration, INPUT_PULLUP);
    pinMode(button_ATK, INPUT_PULLUP);
    pinMode(button_jump, INPUT_PULLUP);
    pinMode(button_specialSKILL, INPUT_PULLUP);
    pinMode(r_x, INPUT);
    pinMode(r_y, INPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);

    Serial.begin(9600);
}

void loop() {
    // Handle button states and control LEDs
    manageButtonsAndLeds();
    int joy_x = analogRead(A0);
    int joy_y = analogRead(A1);

    // Check if it's time to update the scroll
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        scrollTextAnimation();
    }
}

void manageButtonsAndLeds() {
    int joy_x = analogRead(A0);
    int joy_y = analogRead(A1);

    // Read button states
    bool acceleration_state = digitalRead(button_acceleration);
    bool ATK_state = digitalRead(button_ATK);
    bool jump_state = digitalRead(button_jump);
    bool specialSKILL_state = digitalRead(button_specialSKILL);

    // LEFT button
    if (acceleration_state != prev_acceleration) {
        if (acceleration_state == LOW) {
            Serial.println("Acceleration");
            lcd.setCursor(4, 1);
            lcd.print("Acceleration    ");
            digitalWrite(led1,LOW); 
        } else {
            clearDisplay();
            digitalWrite(led1,HIGH); 
        }
        prev_acceleration = acceleration_state;
    }
    
    // RIGHT button
    if (ATK_state != prev_ATK) {
        if (ATK_state == LOW) {
            Serial.println("ATK");
            lcd.setCursor(4, 1);
            lcd.print("Attack  ");
            digitalWrite(led4,LOW); 
        } else {
            clearDisplay();
            digitalWrite(led4,HIGH);
        }
        prev_ATK = ATK_state;
    }

    // JUMP button
    if (jump_state != prev_jump) {
        if (jump_state == LOW) {
            Serial.println("JUMP");
            lcd.setCursor(4, 1);
            lcd.print("JUMP    ");
            digitalWrite(led2,LOW); 
        } else {
            clearDisplay();
            digitalWrite(led2,HIGH);
        }
        prev_jump = jump_state;
    }

    // SPECIAL SKILL button
    if (specialSKILL_state != prev_specialSKILL) {
        if (specialSKILL_state == LOW) {
            Serial.println("SKILL");
            lcd.setCursor(4, 1);
            lcd.print("SpecialSKILL    ");
            digitalWrite(led3,LOW);
        } else {
            clearDisplay();
            digitalWrite(led3,HIGH);
        }
        prev_specialSKILL = specialSKILL_state;
    }

    // Joystick movement detection
    if (joy_x < threshold) {
        // Joystick moved to the left
        if (!prev_joystick_left) {
            lcd.setCursor(0, 1);
            lcd.print("Joystick up    ");
            Serial.println("up");
        }
        prev_joystick_left = true;
    } else {
        prev_joystick_left = false;
    }

    if (joy_x > 1023 - threshold) {
        // Joystick moved to the right
        if (!prev_joystick_right) {
            lcd.setCursor(0, 1);
            lcd.print("Joystick down   ");
            Serial.println("down");
        }
        prev_joystick_right = true;
    } else {
        prev_joystick_right = false;
    }

    if (joy_y < threshold) {
        // Joystick moved upward
        if (!prev_joystick_up) {
            lcd.setCursor(0, 1);
            lcd.print("Joystick right     ");
            Serial.println("right");
        }
        prev_joystick_up = true;
    } else {
        prev_joystick_up = false;
    }

    if (joy_y > 1023 - threshold) {
        // Joystick moved downward
        if (!prev_joystick_down) {
            lcd.setCursor(0, 1);
            lcd.print("Joystick left   ");
            Serial.println("left");
        }
        prev_joystick_down = true;
    } else {
        prev_joystick_down = false;
    }
}


void scrollTextAnimation() {
    static int startPos = 0;
    int len = strlen(scrollText);
    for (int pos = 0; pos < 16; pos++) {
        int charIndex = (startPos + pos) % len; // Wrap around the text
        lcd.setCursor(pos, 0);
        lcd.print(scrollText[charIndex]);
    }
    startPos++;
    if (startPos >= len) startPos = 0;
}

void clearDisplay() {
    lcd.setCursor(1, 0);
    lcd.print("                                   ");   // Clear the entire display
}
