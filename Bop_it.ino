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


}

bool buttonInput(int responseTime)
{
    //Speaker beep for button

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
    digitalWrite(correct_led, LOW);
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

bool dialInput(int responseTime)
{


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
    //choose random command
    //command_num = random(0,2);

    //Choose one command randomly
    command_num = 0;
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
      //Speaker beeps for dial
      speakerOutput(command_num);
    }
    else
    {
      //speaker beeps for microphone
      speakerOutput(command_num);

    }
  

    if (correct_answer == false)
    {
      //Call endgame function
      game_is_on = false;
    }

  }
}

