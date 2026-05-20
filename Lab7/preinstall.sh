#!/bin/bash
echo "=========================================="
echo "Встановлення бібліотек для Lab7"
echo "=========================================="

sudo apt update
sudo apt install -y libopencv-dev cmake g++ make wget

mkdir -p models
cd models

echo "Завантаження моделі для детекції облич..."

if [ ! -f deploy.prototxt ]; then
    wget https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
fi

if [ ! -f res10_300x300_ssd_iter_140000.caffemodel ]; then
    wget https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel
fi

cd ..
echo "✅ Встановлення завершено"