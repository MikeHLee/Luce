#include <Time.h>
#include <TimeAlarms.h>
#include <Stepper.h>
#include <DS1307RTC.h>


const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int rolePerMinute = 17;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup()
{
  // initialize the serial port:
  Serial.begin(9600);
  // SYNC the RTC and systime at boot
  setTime(RTC.get());   // RTC.get() - the function to get the time from an RTC in unix format (uint32_t)
  if(timeStatus()!= timeSet) 
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");  
  setSyncInterval(600); // set the number of seconds between re-sync of time
  
  myStepper.setSpeed(rolePerMinute);
 
  // create the alarms 
  Alarm.alarmRepeat(06,30,00, MorningAlarm);  // 
  Alarm.alarmRepeat(19,30,00, EveningAlarm);  //  
}

void loop(){  
  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
}

// functions to be called when an alarm triggers:
void MorningAlarm(){
  Serial.println("Opening Blinds");
  Serial.println("clockwise");
  myStepper.step(-5.5*stepsPerRevolution);
  delay(500); //stepper code for closing  
}

void EveningAlarm(){
  Serial.println("Closing Blinds"); 
  Serial.println("counterclockwise");
  myStepper.step(5.5*stepsPerRevolution);
  delay(500);//stepper code for closing here       
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
