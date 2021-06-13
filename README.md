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
