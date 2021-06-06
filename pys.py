import serial
import json

ser = serial.Serial("COM5", 115200)
points = []
point = {"lng": "", "lat": ""}

def deg_min_to_deg(deg_min_string):
    sep_idx = deg_min_string.find(".") - 2
    return float(deg_min_string[:sep_idx]) + (float(deg_min_string[sep_idx:]) / 60.)


while True:
    if ser.in_waiting:
        s = ser.readline().decode().strip()
        print(s)
        if s == "end":
            break
        if point["lat"] == "":
            point["lat"] = deg_min_to_deg(s)
        elif point["lat"] != "" and point["lng"] == "":
            point["lng"] = deg_min_to_deg(s)
            points.append(point)
            point = {"lng": "", "lat": ""}

while True:
    if ser.in_waiting:
        s = ser.readline().decode()
        obj = {"distance": s, "coordinates": points}
        break

with open("file.json", "w") as f:
    json.dump(obj, f)

