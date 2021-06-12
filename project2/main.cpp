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

void modeselect(Arguments *in, Reply *out);
RPCFunction bLED(&modeselect, "modeselect");

int mode = 0;           // mode = 0 accept xbee
                        // mode = 1 line detection
                        // mode = 2 Apriltag
int b = 0;

// for line detection
Thread t1;
EventQueue queue1(32 * EVENTS_EVENT_SIZE);
// for Apriltag
Thread t2;
EventQueue queue2(32 * EVENTS_EVENT_SIZE);


void carrunning(){

    uart.set_baud(9600);
    t2.start(callback(&queue2, &EventQueue::dispatch_forever));
    //queue2.call_every(2s, &detectangle);
    char buffer[200];
    //parallax_ping  ping1(pin10);
    while(1) {
        if (mode == 1) {
            if(uart.readable()){
                char recv[1];
                uart.read(recv, sizeof(recv));
                //pc.write(recv, sizeof(recv));
                b = atoi(recv);
                sprintf(buffer, "%d", b);
                xbee.write(buffer, sizeof(buffer));
                if (b == 1) {
                    car.goStraight(50);
                    ThisThread::sleep_for(1s);
                    car.stop();
                    ThisThread::sleep_for(500ms);
                } else if (b == 0) {
                    car.stop();
                    sprintf(buffer, "%d", b);
                    xbee.write(buffer, sizeof(buffer));
                }
            }
        } 
        /*else if (mode == 2) {
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
        }*/
    }
}

int main() {

   //uart.set_baud(9600);
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
   //xbee.write("1", 1);
   }
}

void modeselect(Arguments *in, Reply *out){
    int x = in->getArg<int>();
    mode = x;
    char mode1[200];
    //mode1 = mode;
    //uart.write(("%d", mode).deocde());
    sprintf(mode1, "%d", mode);
    //xbee.write("1", 1);
    xbee.write(mode1, sizeof(mode1));
}