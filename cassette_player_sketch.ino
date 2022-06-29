#include "Audio.h"
#include "src\esp32\SD.h"
#include "src\esp32\FS.h"
#include "src\esp32\SPI.h"

// Digital I/O used
#define SD_CS         33
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

#define BUTTON_PLAY_1 12   
#define BUTTON_PLAY_2 15  
#define BUTTON_STOP   32  

Audio audio;
int lastActionState = 3;

void setup(){

    pinMode(BUTTON_PLAY_1, INPUT_PULLUP);
    pinMode(BUTTON_PLAY_2, INPUT_PULLUP);
    pinMode(BUTTON_STOP, INPUT_PULLUP);

    Serial.begin(115200);

    if(!SD.begin(SD_CS)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }


    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21); // 0...21
//    cassetteAction(3);
}

void playAudio1(){
  audio.connecttoFS(SD, "nixon.wav");
}

void playAudio2(){
  audio.connecttoFS(SD, "nyt_vs_us.wav");
  
}

void stopAudio(){
  audio.pauseResume();
  audio.setAudioPlayPosition(0);
}

void cassetteAction(int state){
  if ( state == 1 ){ 
    Serial.println("play audio 1");
    playAudio1();
    delay(30);
  } 
  if ( state == 2 ){ 
    Serial.println("play audio 2");
    playAudio2();
    delay(30);
  } 
  if ( state == 3 ) { 
    Serial.println("pause audio");
    stopAudio();
    delay(30);
  }
//  delay(30);
}

void loop(){
  int play1_button_state = digitalRead(BUTTON_PLAY_1);
  int play2_button_state = digitalRead(BUTTON_PLAY_2);
//  int stop_button_state = digitalRead(BUTTON_STOP);
//  Serial.println(digitalRead(BUTTON_PLAY_1));
//  Serial.println(digitalRead(BUTTON_PLAY_2));
//  Serial.println(digitalRead(BUTTON_STOP));
//  Serial.println();
  audio.loop();
  int actionState = 3;
  if ( play1_button_state == LOW ){ 
    actionState = 1;
  } else if ( play2_button_state == LOW ){ 
    actionState = 2;
  }
  if(actionState != lastActionState) {
    cassetteAction(actionState);  
    lastActionState = actionState; 
  } 
}
