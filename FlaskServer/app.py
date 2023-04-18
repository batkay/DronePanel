import os
from flask import Flask, flash, request, redirect, url_for, render_template
import cv2
import numpy as np

app = Flask(__name__)
ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg', 'gif', 'img'}

app.config['UPLOAD_FOLDER'] = '/images'

# global variable to hold the image as a numpy array        
global image
image = None

# function to convert a bgr input to a number that the neopixel panel can use (bgr because that's what opencv uses by default)
def color (b, g, r):
    return (r << 16) | (g << 8) | b

# make sure we are given a file type we can use
def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS
           
# test network
@app.route("/")
def home():
    return "Hello World"

# display the app.js html page on /upload page
@app.route("/upload")
def upload():
    return render_template("page.html")

# given a post request, take the included image file and write it to the global variable
@app.route("/uploader", methods = ["GET", "POST"])
def readFile():
    print('request.method', request.method)
    print('request.args', request.args)
    print('request.form', request.form)
    print('request.files', request.files)
    global image
    if request.method == "POST":
        if 'image' not in request.files:
            return 'there is no image in form!'
        if not allowed_file(request.files['image'].filename):
            return 'not allowed file type'
        file = request.files['image']
        # filestr = request.files['image'].read()
        
        file_bytes = np.fromfile(file, np.uint8)
        image = cv2.imdecode(file_bytes, cv2.IMREAD_COLOR)

        
        # image = cv2.imread(img)
        image = cv2.resize(image, (32, 8), interpolation = cv2.INTER_AREA)
        
        return "success"

@app.route("/image")
def showImage():
    return "image"

# send the image out to the panel. The image is 32x8, and we need to go along the y (8) axis first when displaying on neopixel
# also neopixel uses a snaking pattern
# put all the values in snaking order into a csv with "color"
@app.route("/data")
def sendImage():
    global image
    if (image is None):
        
        return {"colors": [color(0, 0, 0)] * 32 * 8}
    colors = []
    for i in range(0,image.shape[1]):
        if i % 2 == 0:
            for j in range(0,image.shape[0]):
                
                colors.append(int(color(image[j,i,0], image[j,i,1],image[j,i,2])))
        else:
            for j in range(image.shape[0]-1, -1, -1):
                # pixel = image[i, j]
                # print(color(image[i,j,0], image[i,j,1],image[i,j,2]))
                colors.append(int(color(image[j,i,0], image[j,i,1],image[j,i,2])))
        
    return {"colors": colors}

# make it on the ip of my macbook on my hotspot
if __name__ == "__main__":
    # app.run(host="127.0.0.1", port=3000)
    app.run(host="172.20.10.5", port=3000)

    # can alter host and port number here. Right now the default host is localhost and port is 5000
    app.run(debug=True)
    
