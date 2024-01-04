#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS3231.h> //mengincludekan library DS3231
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#define BUTTON_PIN 8
Adafruit_SSD1306 display(OLED_RESET);
DS3231  rtc(SDA, SCL); // inisialisasi penggunaan i2c
Time  t;
int buttonState=0;
bool flag=true;
bool kondisiBuzzer=false;
const int button = 0;
long timer;
float jarak;
const int trigPin = 4;
const int echoPin = 5;
const int Buzzer = 10;
int halaman = 0;
int kondisi_sekarang = 0;
float jarak_lama =0;
float jarak_sekarang=0;
float jumlah_senin;
float jumlah_selasa;
float jumlah_rabu;
float jumlah_kamis;
float jumlah_jumat;
float jumlah_sabtu;
float jumlah_minggu;
float jumlah_minggu_ini = 0.0;

int seconds, minutes, hours;
int read_value = 0; //read sensor reading in cm
float average_water_level = 0; //store average of multiple reading
int water_amount_in_ounce = 0; //store calculated amount of water

int idle_time = 0;
int intake_day = 1;
int previous_value = 0;

long previous_state = millis();
int count = 1;
int daily_intake = 0;
int weekly_intake = 0;
long sleep_time = millis();

unsigned long awalcek = 0;
unsigned long waktucek = 0;
unsigned long waktuawal = 0;
unsigned long waktuawal1 = 0;
unsigned long waktusekarang = 0;
unsigned long waktuganti = 0;
int belum_minum = 0;
int jumlah_minum = 0;
int jumlah_minum_total = 0; 
float jarak1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  rtc.begin();
  //setting pertama download program
  rtc.setDate(12, 12, 2023);   //mensetting tanggal 07 april 2018
  rtc.setTime(14, 38, 00);     //menset jam 22:00:00
  rtc.setDOW(2);     //menset hari "Sabtu"
  pinMode(Buzzer, OUTPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(button, INPUT);
  rtc.begin();
  waktuawal = millis();
  waktuawal1 = millis();
  waktucek = millis();
  waktuganti = millis();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Welcome Mr. Jo");
  display.println("Please wait...");
  display.display();
  display.clearDisplay();
}

void loop() {
  digitalWrite(trigPin, LOW);                   
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);                  
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);                   
  timer = pulseIn(echoPin, HIGH);
  jarak1 = 19 - (timer/58);
  delay(100);
//  Serial.print("Jarak 1 = ");
//  Serial.println(jarak1);
  
  buttonState=digitalRead(BUTTON_PIN);
  if(buttonState==0){
    if(flag==true){
      kondisiBuzzer=!kondisiBuzzer;   
      flag=false;
    }
  }
  else{
    flag=true;
  }

  if(awalcek == 0){  
    jarak_lama = jarak1;
    awalcek = 1;
    Serial.print("Jarak Lama Awal: ");
    Serial.println(jarak_lama);
  }
  
 if(millis() - waktucek >= 5000){
    jarak_sekarang = jarak1;
    int selisih = jarak_lama - jarak_sekarang;
    Serial.print("(Jarak L = ");
    Serial.println(jarak_lama);
    Serial.print("Jarak Selisih = ");
    Serial.println(selisih);
    Serial.print("Jarak Sekarang = ");
    Serial.println(jarak_sekarang);
    
    if((selisih) <= 2){
      belum_minum =1;
      jarak_lama = jarak_sekarang;
      Serial.println("Blm minum )");
    }
    
    if((selisih) > 2){
      belum_minum = 0;
      jumlah_minum = jarak_lama - jarak_sekarang;
      jumlah_minum_total += jumlah_minum;
      jarak_lama = jarak_sekarang;
      Serial.println("Udah minum");
      Serial.println(jumlah_minum_total);
      waktuawal = millis();
    }
    
    waktucek = millis();
  }  


  
  if(((millis() - waktuawal) > 15000) and (belum_minum=1)){
    if(kondisiBuzzer==false){
      Serial.println("NYALA");
      alert();
      belum_minum = 0;
      waktuawal = millis();
    }
    else{
      alert2();
      belum_minum = 0;
      Serial.println("MATI");
      waktuawal = millis();
    }
  }
  OLED();
  read_time();
}

void alert(){
  digitalWrite(Buzzer, HIGH); delay(100);
  digitalWrite(Buzzer, LOW); delay(100);
  Serial.println("buzzer nyala");
  }

void alert2(){
  if (millis() - waktuganti >= 5000)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Don't forget to drink your water!");
  display.setTextSize(1);
  display.println("^^");
  display.display();
  display.clearDisplay();
}

void read_time() {
  t = rtc.getTime();
  hours = t.hour;
  minutes = t.min;
  seconds = t.sec;
//  Serial.print(rtc.getDOWStr()); //prosedur pembacaan hari
//  Serial.print(" ");
//  
//  Serial.print(rtc.getDateStr()); //prosedur pembacaan tanggal
//  Serial.print(" -- ");
// 
//  Serial.println(rtc.getTimeStr()); //prosedur pembacaan waktu
  } 

//int distance_in_cm(){
//  digitalWrite(trigPin, LOW);                   
//  delayMicroseconds(2);
//  digitalWrite(trigPin, HIGH);                  
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);                   
//  timer = pulseIn(echoPin, HIGH);
//  jarak = timer/58;
//  jarak1 = 19 - jarak;
//  
//  return jarak1;
//}

void OLED(){
  float Kapasitas_air = (jarak1)/19;
  float Persen = Kapasitas_air*100;
  String persen = "%";
  String ml = " ml";
//  Serial.println(Kapasitas_air);
//  Serial.println(Persen);
  String Hari = rtc.getDOWStr();
  if (Hari = "Monday"){
    Hari = "Mon";}
  else if (Hari = "Tuesday"){
    Hari = "Tue";}
  else if (Hari = "Wednesday"){
    Hari = "Wed";}
  else if (Hari = "Thursday"){
    Hari = "Thu";}
  else if (Hari = "Friday"){
    Hari = "Fri";}
  else if (Hari = "Saturday"){
    Hari = "Sat";}
  else if (Hari = "Sunday"){
    Hari = "Sun";}

   if (Hari == "Mon" and hours == 23){
    jumlah_senin = jumlah_minum_total;
    jumlah_minum_total = 0;}
  else if (Hari == "Tue" and hours == 23){
    jumlah_selasa = jumlah_minum_total;
    jumlah_minum_total = 0;}
  else if (Hari == "Wed" and hours == 23){
    jumlah_rabu = jumlah_minum_total;
    jumlah_minum_total = 0;}
  else if (Hari == "Thu" and hours == 23){
    jumlah_kamis = jumlah_minum_total;
    jumlah_minum_total = 0;}
  else if (Hari == "Fri" and hours == 23){
    jumlah_jumat = jumlah_minum_total;
    jumlah_minum_total = 0;}
  else if (Hari == "Sat" and hours == 23){
    jumlah_sabtu = jumlah_minum_total;
    jumlah_minum_total = 0;}
  else if (Hari == "Sun" and hours == 23){
    jumlah_minggu = jumlah_minum_total;
    jumlah_minum_total = 0;
    jumlah_minggu_ini = jumlah_senin + jumlah_selasa + jumlah_rabu + jumlah_kamis + jumlah_jumat + jumlah_sabtu + jumlah_minggu;
    }
  if(millis() - waktuganti >= 10000){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(Hari+ " " +rtc.getDateStr());
    display.setTextSize(1);
    display.println(rtc.getTimeStr()); //prosedur pembacaan waktu
    display.print("Weekly intake ");
    display.println(jumlah_minggu_ini + ml);
    display.display();
    display.clearDisplay();
   }
  if(millis() - waktuganti >= 20000){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Water Level: ");
    display.setTextSize(1);
    display.println(Persen+persen);
    display.println("Today's intake: ");
    display.println(jumlah_minum_total + ml);
    display.display();
    display.clearDisplay();
    waktuganti = millis();
  }
}