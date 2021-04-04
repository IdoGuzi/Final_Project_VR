from serial import Serial
import numpy as np

def main():
    ser = Serial('/dev/ttyUSB0',115200)
    ser.readline()
    ser.readline()
    ser.readline()
    ser.readline()

    while 1:
        # Wait until there is data waiting in the serial buffer
        if ser.in_waiting > 0:

            # Read data out of the buffer until a carraige return / new line is found
            yprBytes = ser.readline()
            data = yprBytes.decode("utf-8")
            index = len(data)
            i=0
            ypr = [0.0,0.0,0.0]
            while index != -1:
                index = data.find(',')
                ypr[i] = float(data[:index])
                data = data[index+1:]
                i=i+1
            print(ypr)

if __name__ == "__main__":
    main()