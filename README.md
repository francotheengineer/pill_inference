# Pill Scanning and Inference
![Python 3.6](https://img.shields.io/badge/python-3.6-green.svg)

This repo is designed to outline the engineering steps used to make a device that captures high quality images, and passes images to the cloud/localhost for inference. 

```Mainly, this project serves me as part of my portfolio for rapid minimal viable product (MVP) development.```

The mission statement is such:

>The aim is to eradicate the serious health
risk the elderly face due to dosage and pill
identification errors.
>
>The project provides piece of mind by
removing doubt about the identity of a pill
and managing dosing and hence provide
piece of mind for the user and loved ones.

<p float="middle">
<img  src="https://raw.githubusercontent.com/francotheengineer/pill_inference/master/Docs/shroud_sideview.jpg" width="400" > 
</p>


## Hardware Stack
---

1. Perplex Shrowing, light array and camera

2. USB connection to computer

3. C++ or Python to poll for new images and send to locahost or cloud server for images

4. Custom trained convolutional neural network (CNN) to deliver classification results. This will be running in Tensorflow. So make sure you have it installed to run this project 👀🤓 - the gpu version will be fine for this version. But to scale, TF-Serving on GPU would be a better choice.

---

### Persplex Shrowing and camera
<p float="middle">
 <img src="https://raw.githubusercontent.com/francotheengineer/pill_inference/master/Docs/shroud_sideview_2.jpg" width="400"  title="hardware"> 
<img src="https://raw.githubusercontent.com/francotheengineer/pill_inference/master/Docs/camera.jpg" width="400"  title="hardware"> 
</p>

### Lighting Array and Wiring:

Please see the circuit diagram in the docs dir.
<p float="middle">
<img style="float: right" src="https://raw.githubusercontent.com/francotheengineer/pill_inference/master/Docs/light_wiring.jpg" width="400"  title="hardware"> 
<img style="float: left" src="https://raw.githubusercontent.com/francotheengineer/pill_inference/master/Docs/light_array.jpg" width="400"  title="hardware"> 
</p>

## Software Stack

This project runs on the principal of CNN retraining where the last layer of a CNN is retrained. Unfortunately, I have since lost the original dataset due to dropping a HDD.
Make a quick video of each pill (~1min), from perpendicular, ideally filling the frame with the pill. Then use ffmpeg to slice:

```ffmpeg -i <your video file?> -vf fps=15 <pill name>_%d.jpg```
 
Do this for however many pills you want to train and ideally have 200-ish images per class. Also, each image should be a bit different so change the angle and lighting slightly.

The original results provided at best usable results (50-60% accuracy) as be seen below. The original dataset was made up of images like those seen below for inference. This removes the in-vs-out of sample issue often encountered.  

Some inference results, showing 2 calcium pills left and 1 omega3 pill: 
<p float="middle">
<img  src="https://raw.githubusercontent.com/francotheengineer/pill_inference/master/Docs/calcium_result.jpg" width="200" > 
<img src="https://raw.githubusercontent.com/francotheengineer/pill_inference/master/Docs/omega3_result.JPG" width="200"  > 
</p>


To do re-training follow <a href="https://www.tensorflow.org/hub/tutorials/image_retraining">this</a> Tensorflow tutorial.

### Server
Setting up the web-server is easy and can be done using a flask web server that accepts post request containing a  ```XMLHttpRequest``` element with a 200 return code if a successful inference result has been found.
The general pipeline for this is:

1. Receive POST request to server IP. Sanitize image, assign name and save to file system.
2. Start Tensorflow <a href="https://github.com/tensorflow/tensorflow/raw/master/tensorflow/examples/label_image/label_image.py">label_image.py</a> using os.subprocess and parse the output to select the top classification.
    ```bash
    python label_image.py \
    --graph=/tmp/output_graph.pb --labels=/tmp/output_labels.txt \
    --input_layer=Placeholder \
    --output_layer=final_result \
    --image=$HOME/flower_photos/daisy/21652746_cc379e0eea_m.jpg
    ```
3. Return the top classification result to the user.

Ideally: modify label_image.py to accept arguments like a standard python function. But you MUST start the tensorflow session in the flask server code, and pass the tf.Session() as a object to the label_image.py code. Otherwise, you will hit performance issues. 

### Client

An example of the client can be see in ```main.cpp```, without the post to server code. The easiest way to do this is to run a process separate from the main code that waits for a response and shows it to the user. This code essentially grabs an image from the camera at a certain rate and saves to the file system. Simple and fast. 
It's written in c++ so it should be easy to deploy onto a lightweight arm cpu like a RasberryPI.  



