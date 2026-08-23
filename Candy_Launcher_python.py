import psutil
import time
import requests
import tkinter as tk
from threading import Thread
import cv2
import numpy as np

seen = set()
i = 1

Servo1 = [25.82, -64.80, -17.28]
Servo2 = [2.06, -64.80, 31.00]
Servo3 = [-27.88, -64.80, -13.72]
Plane_normal = [0.0, 1.0, 0.0]
Distance = 1200
focal_length = 543
image_center_x = 320
image_center_y = 240
SCREEN_CENTER_X = 640 // 2
ESP32_IP = "192.168.0.120"  
CAPTURE_URL = "http://192.168.0.120/capture"
COMMAND_URL = "http://192.168.0.120/send_command"

face_classifier = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")
mouth_classifier = cv2.CascadeClassifier("haarcascade_mcs_mouth.xml")

def IK(pitch_deg, roll_deg):
    """
    Step A:
    Calculate the positions of the three platform attachment points
    after rotating the platform from its reference position.

    Reference position:
        All servos = 165 degrees
        Platform is parallel to the wall
        Platform is 50 mm in front of the servo plane

    pitch_deg = rotation around X axis
    roll_deg  = rotation around Z axis

    Returns:
        P1, P2, P3
    """

    # Distance from the servo plane to the platform plane
    platform_distance = 50.0

    # Servo plane is at Y = -64.80.
    # Positive Y is assumed to point from the wall toward the platform.
    platform_y = Servo1[1] + platform_distance

    # Convert degrees to radians
    pitch = np.radians(pitch_deg)
    roll = np.radians(roll_deg)

    # Platform attachment points in the reference position.
    # At the reference position they have the same X and Z
    # coordinates as their corresponding servo.
    P1_reference = np.array(
        [Servo1[0], platform_y, Servo1[2]],
        dtype=float
    )

    P2_reference = np.array(
        [Servo2[0], platform_y, Servo2[2]],
        dtype=float
    )

    P3_reference = np.array(
        [Servo3[0], platform_y, Servo3[2]],
        dtype=float
    )

    # Find the center of the platform.
    # The platform rotates around this point.
    platform_center = (
        P1_reference + P2_reference + P3_reference
    ) / 3.0

    # Rotation around X axis = pitch
    Rx = np.array([
        [1, 0, 0],
        [0, np.cos(pitch), -np.sin(pitch)],
        [0, np.sin(pitch),  np.cos(pitch)]
    ])

    # Rotation around Z axis = roll
    Rz = np.array([
        [np.cos(roll), -np.sin(roll), 0],
        [np.sin(roll),  np.cos(roll), 0],
        [0,             0,            1]
    ])

    # Apply roll first, then pitch
    R = Rx @ Rz

    # Rotate each point around the platform center
    P1 = platform_center + R @ (P1_reference - platform_center)
    P2 = platform_center + R @ (P2_reference - platform_center)
    P3 = platform_center + R @ (P3_reference - platform_center)
    horn_length = 20
    rod_length = 30
    servo_reference = 165

    distance_1 = np.linalg.norm(P1 - Servo1)
    cos_1 = (rod_length**2 + distance_1**2 - horn_length**2) / (2 * rod_length * distance_1)
    cos_1 = np.clip(cos_1, -1.0, 1.0)
    servo_1_angle = servo_reference - np.degrees(np.arccos(cos_1))

    distance_2 = np.linalg.norm(P2 - Servo2)
    cos_2 = (rod_length**2 + distance_2**2 - horn_length**2) / (2 * rod_length * distance_2)
    cos_2 = np.clip(cos_2, -1.0, 1.0)
    servo_2_angle = servo_reference - np.degrees(np.arccos(cos_2))

    distance_3 = np.linalg.norm(P3 - Servo3)
    cos_3 = (rod_length**2 + distance_3**2 - horn_length**2) / (2 * rod_length * distance_3)
    cos_3 = np.clip(cos_3, -1.0, 1.0)
    servo_3_angle = servo_reference - np.degrees(np.arccos(cos_3))

    print(f"Servo angles: {servo_1_angle:.2f}, {servo_2_angle:.2f}, {servo_3_angle:.2f}")
    return servo_1_angle, servo_2_angle, servo_3_angle

def popup():
    root = tk.Tk()
    root.title("OMEN Triggered")
    root.geometry("300x100")
    tk.Label(root, text="ESP trigger fired").pack()
    root.mainloop()

def sent_command(command):
    response = requests.post(COMMAND_URL, data=command, timeout=2)
    print(f"Sent command: [{command}] -> ESP32 Replied: [{response.text}]")
    return response.text.strip()

def capture_image():
    response = requests.get(CAPTURE_URL, timeout=3)
    if response.status_code == 200:
        frame = cv2.imdecode(np.frombuffer(response.content, dtype=np.uint8), cv2.IMREAD_COLOR)
        return frame
    else:
        print(f"Server error fetching image. Status code: {response.status_code}")
        return None

    

while True:
    for p in psutil.process_iter(['pid', 'name']):
        name = (p.info['name'] or "").lower() 
        pid = p.info['pid'] 

        if "hp.omen.omencommandcenter" in name:
            if pid not in seen:
                seen.add(pid)

                try:
                    psutil.Process(pid).terminate()
                except:
                    pass

                result = sent_command("FIRE")
                if result == "FIRE_OK":
                    while True:
                        

                        frame = capture_image()

                        if frame is not None:

                            gray_image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

                            face = face_classifier.detectMultiScale(gray_image, scaleFactor=1.1, minNeighbors=5, minSize=(40, 40))

                            if face is not None and len(face) > 0:
                                
                                closest_face = min(face, key=lambda b: abs((b[0] + (b[2] // 2)) - SCREEN_CENTER_X))
    
                                face_x, face_y, face_w, face_h = closest_face

                                face_cropped_gray = gray_image[face_y:face_y+face_h, face_x:face_x+face_w]
                                lower_face = face_cropped_gray[face_h // 2:, :]

                                mouth = mouth_classifier.detectMultiScale(lower_face, scaleFactor=1.1, minNeighbors=15, minSize=(20, 20))

                                if len(mouth) > 0:
                                    #lowest mouth
                                    mouth_box = max(mouth, key=lambda b: b[1])

                                     
                                    mouth_x, mouth_y, mouth_w, mouth_h = mouth_box
                                    mouth_y += face_h // 2

                                    target_x = face_x + mouth_x + (mouth_w // 2)
                                    target_y = face_y + mouth_y + (mouth_h // 2)

                                    print(f"Mouth detected at absolute coordinates: ({target_x}, {target_y})")

                                    cv2.rectangle(gray_image,(face_x + mouth_x, face_y + mouth_y),(face_x + mouth_x + mouth_w, face_y + mouth_y + mouth_h),(0, 255, 0),2)

                                    cv2.circle(gray_image, (target_x, target_y), 4, (255, 255, 255), -1)

                                    cv2.imshow("Target Spotted", gray_image)
                                    cv2.waitKey(1)  

                                    gain = 0.025
                                    error_x = gain * (target_x - image_center_x)
                                    error_y = gain * (target_y - image_center_y)
                                    IK_angles = IK(error_y, error_x)
                                    if all(135 <= angle <= 165 for angle in IK_angles):
                                        set_servos = sent_command(f"SET_SERVO_ANGLES {IK_angles[0]:.2f} {IK_angles[1]:.2f} {IK_angles[2]:.2f}")
                                        if set_servos == "SERVO_ANGLES_SET":
                                            print(f"Servo angles set to: {IK_angles[0]:.2f}, {IK_angles[1]:.2f}, {IK_angles[2]:.2f}")
                                            time.sleep(0.1)


                                else:
                                    print("Mouth not detected within the face.")


                                
                            else:
                                print("Face detection failed.")

                            
                        else:
                            print("Failed to capture image from ESP32.")
                            break

                else:
                    print(f"Failed to send FIRE command. ESP32 replied: {result}")

                


    seen = {x for x in seen if psutil.pid_exists(x)}

    time.sleep(0.2)
