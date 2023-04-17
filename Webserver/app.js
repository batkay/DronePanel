const http = require('http');
const fs = require('fs');


// const hostname = '172.20.10.5';
// const port = 3000;
const hostname = '127.0.0.1';
const port = 3000;

const server = http.createServer((req, res) => {
  if (req.url == '/') {
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/plain');
    res.end('Hello World');
  }
  

  if (req.url == '/data') { //check the URL of the current request
    image = fs.open("mushroom.png");
    console.log(image);
    
    
    // let arr = [];
    // for (let i = 0; i < 3; i += 1) {
    //   arr[i] = Array.apply(null, {length: 8*32}).map(Function.call, Math.random);
    // }
    let arr = Array.apply(null, {length: 8*32}).map(Function.call, Math.random);
    for (let i = 0; i < 8*32; i+=1) {
      arr[i] = color(50, 0, 0);
    }
    // console.log(color(50, 0, 0));
    // let color =((uint32_t)r << 16) | ((uint32_t)g << 8) | b
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.write(JSON.stringify({ colors: arr}));
    res.end();  
  }
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});

function color (r, g, b) {
  return (r << 16) | (g << 8) | b;
}

// var ArduinoIotClient = require('@arduino/arduino-iot-client');

// var api = new ArduinoIotClient.DashboardsV2Api()
// var id = "6fcd53d4-c01e-4eae-8f1d-4b9d32827342"; // {String} The id of the dashboard
// var dashboardv2 = Dashboardv2; // {Dashboardv2} 
// var opts = {
//   'xOrganization': xOrganization_example // {String} 
// };

// let data = "hello world";
// api.dashboardsV2Update(id, dashboardv2, opts).then(function(data) {
//   console.log('API called successfully. Returned data: ' + data);
// }, function(error) {
//   console.error(error);
// });