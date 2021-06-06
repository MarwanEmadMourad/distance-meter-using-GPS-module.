// This example creates a 2-pixel-wide red polyline showing the path of
// the first trans-Pacific flight between Oakland, CA, and Brisbane,
// Australia which was made by Charles Kingsford Smith.
let { coordinates, distance } = {
    distance: 160.202,
    coordinates: [
      { lat: 30.064710141381084, lng: 31.280019586660455 },
      { lat: 30.064436222295758, lng: 31.28012151059636 },
      { lat: 30.06428765569513, lng: 31.27986938296543 },
      { lat: 30.06327089703697, lng: 31.280368273809607 },
    ],
  };
  
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
  }
  