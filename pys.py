import serial
import json

ser = serial.Serial("/dev/ttyUSB0", 9600)
points = []
point = {"lng": "", "lat": ""}

while True:
    if ser.in_waiting:
        s = ser.readline().decode()
        print(s)
        if s == "q":
            break
        if point["lng"] == "":
            point["lng"] = s
        elif point["lng"] != "" and point["lat"] == "":
            point["lat"] = s
            points.append(point)
            point = {"lng": "", "lat": ""}

while True:
    if ser.in_waiting:
        s = ser.readline().decode()
        obj = {"distance": s, "coordinates": points}
        break

with open("file.json", "w") as f:
    json.dump(obj, f)

