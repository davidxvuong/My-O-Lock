import serial, time
arduino = serial.Serial('/dev/cu.usbmodem1413', 9600, timeout=.1)
time.sleep(1)
arduino.write("Hello from python!")
