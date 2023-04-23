// enabling drawing on the blank canvas

let girth = 25;

let size = 1;

const colorElement = document.getElementsByName("colorRadio");

let color;
colorElement.forEach((c) => {
  if (c.checked) color = c.value;
});
colorElement.forEach((c) => {
  c.onclick = () => {
    color = c.value;
  };
});
drawOnImage();

function drawOnImage(image = null) {
    const canvasElement = document.getElementById("canvas");

  const context = canvasElement.getContext("2d");
  // context.scale(10, 10);

  // if an image is present,
  // the image passed as parameter is drawn in the canvas
  if (image) {
    const imageWidth = image.width;
    const imageHeight = image.height;

    // rescaling the canvas element
    canvasElement.width = imageWidth;
    canvasElement.height = imageHeight;

    context.drawImage(image, 0, 0, imageWidth, imageHeight);
  }

  const clearElement = document.getElementById("clear");
  clearElement.onclick = () => {
    context.clearRect(0, 0, canvasElement.width, canvasElement.height);
  };

  let isDrawing;
  
  canvasElement.onmousedown = (e) => {
    isDrawing = true;
    context.beginPath();
    context.lineWidth = girth;
    context.strokeStyle = color;
    context.lineJoin = "round";
    context.lineCap = "round";
    context.moveTo(
      Math.floor(e.clientX / girth) * girth + girth / 2,
      Math.floor(e.clientY / girth) * girth + girth / 2
    );
  };

  canvasElement.onmousemove = (e) => {
    if (isDrawing) {
      context.lineTo(
        Math.floor(e.clientX / girth) * girth + girth / 2,
        Math.floor(e.clientY / girth) * girth + girth / 2
      );
      context.stroke();
    }
  };

  canvasElement.onmouseup = function () {
    isDrawing = false;
    context.closePath();
  };

  const send = document.getElementById("send");
  send.onclick = () => {
    var dataURL = canvasElement.toDataURL();
    $.ajax({
      type: "POST",
      url: "/hook",
      data: {
        imageBase64: dataURL,
      },
    }).done(function () {
      console.log("sent");
    });
  }

  
}
