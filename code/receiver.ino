#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define DEBUG 0
#define TICKRATE 1
#define LOG DEBUG || TICKRATE

#define SPEED_1 5
#define DIR_1 4

#define SPEED_2 6
#define DIR_2 7

#define MATRIX_PIN 10
#define LED_COUNT 7
#define MAX_BRIGHTNESS 255

Adafruit_NeoPixel matrix(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

RF24 radio(8, 9);  // nRF24L01+ (CE, CSN)
int data[5];

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  #if LOG
  Serial.begin(115200);
  #endif

  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1, 0x1234567890LL);
  radio.startListening();
  //  radio.stopListening ();

  // motor pins
  for (int i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  matrix.begin();
  matrix.setBrightness(MAX_BRIGHTNESS);
}

int btnState;
int tmblrState;
int ptmrVal;

#ifdef TICKRATE
#define M_TICK_SIZE 32

unsigned int tickrate[M_TICK_SIZE];
int i = 0;
unsigned int old_v, new_v;
unsigned int sum, max;
#endif

void loop() {

  if (radio.available()) {
    radio.read(&data, sizeof(data));

    #if DEBUG
    Serial.println("");
    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
    Serial.println(data[3]);
    Serial.println(data[4]);
    #endif
    
    btnState = data[2];
    tmblrState = data[3];
    ptmrVal = data[4];

    // 0 - Forward, 1 - backward, 2 - right, 3 - left
    switch (data[0]) {
      case 0:
        {
          digitalWrite(DIR_1, HIGH);  // set direction
          analogWrite(SPEED_1, 255);  // set speed

          digitalWrite(DIR_2, HIGH);
          analogWrite(SPEED_2, 0);

          break;
        }
      case 1:
        {
          digitalWrite(DIR_1, LOW);
          analogWrite(SPEED_1, 0);

          digitalWrite(DIR_2, LOW);
          analogWrite(SPEED_2, 255);

          break;
        }
      case 2:
        {
          digitalWrite(DIR_1, HIGH);
          analogWrite(SPEED_1, 255);

          digitalWrite(DIR_2, LOW);
          analogWrite(SPEED_2, 255);

          break;
        }
      case 3:
        {
          digitalWrite(DIR_1, LOW);
          analogWrite(SPEED_1, 255);

          digitalWrite(DIR_2, HIGH);
          analogWrite(SPEED_2, 255);

          break;
        }
      case 4:
        {
          analogWrite(SPEED_1, 0);
          analogWrite(SPEED_2, 0);

          break;
        }
    }
  }

  matrix.clear(); // Set all pixel colors to 'off'
  if (!tmblrState) {
      Serial.println("LoL 1");
    if (!btnState) {
      Serial.println("LoL 2");
      matrix.fill(matrix.Color(255, 255, 255), 0, 7);
    } else {
      Serial.println("LoL 3");
      matrix.fill(matrix.Color(50, 50, 50), 0, 7);
    }
  } else {
    matrix.fill(matrix.Color(0, 0, 0), 0, 7);
  }
  matrix.show();
  
  #if TICKRATE
    new_v = millis();

    sum -= tickrate[i];
    tickrate[i] = new_v - old_v;
    sum += tickrate[i];
    max = 0;
    for (int j = 0; j < M_TICK_SIZE; j++) {
      max = (tickrate[j] > max) ? tickrate[j] : max;
    }
    
    i = (i == 31) ? 0 : i + 1;
    Serial.println("");
    Serial.print("Tickrate\t");
    Serial.println((float)sum / M_TICK_SIZE);
    Serial.print("Max ping\t");
    Serial.println(max);
    old_v = new_v;
    #endif
}