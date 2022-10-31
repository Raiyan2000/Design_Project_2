#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int Button_start_pin = 8;
int speaker_output = 6;
int scl_lcd = A5, sda_lcd = A4;                                                 //LCD outputs
int button_cmd_input = 7, dial_cmd_input = A2, microphone_cmd_input = A3;      //Command Inputs
int correct_led = 10, wrong_led = 9;                                           //LED pins for controlling right and wrong answer
int led_button = 0, led_dial = 1, led_microphone = 2;                           //LED pins that light up with each commands

int command_num;
bool game_is_on = true;
int answer_time;
bool correct_answer;
int temp_dial_volt;
int userPoints = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(button_cmd_input, INPUT);
  pinMode(dial_cmd_input, INPUT);
  pinMode(microphone_cmd_input, INPUT);
  pinMode(Button_start_pin, OUTPUT);
  pinMode(speaker_output, OUTPUT);
  pinMode(scl_lcd, OUTPUT);
  pinMode(sda_lcd, OUTPUT);
  pinMode(correct_led, OUTPUT);
  pinMode(wrong_led, OUTPUT);
  pinMode(led_button, OUTPUT);
  pinMode(led_dial, OUTPUT);
  pinMode(led_microphone, OUTPUT);

  Serial.begin(115200);
  //Might have to change address
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 10);

}

bool buttonInput(int responseTime)
{
    unsigned long start = millis(); 
    digitalWrite(led_button, HIGH);

    while (millis() - start < responseTime)
    {
      if (digitalRead(button_cmd_input) == HIGH)
      {
        digitalWrite(correct_led, HIGH);
        delay(500);
        digitalWrite(correct_led, LOW);
        digitalWrite(led_button, LOW);
        return true;
      }
    }
    //digitalWrite(correct_led, LOW);
    digitalWrite(wrong_led, HIGH);
    delay(500);
    digitalWrite(wrong_led, LOW);
    digitalWrite(led_button, LOW);
    
    return false;
}

bool micInput(int responseTime){
   unsigned long start= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
   unsigned int sound;
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   //digitalWrite(microphone_cmd_input, HIGH);
  
   // collect data for responseTime miliseconds
   while (millis() - start < responseTime)
   {
     sound = analogRead(microphone_cmd_input);
     //This is the max of the 10-bit ADC so this loop will include all readings
     if (sound < 1024){
        if (sound > signalMax){
           signalMax = sound;  // save just the max levels
        }
        else if (sound < signalMin){
           signalMin = sound;  // save just the min levels
        }
     }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5) / 1024;  // convert to volts

   return (volts>=2.0) ? true:false;
}

bool dialInput(int responseTime, int temp_volt)
{
  unsigned long start = millis();
  bool state;
  int current_volt;
  temp_dial_volt = analogRead(dial_cmd_input);

  if(temp_dial_volt < 300)
  {
    //Low
    state = false;
  }
  else if(temp_dial_volt > 1000)
  {
    //High
    state = true;
  }

  digitalWrite(led_dial, HIGH);
  

  while (millis() - start < responseTime)
  {
    current_volt = analogRead(dial_cmd_input);

    //high
    if (state == true)
    {
      if (current_volt < 400)
      {
        digitalWrite(led_dial, LOW);
        digitalWrite(correct_led, HIGH);
        delay(2000);
        digitalWrite(correct_led, LOW);
        return true;
      }
    }
    //low
    else
    {
      if (current_volt > 700)
      {
        digitalWrite(led_dial, LOW);
        digitalWrite(correct_led, HIGH);
        delay(2000);
        digitalWrite(correct_led, LOW);
        return true;
      }
    }
    
  }
  //digitalWrite(correct_led, LOW);
  digitalWrite(led_dial, LOW);
  digitalWrite(wrong_led, HIGH);
  delay(2000);
  digitalWrite(wrong_led, LOW);
  
  
  return false;
}

void speakerOutput(int command)
{
  if (command == 0)
  {
     digitalWrite(speaker_output, HIGH);
     delay(200);
     digitalWrite(speaker_output, LOW);
  }
  else if (command == 1)
  {
    digitalWrite(speaker_output, HIGH);
    delay(200);
    digitalWrite(speaker_output, LOW);
    delay(200);
    digitalWrite(speaker_output, HIGH);
    delay(200);
    digitalWrite(speaker_output, LOW);

  }
  else
  {
    digitalWrite(speaker_output, HIGH);
    delay(200);
    digitalWrite(speaker_output, LOW);
    delay(200);
    digitalWrite(speaker_output, HIGH);
    delay(200);
    digitalWrite(speaker_output, LOW);
    delay(200);
    digitalWrite(speaker_output, HIGH);
    delay(200);
    digitalWrite(speaker_output, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Check if start button is pressed which would start the game
  display.clearDisplay();
  display.println("Start");
  display.display();
  
  if(digitalRead(Button_start_pin) == HIGH)
  {
    game_is_on = true;
  }
  else
  {
    game_is_on = false;
  }


  while(game_is_on)
  {
    display.clearDisplay();
    display.println("Points: ");
    display.display();

    digitalWrite(correct_led, LOW);

    //choose random command
    command_num = random(0,2);
    //command_num = 0;
    answer_time = 1000;


    //if command is pressing the button
    if (command_num == 0)
    {
      //Speaker beeps for button
      speakerOutput(command_num);

      correct_answer = buttonInput(answer_time);
    } 
    else if (command_num == 1)
    {
      //Read current state of potentiometer
      temp_dial_volt = analogRead(dial_cmd_input);

      //Speaker beeps for dial
      speakerOutput(command_num);

      correct_answer = dialInput(answer_time, temp_dial_volt);
    }
    else
    {
      //speaker beeps for microphone
      speakerOutput(command_num);

      correct_answer = micInput(answer_time);
    }
  

    if (correct_answer == false)
    {
      display.clearDisplay(); 
      display.println("Points: ");
      display.print(userPoints, DEC);
      delay(1000);
      //sprintf(buf, "%d", points);
      //u8g.drawStr(18, 3, buf);
      //Call endgame function
      game_is_on = false;
      //Set the timer to default value
      answer_time = 2000;

      userPoints = 0;

    }
    else
    {
     
      //Reduce answer time by 5ms
      answer_time -= 5;

      //Update display
      userPoints += 1;

      display.clearDisplay(); 
      display.println("Points: ");
      display.print(userPoints, DEC);
      delay(1000);
    }

    

  }
}

