#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DEBUG 0
#define TICKRATE 1
#define LOG DEBUG || TICKRATE

RF24 radio(9, 10);
int data[5];  

const int xPin = A0;
const int yPin = A1;
const int buttonPin = 4;
const int ptmrPin = A2;
const int tumblrPin = 6;

int xPosition = 0;
int yPosition = 0;
#define POS_DELTA 25
#define POS_STOP 150

int buttonState = 0;
int tumblrState = 0;
int ptmrState = 0;

int position = 4;
int speed = 0;
int direction = 0;
                                
void setup(){
    radio.begin();
    radio.setChannel(100); // Channel (from 0 to 127), 5 - freq 2,405 GHz (only 1 receiver on a single channel and up to 6 transmitters)
    radio.setDataRate     (RF24_1MBPS); // Speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS)
    radio.setPALevel      (RF24_PA_HIGH); // Transmitter power (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe (0x1234567890LL); 
    #if LOG
    Serial.begin(115200);
    #endif
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(tumblrPin, INPUT);
    pinMode(ptmrPin, INPUT);
}

#ifdef TICKRATE
#define M_TICK_SIZE 32

unsigned int tickrate[M_TICK_SIZE];
int i = 0;
unsigned int old_v, new_v;
unsigned int sum, max;
#endif

void loop(){
    xPosition = analogRead(xPin);
    yPosition = analogRead(yPin);
    buttonState = digitalRead(buttonPin);
    tumblrState = digitalRead(tumblrPin);
    ptmrState = analogRead(ptmrPin);

    #if DEBUG
    Serial.println("");
    Serial.println("");
    Serial.print("x\t");
    Serial.println(xPosition);
    Serial.print("y\t");
    Serial.println(yPosition);
    Serial.print("btn\t");
    Serial.println(buttonState);
    Serial.print("tmb\t");
    Serial.println(tumblrState);
    Serial.print("ptr\t");
    Serial.println(ptmrState);
    #endif

    xPosition -= 512;
    yPosition -= 512;

    position = ((abs(xPosition) - abs(yPosition)) > POS_DELTA) ? xPosition : ((abs(yPosition) - abs(xPosition)) > POS_DELTA) ? yPosition : position;
    direction = ((abs(xPosition) - abs(yPosition)) > POS_DELTA) ? 2 : ((abs(yPosition) - abs(xPosition)) > POS_DELTA) ? 0 : direction;
    direction += (position > 0) ? 0 : 1;

    bool flag = (abs(xPosition) < POS_STOP && abs(yPosition) < POS_STOP);

    direction = (flag) ? 4 : direction;
    speed = (flag) ? 0 : map(abs(position), 0, 512, 0, 255);
    
    #if DEBUG
    Serial.print("pos\t");
    Serial.println(position);
    Serial.print("speed\t");
    Serial.println(speed);
    Serial.print("dir\t");
    Serial.println(direction);
    #endif

    data[0] = direction;
    data[1] = speed;
    data[2] = !buttonState;
    data[3] = tumblrState;
    data[4] = ptmrState;
    radio.write(&data, sizeof(data)); // check delivery: if(radio.write(&data, sizeof(data)))

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