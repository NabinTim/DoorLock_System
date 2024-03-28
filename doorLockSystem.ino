//Min Prasad Timalsina
//Date : 2024 January 21
//Description of final project : Door lock system using arduino

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#define Password_Length 5
Servo myDoorServo;
LiquidCrystal myLcd(A0, A1, A2, A3, A4, A5);
int redLED = 13;   
int greenLED = 12;
int pos = 0;

char enteredPassword[Password_Length]; // Renamed Data to enteredPassword
char masterPassword[Password_Length] = "911#"; 
byte passwordLength = 0;

bool isPasswordCorrect;
bool isDoorOpen = false;
char pressedKey;

const byte ROWS = 4;
const byte COLS = 4;
char keypadKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad myKeypad(makeKeymap(keypadKeys), rowPins, colPins, ROWS, COLS);

void setup()
{
  myDoorServo.attach(9, 2000, 2400); 
  pinMode(redLED, OUTPUT);   // Set red LED pin as output
  pinMode(greenLED, OUTPUT); // Set green LED pin as output
  ServoClose();
  myLcd.begin(16, 2);
  myLcd.clear();
}

void loop()
{
  // Check if the door is open
  if (isDoorOpen == true)
  {
    // Check if the '#' key is pressed to close the door
    pressedKey = myKeypad.getKey();
    if (pressedKey == '#')
    {
      myLcd.clear();
      ServoClose();
      digitalWrite(redLED, LOW);   // Turn off red LED when the door is closed
      digitalWrite(greenLED, LOW); // Turn off green LED when the door is closed
      myLcd.print("Door is closed");
      delay(3000);
      isDoorOpen = false; // Set door status to closed
    }
  }
  else
    Open(); // If the door is closed, go to the Open function
}

void loading(char msg[])
{
  myLcd.setCursor(0, 1);
  myLcd.print(msg);
  for (int i = 0; i < 9; i++)
  {
    delay(1000);
    myLcd.print(".");
  }
}

void clearEnteredPassword()
{
  // Clear stored entered password
  while (passwordLength != 0)
  {
    enteredPassword[passwordLength--] = 0;
  }
  return;
}
void ServoClose()
{
  // Close the servo-controlled door
  for (pos = 90; pos >= 0; pos -= 10)
  {
    myDoorServo.write(pos); // Renamed myservo to myDoorServo
  }
}

void ServoOpen()
{
  // Open the servo-controlled door
  for (pos = 0; pos <= 180; pos += 10)
  {
    myDoorServo.write(pos); // Renamed myservo to myDoorServo
  }
}

void Open()
{
  // Display "Enter Password" on the LCD
  myLcd.setCursor(0, 0);
  myLcd.print("Enter Password");

  // Get the pressed key from the keypad
  pressedKey = myKeypad.getKey();
  if (pressedKey)
  {
    // Store the pressed key and display '*' on the LCD
    enteredPassword[passwordLength] = pressedKey;
    myLcd.setCursor(passwordLength, 1);
    myLcd.print(enteredPassword[passwordLength]);
    passwordLength++;
  }

  // Check if the entered password is complete
  if (passwordLength == Password_Length - 1)
  {
    // Check if the entered password is correct
    if (!strcmp(enteredPassword, masterPassword))
    {
      myLcd.clear();
      ServoOpen();                    // Open the door
      myLcd.print(" Door is Open ");
      isDoorOpen = true;                    // Set door status to open
      digitalWrite(redLED, LOW);     // Turn off red LED
      digitalWrite(greenLED, HIGH);   // Turn on green LED
      delay(5000);
      loading("Waiting");
      myLcd.clear();
      myLcd.print(" Time is up! ");
      delay(1000);
      ServoClose();                   // Close the door
      digitalWrite(greenLED, LOW);   // Turn off green LED
      isDoorOpen = false;                   // Set door status to closed
    }
    else
    {
      myLcd.clear();
      myLcd.print(" Wrong Password ");
      digitalWrite(redLED, HIGH);   // Turn on red LED for wrong password
      delay(1000);
      myLcd.clear();
      myLcd.print("Enter Password");
      digitalWrite(redLED, LOW);    // Turn off red LED
      isDoorOpen = false;      // Set door status to closed
    }
    delay(1000);
    myLcd.clear();
    clearEnteredPassword();
  }
}
