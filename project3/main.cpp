#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "cstdlib"
#include "mbed_rpc.h"

BufferedSerial pc(USBTX,USBRX);
BufferedSerial uart(D1,D0);
BufferedSerial xbee(D10, D9);

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);

void carrunning(Arguments *in, Reply *out);
RPCFunction bLED(&carrunning, "carrunning");

void carcircling(Arguments *in, Reply *out);
RPCFunction bLED1(&carcircling, "carcircling");


int mode1 = 0;           // mode = 0 accept xbee
                        // mode = 2 circle
                        // mode = 1 line detection
                        // mode = 3 Apriltag

int b = 0;
int a = 0;

// for line detection
Thread t1;
EventQueue queue1(32 * EVENTS_EVENT_SIZE);
// for Apriltag
Thread t2;
EventQueue queue2(32 * EVENTS_EVENT_SIZE);

DigitalOut led1(LED1);
DigitalInOut pin10(D11);

void detectangle () {
    if (mode1 == 3) {
        if (a == 1) {
                car.turn(40, -0.8);  // turn right
                ThisThread::sleep_for(1s);
                car.stop();
                ThisThread::sleep_for(1s);
        }
        else if (a == 2) {
                car.turn(30, -0.9);  // turn right
                ThisThread::sleep_for(1s);
                car.stop();
                ThisThread::sleep_for(1s);
            } 
        else if (a == 3) {
                car.goStraight(20);  
                ThisThread::sleep_for(1s);
                car.stop();
                ThisThread::sleep_for(1s);
            }
        else if (a == 4) {
                car.goStraight(20);
                ThisThread::sleep_for(1s);
                car.stop();
                ThisThread::sleep_for(1s);
        }
        else if (a == 5) {
                car.goStraight(20);  
                ThisThread::sleep_for(1s);
                car.stop();
                ThisThread::sleep_for(1s);
        } else if (a == 6) {
                car.turn(30, 0.9);  // turn left
                ThisThread::sleep_for(1s);
                car.stop();
                ThisThread::sleep_for(1s);
        } else if (a == 7) {
                car.turn(40, -0.8);  // turn left
                ThisThread::sleep_for(1s);
                car.stop();
                ThisThread::sleep_for(1s);
        } else if (a == 8) {
                car.stop();
                ThisThread::sleep_for(2s);
        } else if (a == 0) {
                car.stop();
                ThisThread::sleep_for(2s);
        }
    }
}
void detectline () {
    if (mode1 == 1 && b == 1) {
      car.goStraight(50);
      ThisThread::sleep_for(1s);
      car.stop();
      ThisThread::sleep_for(1s);  
    }
}

void carrunning(Arguments *in, Reply *out){

    uart.set_baud(9600);
    int x = in->getArg<int>();
    //mode1 = in->getArg<int>();
    //mode1 = x;
    //t2.start(callback(&queue2, &EventQueue::dispatch_forever));
    //queue2.call_every(2s, &detectangle);
    t1.start(callback(&queue1, &EventQueue::dispatch_forever));
    queue1.call_every(2s, &detectline);    
    //char buffer[200];
    //parallax_ping  ping1(pin10);

    mode1 = x;
    while(mode1 == 1) {
        //mode1 = x;
        /*if (mode1 == 1) {*/
            if(uart.readable()){
                char recv[1];
                uart.read(recv, sizeof(recv));
                //pc.write(recv, sizeof(recv));
                b = atoi(recv);
                //sprintf(buffer, "%d", b);
                //xbee.write(buffer, sizeof(buffer));
                //if (b == 1) {
                //    car.goStraight(50);
                //    ThisThread::sleep_for(1s);
                //    car.stop();
                //    ThisThread::sleep_for(1s);
                //} else if (b == 0) {
                //    car.stop();
                //    ThisThread::sleep_for(2s);
                    //sprintf(buffer, "%d", b);
                    //xbee.write(buffer, sizeof(buffer));
                //}
            }
            //if (b == 0){break;}
        //} //else {car.stop();}
        /*else if (x == 3) {
            if((float)ping1>10) {
                led1 = 1;
                if(uart.readable()){
                    char recv[1];
                    uart.read(recv, sizeof(recv));
                    //pc.write(recv, sizeof(recv));
                    a = atoi(recv);
                }
            }
            else {
                a = 8;
                led1 = 0;
                break;
            }
            ThisThread::sleep_for(10ms);

        }*/
        /*else {
            car.stop();
            ThisThread::sleep_for(1s);
        }*/
    }
    b = 0;
}
void carcircling(Arguments *in, Reply *out){
    int y = in->getArg<int>();
    //mode1 = in->getArg<int>();
    mode1 = y;
    if (mode1 == 2) {
            car.turn(80, -0.3);
            ThisThread::sleep_for(1s);
            ThisThread::sleep_for(500ms);
            car.stop();
            ThisThread::sleep_for(500s);
            //car.goStraight(50);
            //ThisThread::sleep_for(1s);
            //car.stop();
            //ThisThread::sleep_for(1s);
            //car.turn(80, 0.3);
            //ThisThread::sleep_for(1s);
            //ThisThread::sleep_for(500ms);
            //car.stop();
            //ThisThread::sleep_for(1s);
    }
}

int main() {

   //uart.set_baud(9600);
   pc.set_baud(9600);
   //parallax_ping  ping1(pin10);
   //t1.start(callback(&queue1, &EventQueue::dispatch_forever));
   //queue1.call(carrunning);

   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   char buffer[200];
   while (1) {  // mode == 0
    /*while(1) {*/
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   sprintf(buffer, "%s", buf);
   xbee.write(buffer, sizeof(buffer));
   //printf("%s",buf);
   }
}