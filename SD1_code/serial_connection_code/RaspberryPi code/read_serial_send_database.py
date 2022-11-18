import requests
import serial

url = 'http://connect-letitgrow.com/API/addData.php'
headers = {'Content-Type': 'application/json'}

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.reset_input_buffer()

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            values = line.split(" ")

            body = {
              "GardenID": 1,
              "tds": int(values[0]),
              "pH": float(values[1]),
              "waterLevel": int(values[2]),
              "waterTemp": float(values[3]),
              "humidity": float(values[4]),
              "airTemp": float(values[5])
            }
            
            req = requests.post(url, headers=headers, data=body)
            
            print(req.status_code)
            print(req.headers)
            print(req.text)