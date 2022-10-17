int Button_start_pin = 8;
int speaker_output = 6;
int scl_lcd = A5, sda_lcd = A4;                                                 //LCD outputs
int button_cmd_input = 7, dial_cmd_input = A2, microphone_cmd_input = A3;      //Command Inputs
int correct_led = 10, wrong_led = 9;                                           //LED pins for controlling right and wrong answer
int led_button = 0, led_dial = 1, led_microphone = 2;                           //LED pins that light up with each commands

 int command_num = 0;

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



void loop() {
  // put your main code here, to run repeatedly:




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
