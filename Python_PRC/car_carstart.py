import serial
import time
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

#s.write("/parkingcar/run 10 20 0 \n".encode())
#print("ok")
#time.sleep(15)
#while True:
#    char = s.read(200)
#    print(char.decode())
s.write("/carrunning/run 1 \r\n".encode())
#s.write("/modeselect/run 1 \r\n".encode())
print("ok")
#char = s.read(18)
#print(char.decode())

#print(char1.decode())
#print(type(char1))
#print(type(char1.decode()))
#if char1.decode() == '1':
#   print("yes")
#else:
#    print("no")
#time.sleep(5)
#char2 = s.read(1)
#print(char2.decode())
#print(type(char2))
#print(type(char2.decode()))
#if char2.decode() == '1':
#    print("yes")
#else:
#    print("no")
#i = 0
#while True:
#    char3 = s.read(2)
 #   print(char3.decode())
  #  if char3.decode() == '0':
   #     i += 1
    #    if i == 2:
     #       s.write("/modeselect/run 2 \r\n".encode()) 
      #      print("ok")
            #break
    #time.sleep(1)
time.sleep(10)
#s.write("/carcircling/run 2 \r\n".encode())
#print("ok")  
#char1 = s.read(19)
#print(char1.decode())
#time.sleep(3)
#s.write("/carcircling/run 2 \r\n".encode())
#print("ok")
#time.sleep(3)

s.write("/carrunning/run 3 \r\n".encode())
print("ok")
time.sleep(10)
#time.sleep(15)

s.close()