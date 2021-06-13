#include"mbed.h"
#include "mbed_rpc.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BufferedSerial xbee(D10, D9);

//void linedetection(Arguments *in, Reply *out);
//RPCFunction bLED(&linedetection, "linedetection");

void aprildetection(Arguments *in, Reply *out);
RPCFunction bLED1(&aprildetection, "aprildetection");

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);

DigitalOut led1(LED1);
DigitalInOut pin10(D11);

int mode;
int main(){
   uart.set_baud(9600);
   pc.set_baud(9600);
   char buf[256], outbuf[256];
   led1 = 1;
   char buffer[200];
   while(1){
      for (int i = 0; ; i++){
            char *recv = new char[1];
            uart.read(recv, 1);
            buf[i] = *recv;
            if (*recv == '\n') {break;}
      }
      RPC::call(buf, outbuf);
      sprintf(buffer, " %s \r\n %s \r\n", buf, outbuf);
      xbee.write(buffer, sizeof(buffer));
   }
}

void aprildetection(Arguments *in, Reply *out){
   //char outbuf[256];
    int a = in->getArg<int>();
    int y1 = in->getArg<int>();
        if (a == 1) {
            car.turn(40, 0.8);  // turn left
            ThisThread::sleep_for(50ms);
            car.stop();
            ThisThread::sleep_for(500ms);
        } else if (a == 2) {
            car.turn(40, -0.8);  // turn right
            ThisThread::sleep_for(500ms);
            car.stop();
            ThisThread::sleep_for(500ms); 
        } else if (a == 3) {
            car.goStraight(50);
            ThisThread::sleep_for(600ms);
            car.stop();
            ThisThread::sleep_for(400ms);
        } else {
            car.stop();
            ThisThread::sleep_for(1s);
        }
        out->putData(a);
}


