#include "EthernetLab5.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define number 0
#define pair 2
#define mod NON_PRES
#define BAUD_RATE 115200
#define TIME_SLOT 500
#define propTime 300
#define names_len 15
//states:
#define check_line 0
#define ex_backoff 1
#define transmit 2
#define stop_n_wait 3

int state = 0;

static unsigned long refTime = 0;
static unsigned long currTime, ackTime;
static unsigned long ex_time_ref;
static unsigned long prop_ref;
static unsigned long s_w_ref;

static int SN = 0; 
static int i = 0;
static char names[] = "Shaked & Priel";
static int data_lengh = strlen(names);
static char payload[names_len + 3] = {0};

//static int payload_size = strlen(payload);
static int payload_size = 18;
static int d_type = 0;
static void* payload_ptr = (&payload);
static int temp_c_l;
static char ACK_buffer[3];
static int ACK_buffer_size = 2;
static int is_new;
static unsigned long RTT = 2*propTime;
static unsigned long P_RTT = 2*propTime;
static unsigned long random_time;
static int n = 1;
static int temp_type;
static int U;
static int ACK;

void setup() {

  Serial.begin(BAUD_RATE);
  setAddress(number, pair);
  setMode(mod);
}

void loop() {
  non_persistent();
}

void non_persistent(){
  
  switch(state){

    case check_line:
      temp_c_l = checkLine();
      if(temp_c_l){
        Serial.println("free line");
        //Serial.println(i);
        i = 0;
        state = transmit;
      }
      else{
        ex_time_ref = millis();
        Serial.println("busy line");
        i++;
        U = pow(2, i+3) - 1;
        random_time = random(0,U) * TIME_SLOT;
        state = ex_backoff;
      }
    break;
    
    case ex_backoff:
      //Serial.println("i is:");
      //Serial.println(i);
      currTime = millis();
      if(currTime - ex_time_ref > random_time){
        if(i >= 7){
          i = 0;
        }
        state = check_line;
      }
    break;

    case transmit:
      payload[0] = d_type;
      payload[1] = SN;
      payload[2] = data_lengh;     
      for(int i = 0; i < data_lengh; i++){
        payload[i+4] = names[i];
      }

      startPackage(payload_ptr, payload_size);
      prop_ref = millis();
      currTime = millis();
      while(currTime - prop_ref < propTime){
        temp_c_l = checkLine();
        currTime = millis();
        if(temp_c_l == 0){
          Serial.println("collision occurred");
          endPackage(0);
          ex_time_ref = millis();
          i++;
          U = pow(2, i+3) - 1;
          random_time = random(0,U) * TIME_SLOT;
          state = ex_backoff;
          break;
        }
      }
      //Serial.println("end of prop while");
      //Serial.println(temp_c_l);
      if(temp_c_l == 1){
        endPackage(1);
        s_w_ref = millis();
        state = stop_n_wait;
      }
    break;

    case stop_n_wait:
      currTime = millis();
      is_new = readPackage(ACK_buffer, ACK_buffer_size);
      temp_type = ACK_buffer[0];
      ACK = ACK_buffer[1];
      //Serial.println("type:");
      //Serial.println(temp_type);
      if(is_new && temp_type){
        Serial.println("recived ACK:");
        Serial.println(ACK);
        SN = ACK_buffer[1];
        n++;
        P_RTT = RTT;
        RTT = ((n - 1.0) * P_RTT + (1.0 / n) + (currTime - s_w_ref))/n; 
        //Serial.println("RTT:");
        //Serial.println(RTT);
        state = check_line;
        break;
      }

      if(currTime - s_w_ref > RTT){
        n++;
        P_RTT = RTT;
        RTT = ((n - 1.0) * P_RTT + (1.0 / n) + (currTime - s_w_ref))/n;
        //Serial.println("RTT:");
        //Serial.println(RTT);
        state = check_line;
      }
    break;
  }
}





