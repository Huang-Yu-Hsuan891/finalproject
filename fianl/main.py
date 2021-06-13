import pyb, sensor, image, time, math
enable_lens_corr = False # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.RGB565) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
clock = time.clock()
tag_families= 0
tag_families|= image.TAG36H11

f_x = (2.8 / 3.984) * 160 # find_apriltags defaults to this if not set
f_y = (2.8 / 2.952) * 120 # find_apriltags defaults to this if not set
c_x = 160 * 0.5 # find_apriltags defaults to this if not set (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags defaults to this if not set (the image.h * 0.5)

def degrees(radians):
   return (180 * radians) / math.pi
uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

i = 0
mode = 1
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
               #else:
                #   i += 1
   print("%d"% i)
   if detectline1 == 0:
       i += 1
   print("/linedetection/run %d %d\r\n" % detectline)
   uart.write(("/linedetection/run %d %d \r\n" % detectline).encode())
   time.sleep(1)
   if i > 10:
       break

uart.write(("/turn/run 80 -0.3 \r\n").encode())
time.sleep(1)
uart.write(("/turn/run 80 -0.3 \r\n").encode())
time.sleep(1)
uart.write(("/stop/run \r\n").encode())
time.sleep(1)

mode = 2
print_args = 0.0
j = 0
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

uart.write(("/stop/run \r\n").encode())
time.sleep(1)


