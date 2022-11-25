import requests
import serial
import cv2 



url = 'http://connect-letitgrow.com/API/addData.php'
urlImage = 'http://connect-letitgrow.com/API/addImage.php'
headers = {'Content-Type': 'application/json'}

GardenID = 1

# files = {'media': open('test.jpg', 'rb')}
# requests.post(url, files=files)

webcam = cv2.VideoCapture(0)

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.reset_input_buffer()

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            values = line.split(" ")
            
            print(values)
            
            tds = int(values[0])
            pH = float(values[1])
            waterLevel = int(values[2])
            waterTemp = float(values[3])
            humidity = float(values[4])
            airTemp = float(values[5])
            
            body = """{
              "GardenID": %d,
              "tds": %d,
              "pH": %f,
              "waterLevel": %d,
              "waterTemp": %f,
              "humidity": %f,
              "airTemp": %f
            }""" % (GardenID, tds, pH, waterLevel, waterTemp, humidity, airTemp)
            
            check, frame = webcam.read()
            print(check) #prints true as long as the webcam is running
            print(frame) #prints matrix values of each framecd 
            cv2.imwrite(filename='saved_img.jpg', img=frame)
    
            files = {'media': open('saved_img.jpg', 'rb')}
            requests.post(urlImage, files=files)

            req = requests.post(url, headers=headers, data=body)
            
            print(req.status_code)
            print(req.headers)
            print(req.text)
