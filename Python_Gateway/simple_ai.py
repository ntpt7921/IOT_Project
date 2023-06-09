from keras.models import load_model  # TensorFlow is required for Keras to work
import cv2  # Install opencv-python
import numpy as np

# Disable scientific notation for clarity
np.set_printoptions(suppress=True)

# Load the model
model = load_model("./keras_model.h5", compile=False)

# Load the labels
#class_names = open("labels.txt", "r").readlines()
label = [line.replace('\n', '').split(sep=' ', maxsplit=1)[1] 
    for line in open("./labels.txt").readlines()]

# CAMERA can be 0 or 1 based on default camera of your computer
camera = cv2.VideoCapture(0)

def image_deterctor():
    # Grab the webcamera's image.
    _, image = camera.read()

    # Resize the raw image into (224-height,224-width) pixels
    image = cv2.resize(image, (224, 224), interpolation=cv2.INTER_AREA)

    # Show the image in a window
    # cv2.imshow("Webcam Image", image)

    # Make the image a numpy array and reshape it to the models input shape.
    image = np.asarray(image, dtype=np.float32).reshape(1, 224, 224, 3)

    # Normalize the image array
    image = (image / 127.5) - 1

    # Predicts the model
    prediction = model.predict(image)
    index = np.argmax(prediction)
    if (index in range(len(label))):
        class_name = label[index]
        confidence_score = prediction[0][index]
        # Print prediction and confidence score
        print("Class:", class_name)
        print("Confidence Score:", str(np.round(confidence_score * 100))[:-2], "%")
        return class_name
    else:
        return "";
