# Autonomous Person Tracking Car

Welcome to the Autonomous Person Tracking Car project! This repository contains the code and documentation for a smart cart system designed to autonomously track and follow individuals. The system is particularly useful in environments such as smart airports and smart supermarkets, where it can enhance customer convenience and improve operational efficiency.

## Table of Contents
- [Overview](#overview)
- [Speed](#speed)
- [Requirements](#requirements)
- [Build and Run](#build-and-run)
- [Installation](#installation)
- [References](#references)
- [License](#license)
- [Contributing](#contributing)

## Overview
The Autonomous Person Tracking Car leverages YOLOv5 for object detection and DeepSORT for object tracking, implemented on an NVIDIA Jetson Nano. This combination allows the cart to accurately identify and follow a designated person in real-time.

## Speed
The entire process time, from reading an image to completing DeepSORT, includes every step of image preprocessing and postprocessing. For this application, we focus on scenarios where only one individual is being tracked, with a maximum of five people in the frame. All results are processed and obtained on the Jetson Nano.
| Backbone        | before TensorRT without tracking |before TensorRT with tracking |TensortRT(detection)| TensorRT(detection + tracking) | FPS(detection + tracking) |
| :-------------- | --------------- | ------------------ |--------------|------------------------------ | ------------------------- |
| Yolov5s_416      | 100ms           | 0.9s|10-15ms|100-150ms                          | 8 ~ 9                   |
| Yolov5s-640 | 120ms             | 1s|18-20ms|100-150ms                      | 8 ~ 9                     |

------

## Requirements

### Hardware Requirements
- NVIDIA Jetson Nano
- Camera module compatible with Jetson Nano
- Motor driver and motors for movement
- Battery pack for power supply
- Chassis to house the components

### Software Requirements
- Ubuntu 18.04 (Jetson Nano default OS)
- Python 3.6+
- PyTorch
- OpenCV

## Build and Run

```shell
git clone https://github.com/quandang246/Autonomous_Person_Tracking_Car.git
cd Autonomous_Person_Tracking_Car
// before you cmake and make, please change ./src/main.cpp char* yolo_engine = ""; char* sort_engine = ""; to your own path
mkdir build 
cmake ..
make 
```

## Installation

### Jetson Nano Setup
1. **Flash the Jetson Nano SD card image**:
   Download the latest JetPack from NVIDIA's [official website](https://developer.nvidia.com/embedded/jetpack) and flash it to your SD card.

2. **Setup the environment**:
   Connect your Jetson Nano to a display, keyboard, and mouse. Boot it up and follow the on-screen instructions to set up the OS.

3. **Install dependencies**:
   Open a terminal and execute the following commands:
   ```bash
   sudo apt-get update
   sudo apt-get upgrade
   sudo apt-get install python3-pip python3-opencv libopencv-dev
   pip3 install torch torchvision
   ```

### Initialize the submodule

```shell
git submodule update --init --recursive    
```
### Model   
Two models are required for this application: a YOLOv5 model for detection, and a DeepSORT model for tracking. Both models should be generated using TensorRT for optimal performance on the Jetson Nano.
### Generate yolov5 engine file 
1. **Get yolov5 weight**:

Note that: here uses the official pertained model (yolov5-5, v5.0). In case you train your own model, please be sure your yolov5 code is v5.0.

```shell
cd thirdparty/yolov5
mkdir weights
cd weights
// download https://github.com/ultralytics/yolov5/releases/download/v5.0/yolov5s.pt
wget https://github.com/ultralytics/yolov5/releases/download/v5.0/yolov5s.pt

```

2. **Get tensorrtx and generate .wts from pytorch with .pt**:

```shell
// copy gen_wts.py from tensorrtx to yolov5
cp thirdparty/tensorrtx/yolov5/gen_wts.py thirdparty/yolov5
cd thirdparty/yolov5
python3 gen_wts.py -w ./weights/yolov5s.pt -o ./weights/yolov5s.wts
// a file 'yolov5s.wts' will be generated in thirdparty/yolov5/weights folder
```

3. **Build tensorrtx/yolov5 and get tensorrt engine**:

```shell 
cd thirdparty/tensorrtx/yolov5
// update CLASS_NUM in yololayer.h if your model is trained on custom dataset
mkdir build
cd build
// copy 'yolov5s.wts' to thirdparty/tensorrtx/yolov5/build
cp thirdparty/yolov5/yolov5s.wts thirdparty/tensorrtx/yolov5/build
cmake ..
make
// yolov5s
sudo ./yolov5 -s yolov5s.wts yolov5s.engine s
// test your engine file
sudo ./yolov5 -d yolov5s.engine ../samples
```
You can see you dection results in samples folder!

### Get deepsort engine file
1. **Move 'ckpt.t7' file**:
```shell 
// copy 'ckpt.t7' to thirdparty/deep_sort_pytorch
cp assets/ckpt.t7 thirdparty/deep_sort_pytorch
```

2. **Convert the PyTorch model to ONNX format**:

```shell
// copy exportOnnx.py from thirdparty/deep-tensorrt to thirdparty/deep_sort_pytorch
cp thirdparty/deepsort-tensorrt/exportOnnx.py thirdparty/deep_sort_pytorch/
python3 deep_sort_pytorch/exportOnnx.py
// a file 'deepsort.onnx' will be generated in thirdparty/deep_sort_pytorch/ folder
// Move the generated deepsort.onnx file to the resources directory inside deepsort-tensorrt
mv thirdparty/deep_sort_pytorch/deepsort.onnx thirdparty/deepsort-tensorrt/resources/
```

3. **Convert the ONNX model to TensorRT engine format**:

```shell
cd thirdparty/deepsort-tensorrt
mkdir build
cd build
cmake ..
make
./onnx2engine ../resources/deepsort.onnx ../resources/deepsort.engine
// Test the generated engine file
./demo ../resources/deepsort.engine ../resources/track.txt
```

### Put yolov5s.engine and deepsort.engine in My project
After 2 sections above , you can get the yolov5s.engine and deepsort.engine.
```shell
// copy yolov5 engine into my project
cd {Autonomous_Person_Tracking_Car}
mkdir resources
cp thirdparty/tensorrtx/yolov5/build/yolov5s.engine {Autonomous_Person_Tracking_Car}/resources
cp thirdparty/deepsort-tensorrt/resources/deepsort.engine {Autonomous_Person_Tracking_Car}/resources
```

## References

This project incorporates the following submodules:

1. **JetsonGPIO**: A submodule facilitating GPIO pin access specifically tailored for the NVIDIA Jetson platform. [GitHub Repository](https://github.com/pjueon/JetsonGPIO.git)

2. **deepsort-tensorrt**: Implements the DeepSORT algorithm using TensorRT for efficient object tracking, ensuring real-time performance. [GitHub Repository](https://github.com/RichardoMrMu/deepsort-tensorrt.git)

3. **deep_sort_pytorch**: Offers a PyTorch-based implementation of the DeepSORT algorithm, providing flexibility and ease of use. [GitHub Repository](https://github.com/ZQPei/deep_sort_pytorch.git)

4. **yolov5**: Incorporates the YOLOv5 model for object detection, known for its high accuracy and speed. [GitHub Repository](https://github.com/ultralytics/yolov5.git) (branch: v5.0)

5. **tensorrtx**: Utilizes TensorRT to optimize YOLOv5 models for deployment on edge devices, ensuring efficient inference. [GitHub Repository](https://github.com/wang-xinyu/tensorrtx.git) (branch: yolov5-v5.0)

These submodules provide essential functionalities and models for the project. For detailed documentation and usage instructions, refer to their respective repositories.


## License

This project is licensed under the [GNU GENERAL PUBLIC LICENSE](LICENSE). See the LICENSE file for more details.

## Contributing

We welcome contributions to improve this project! Please fork the repository, create a feature branch, and submit a pull request with your changes.

