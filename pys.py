import serial

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

with open("index.js", "w") as f:
    f.write("""let { coordinates, distance } = """+str(obj)+""";
function initMap() {
    let i = 0;
    let j = 1;
    let mesuredDis = 0;
    const flightPlanCoordinates = [];
    flightPlanCoordinates.push(coordinates[0]);
    while (j != coordinates.length) {
      mesuredDis =
        mesuredDis +
        google.maps.geometry.spherical.computeDistanceBetween(
          new google.maps.LatLng(coordinates[i].lat, coordinates[i].lng),
          new google.maps.LatLng(coordinates[j].lat, coordinates[j].lng)
        );
      console.log();
  
      flightPlanCoordinates.push(coordinates[j]);
      console.log("flight", flightPlanCoordinates);
      console.log("cooerdinates", coordinates);
      console.log("distance", distance);
      i++;
      j++;
    }
  
    const map = new google.maps.Map(document.getElementById("map"), {
      zoom: 18,
      center: coordinates[Math.round(coordinates.length / 2)],
      mapTypeId: "terrain",
    });
    const marker1 = new google.maps.Marker({
      position: coordinates[0],
      map: map,
    });
    const marker2 = new google.maps.Marker({
      position: coordinates[coordinates.length - 1],
      map: map,
    });
  
    const flightPath = new google.maps.Polyline({
      path: flightPlanCoordinates,
      geodesic: true,
      strokeColor: "#FF0000",
      strokeOpacity: 1.0,
      strokeWeight: 2,
    });
  
    document.getElementById("tiva").innerHTML = " " + distance.toFixed(2) + " m";
  
    document.getElementById("maps").innerHTML =
      " " + mesuredDis.toFixed(2) + " m";
  
    document.getElementById("error").innerHTML =
      " " +
      (
        ((distance.toFixed(2) - mesuredDis.toFixed(2)) / distance.toFixed(2)) *
        100
      ).toFixed(2) +
      " %";
    flightPath.setMap(map);
}""")
