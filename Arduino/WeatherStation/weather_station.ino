// include the library code:
#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
#include <Wire.h>
#include <DS3231.h> 

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

String str_temperature, str_humidity;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Clock Objects and Strings
String year,month,day,hour,minute,second, time;
DS3231 clock;
RTCDateTime dt;


void setup() {
  lcd.begin(16, 2);
  clock.begin();
  /* Code needed for the first time the sensor is used */
  //clock.setDateTime(__DATE__, __TIME__);   

  lcd.print("Starting");
  lcd.setCursor(0,1);
  lcd.print("Weather Station");
  delay(2000);
  
  lcd.clear();
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void getTime(){
  dt = clock.getDateTime();

  year = String(dt.year);
  month = String(dt.month);
  day = String(dt.day);
  hour = String(dt.hour);
  minute = String(dt.minute);
  second = String(dt.second);
  
  if(dt.month < 10) month = "0"+month;
  if(day.toInt() < 10) day = "0"+day;
  if(dt.hour < 10) hour = "0"+hour;
  if(dt.minute < 10) minute = "0"+minute;
  if(dt.second < 10) second = "0"+second;
  
  time = hour +":"+minute+":"+second;
}


void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  getTime();
  lcd.setCursor(0,0);
  lcd.print("Time: " + time);
  
  float temperature;
  float humidity;

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  {
    //temp_dos = temp_uno + String(temperature);
    
    str_temperature = "T=" + String(int(temperature)) + (char)223 + "C";
    str_humidity = "H=" + String(int(humidity))  + "%";
    
    
    
    lcd.setCursor(0, 1);
    lcd.print(str_temperature);
    //lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.print(str_humidity);

  }
}
