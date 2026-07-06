import psutil
import time
import requests
import tkinter as tk
from threading import Thread
import cv2
import numpy as np

seen = set()
i = 1
SCREEN_CENTER_X = 640 // 2
ESP32_IP = "192.168.0.120"  
CAPTURE_URL = "http://192.168.0.120/capture"
COMMAND_URL = "http://192.168.0.120/send_command"

face_classifier = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")
mouth_classifier = cv2.CascadeClassifier("haarcascade_mcs_mouth.xml")

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

                                    if i==1:
                                        result = sent_command("FLASH_ON")
                                        print(result)
                                        i=0
                                    else:
                                        result = sent_command("FLASH_OFF")
                                        print(result)
    



                                    # inverse kinematics calculations here...

                                else:
                                    print("Mouth not detected within the face. Retrying...")


                                
                            else:
                                print("Face detection failed. Retrying...")

                            
                        else:
                            print("Failed to capture image from ESP32.")
                            break

                else:
                    print(f"Failed to send FIRE command. ESP32 replied: {result}")

                


    seen = {x for x in seen if psutil.pid_exists(x)}

    time.sleep(0.2)