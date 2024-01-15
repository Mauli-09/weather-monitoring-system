#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define Rain 36
#define TH 5
#define LDR 4
#define FLAME_SENSOR_PIN 2
#define SOIL_MOISTURE_PIN 14
#define DHT_PIN 5             // Digital pin connected to the DHT sensor
#define DHT_TYPE DHT11        // Type of DHT sensor


// I2C LCD configuration
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Adjust the address if needed (use an I2C scanner to find the address)

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  delay(500);
  dht.begin();

  pinMode(LDR, INPUT);
  pinMode(Rain, INPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);

  analogReadResolution(12);


  // Initialize LCD
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD
  lcd.backlight();

  // Print message
  lcd.setCursor(0, 0);
  lcd.print("Weather");
  lcd.setCursor(2, 1);
  lcd.print("Monitoring");
  lcd.setCursor(4, 2);
  lcd.print("System");
  lcd.setCursor(6, 3);
  lcd.print("Loading...");
  delay(4000);
  lcd.clear();

}
//Get the rain sensor values
void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;

  lcd.setCursor(0, 1);
  lcd.print("RF:");
  lcd.print(Rvalue);
  lcd.print(" ");
  Serial.println(Rvalue);

}

void flame() {
  int flameStatus = digitalRead(FLAME_SENSOR_PIN);

  lcd.setCursor(6, 1);
  lcd.print("Flame:");
  lcd.setCursor(12, 1);

  if (flameStatus == HIGH) {
    lcd.print("Detect  ");
    Serial.println("Detect  ");
  } else {
    lcd.print("Not Dect");
    Serial.println("Not Dect");
  }
}



//Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  int t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(t);
  lcd.setCursor(7, 0);
  lcd.print("^C");

  lcd.setCursor(10, 0);
  lcd.print("Humd:");
  lcd.print(h);

}

void soil() {
  int soilMoistureValue = digitalRead(SOIL_MOISTURE_PIN);  // Assuming soil moisture sensor is digital
  int moisturePercentage = map(soilMoistureValue, 0, 1023, 0, 100);
  lcd.setCursor(0, 2);
  lcd.print("SoilMoisture:");
 // Check the soil moisture level and display accordingly
  if (soilMoistureValue == HIGH) {
    lcd.setCursor(14, 2);
    lcd.print("Dry");
  } else {
    lcd.setCursor(14, 2);
    lcd.print("Wet");
  }
  {
    Serial.print("Soil Moisture: ");
    Serial.print(soilMoistureValue);

  }

}


//Get the LDR sensor values  
void LDRsensor() {
  // Read analog value from LDR
  int ldrValue = analogRead(LDR);

  // Determine LDR status
  String ldrStatus = (ldrValue > 500) ? "NIGHT" : "DAY  ";

  // Display data on LCD
  lcd.setCursor(0, 3);
  lcd.print("LDR:");
  lcd.print(ldrStatus);
  delay(1000);
}

void smoke(){
 
  int mq2_level=analogRead(12);
  mq2_level = map(mq2_level, 4095, 0, 100, 0);

  lcd.setCursor(11, 3);
  lcd.print("Smoke: ");
  lcd.setCursor(17, 3);
  lcd.print(mq2_level);

}

  void loop() {
  rainSensor();
  DHT11sensor();
  soil();
  LDRsensor();
  smoke();
  flame();
}
