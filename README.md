# finalproject
# 我的主要main.cpp檔在fianl資料夾內
# 還有包括我寫open mv 的內容main.py檔也在fianl資料夾內

# 主要我是從open mv write 一個rpc function到mbed做指令告訴bbcar接下來做甚麼事情，而xbee 負責看現在呼叫了些甚麼rpc進來到bbcar
# 因此我會先讓bbcar沿著一條線走，當它沒有偵測到直線10次，就會break掉當下偵測line的狀態，進入到偵測apriltag，當它對著apriltag有超過10次以上的值，也會就會break掉當下偵測line的狀態而停下來，也就完成了整個bbcar的規劃
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
# 在這裡主要接收從uart來的字串，然後依照從openmv來的內容呼叫rpc function，最後再把所呼叫的rpc function write到xbee顯示出值，可以順便檢查傳出來和實際上是否相符合
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
# 這是一個偵測apriltag的rpc function依據傳回來的值做車車角度的調整

# 以下是open mv的內容
      time.sleep(5)
      while(True):
      clock.tick()
      img = sensor.snapshot()
      if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...
      detectline = (0, mode)
      detectline1 = 0
      for l in img.find_line_segments(merge_distance = 0, max_theta_diff = 5):
            if l.magnitude() > 10:
                  if l.y2() < 50 and abs(l.y2() - l.y1() > 3):
                        if l.x1() > 30 and l.x2() < 90 and abs(l.x2() - l.x1() < 3):
                              img.draw_line(l.line(), color = (255, 0, 0))
                              print_args = (l.x1(),l.y1(),l.x2(),l.y2(),l.length())
                              detectline = (1, mode)
                              detectline1 = 1
      if detectline1 == 0:
            i += 1
      uart.write(("/linedetection/run %d %d \r\n" % detectline).encode())
      time.sleep(1)
      if i > 10:
            break
 # 一開始我會先停5秒，為了開好xbee.python code做準備，接著開始依據線去做限制，讓畫面中線變得較少一點，最後輸出rpc function到mbed上
 
      while(True):

    clock.tick()
    img = sensor.snapshot()
    h = (0, mode)
    hcal = 0
    #time.sleep_ms(500)
    for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # defaults to TAG36H11
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        print_args = tag.x_translation()
        if print_args >= 1.5:
            h = (1, mode) # turn left
            hcal = 1
        elif print_args <= -1.5:
            h = (2, mode) #turn right
            hcal = 2
        else:
            h = (3, mode)
            hcal = 0
    if hcal == 0:
        j += 1
    uart.write(("/aprildetection/run %d %d \r\n" % h).encode())
    print("%d %d" % h)
    time.sleep(1)
    if j > 10:
        h = (0,3)
        break
      print("%d %d" % h)
      uart.write(("/aprildetection/run %d %d \r\n" % h).encode())
  # 接著這邊就是針對apriltag的內容 用x方向上的位移量去做判斷，看試向右偏移多少，或是向左偏移多少，也是在最後輸出rpc function到mbed上
  # 以上就是我大約的內容
  

