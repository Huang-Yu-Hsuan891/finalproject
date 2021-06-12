#include"mbed.h"
#include "mbed_rpc.h"
#include "bbcar.h"
//#include "cstdlib"
#include "bbcar_rpc.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

void linedetection(Arguments *in, Reply *out);
RPCFunction bLED(&linedetection, "linedetection");

//void carcircle(Arguments *in, Reply *out);
//RPCFunction bLED2(&carcircle, "carcircle");

void aprildetection(Arguments *in, Reply *out);
RPCFunction bLED1(&aprildetection, "aprildetection");

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);

DigitalOut led1(LED1);
DigitalInOut pin10(D11);
int mode;

int main(){
   //t1.start(callback(&queue, &EventQueue::dispatch_forever));
   //queue.call(detectline);
   //encoder_ticker.attach(&encoder_control, 10ms);
   uart.set_baud(9600);
   pc.set_baud(9600);
   char buf[256], outbuf[256];
   //FILE *devin = fdopen(&xbee, "r");
   //FILE *devout = fdopen(&xbee, "w");
   parallax_ping  ping1(pin10);
   led1 = 1;
   while(1){
    /*if((float)ping1>25) {
        led1 = 1;*/
      led1 = 0;
      for (int i = 0; ; i++){
            char *recv = new char[1];
            uart.read(recv, 1);
            buf[i] = *recv;
            if (*recv == '\n') {break;}
      }
      printf("%s\r\n", buf);
      RPC::call(buf, outbuf);
      printf("%s\r\n", outbuf);
    /*}
    else {
         led1 = 0;
         mode = 3;
         car.stop();
         break;
    }
    ThisThread::sleep_for(10ms);*/
   }
}
void linedetection(Arguments *in, Reply *out){
   //char outbuf[256];
    int x = in->getArg<int>();
    int y = in->getArg<int>();
    if(x ==1) {
        car.goStraight(30);
        ThisThread::sleep_for(500ms);
        //car.stop();
        //ThisThread::sleep_for(500ms);
        out->putData(x);
    }
}

void aprildetection(Arguments *in, Reply *out){
   char outbuf[256];
    int a = in->getArg<int>();
    int y1 = in->getArg<int>();
    //x1 = a
    if (mode != 3){
        out->putData(a);
        if (a == 1) {
                car.turn(40, -0.8);  // turn right
                ThisThread::sleep_for(500ms);
                //car.stop();
                //ThisThread::sleep_for(1s);
        }
        else if (a == 2) {
                car.turn(30, -0.9);  // turn right
                ThisThread::sleep_for(500ms);
                //car.stop();
                //ThisThread::sleep_for(1s);
            } 
        else if (a == 3) {
                car.goStraight(20);  
                ThisThread::sleep_for(500ms);
                //car.stop();
                //ThisThread::sleep_for(1s);
            }
        else if (a == 4) {
                car.goStraight(20);
                ThisThread::sleep_for(500ms);
                //car.stop();
                //ThisThread::sleep_for(1s);
        }
        else if (a == 5) {
                car.goStraight(20);  
                ThisThread::sleep_for(500ms);
                //car.stop();
                //ThisThread::sleep_for(1s);
        } else if (a == 6) {
                car.turn(30, 0.9);  // turn left
                ThisThread::sleep_for(500ms);
                //car.stop();
                //ThisThread::sleep_for(1s);
        } else if (a == 7) {
                car.turn(40, -0.8);  // turn left
                ThisThread::sleep_for(500ms);
                //car.stop();
                //ThisThread::sleep_for(1s);
        } else if (a == 8) {
                car.stop();
               ThisThread::sleep_for(500ms);
        } else if (a == 0) {
                car.stop();
                ThisThread::sleep_for(500ms);
        }
    }
}
