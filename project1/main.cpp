#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "mbed_rpc.h"
#include "cstdlib"

//void parkingcar(Arguments *in, Reply *out);
//RPCFunction bLED(&parkingcar, "parkingcar");
BufferedSerial pc(USBTX,USBRX);
BufferedSerial uart(D1,D0);
//BufferedSerial uart(D10,D9);
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
//BufferedSerial xbee(D1, D0);
BufferedSerial xbee(D10, D9);

BBCar car(pin5, pin6, servo_ticker);

void modeselect(Arguments *in, Reply *out);
RPCFunction bLED(&modeselect, "modeselect");

int mode = 0;           // mode = 0 accept xbee
                        // mode = 1 line detection
                        // mode = 2 Apriltag
// for line detection
Thread t1;
EventQueue queue1(32 * EVENTS_EVENT_SIZE);
// for Apriltag
Thread t2;
EventQueue queue2(32 * EVENTS_EVENT_SIZE);

DigitalOut led1(LED1);
DigitalInOut pin10(D11);
int a = 0;  // for Apriltagrotation
int b = 0;

void detectangle() {
    if (mode == 1) {
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
                ThisThread::sleep_for(1s);
        }
    }
}

void carrunning(){
    t2.start(callback(&queue2, &EventQueue::dispatch_forever));
    queue2.call_every(2s, &detectangle);
    char buffer[2];
    parallax_ping  ping1(pin10);
    while(1) {
        if (mode == 1) {
            if(uart.readable()){
                char recv[1];
                uart.read(recv, sizeof(recv));
                pc.write(recv, sizeof(recv));
                b = atoi(recv);
                if (b == 1) {
                    car.goStraight(20);
                    ThisThread::sleep_for(500ms);
                    car.stop();
                    ThisThread::sleep_for(500ms);
                } else if (b == 0) {
                    sprintf(buffer, "1");
                    xbee.write(buffer,sizeof(buffer));
                }
            }
        } 
        else if (mode == 2) {
            car.turn(50, 0.3);
            ThisThread::sleep_for(1s);
            car.stop();
            if((float)ping1>25) {
                led1 = 1;
                if(uart.readable()){
                    char recv[1];
                    uart.read(recv, sizeof(recv));
                    pc.write(recv, sizeof(recv));
                    a = atoi(recv);
                }
            }
            else {
                a = 8;
                led1 = 0;
                break;
            }
            ThisThread::sleep_for(10ms);
        }
        
    }
}

int main() {

   uart.set_baud(9600);
   pc.set_baud(9600);

   t1.start(callback(&queue1, &EventQueue::dispatch_forever));
   queue1.call(carrunning);

   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
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
   }
}

void modeselect(Arguments *in, Reply *out){
    int x = in->getArg<int>();
    mode = x;
    //uart.write(("%d", mode).deocde());
}

