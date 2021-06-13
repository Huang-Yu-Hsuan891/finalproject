# finalproject
# 我的主要main.cpp檔在fianl資料夾內
# 還有包括我寫open mv 的內容main.py檔也在fianl資料夾內

# 主要我是從open mv write 一個rpc function到mbed做指令告訴bbcar接下來做甚麼事情，而xbee 負責看現在呼叫了些甚麼rpc進來到bbcar
# 接著先來介紹main.cpp
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
# 在這裡主要接收從uart來的字串，然後依照從openmv來的內容呼叫rpc function
      void linedetection(Arguments *in, Reply *out){
      int x = in->getArg<int>();
      int y = in->getArg<int>();
        if(x ==1) {
        car.goStraight(50);
        ThisThread::sleep_for(600ms);
        car.stop();
        ThisThread::sleep_for(400ms);
        out->putData(x);
        }
      }
# 這是一個rpc function 偵測到一條直線就會傳訊息回來 tell bbcar go straight
    void aprildetection(Arguments *in, Reply *out){
    int a = in->getArg<int>();
    int y1 = in->getArg<int>();
        if (a == 1) {
            car.turn(40, 0.4);  // turn left
            ThisThread::sleep_for(500ms);
            car.stop();
            ThisThread::sleep_for(500ms);
        } else if (a == 2) {
            car.turn(40, -0.4);  // turn right
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
# 這是一個偵測apriltag的rpc function
