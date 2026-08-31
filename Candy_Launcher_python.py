import psutil
import time
import requests
import cv2
import numpy as np


seen = set()

Servo1 = [25.82, -64.80, -17.28]
Servo2 = [2.06, -64.80, 31.00]
Servo3 = [-27.88, -64.80, -13.72]

Plane_normal = [0.0, 1.0, 0.0]

Distance = 1200
focal_length = 543

image_center_x = 320
image_center_y = 240

SCREEN_CENTER_X = 320

ESP32_IP = "192.168.0.120"

CAPTURE_URL = f"http://{ESP32_IP}/capture"
COMMAND_URL = f"http://{ESP32_IP}/send_command"

gain = 0.2

AIM_TOLERANCE_X = 50
AIM_TOLERANCE_Y = 50

STABLE_FRAMES_REQUIRED = 5

SERVO_UPDATE_INTERVAL = 0.2
SERVO_CHANGE_THRESHOLD = 0.5

last_servo_angles = None
last_servo_time = 0
min_servo_angle = 90
max_servo_angle = 165

stable_frames = 0
fire_sent = False

face_classifier = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")

mouth_classifier = cv2.CascadeClassifier("haarcascade_mcs_mouth.xml")


def IK(pitch_deg, roll_deg):

    platform_distance = 50.0

    platform_y = Servo1[1] + platform_distance

    pitch = np.radians(pitch_deg)
    roll = np.radians(roll_deg)

    P1_reference = np.array([Servo1[0], platform_y, Servo1[2]], dtype=float)

    P2_reference = np.array([Servo2[0], platform_y, Servo2[2]], dtype=float)

    P3_reference = np.array([Servo3[0], platform_y, Servo3[2]], dtype=float)

    platform_center = (P1_reference + P2_reference + P3_reference) / 3.0

    Rx = np.array([[1, 0, 0], [0, np.cos(pitch), -np.sin(pitch)], [0, np.sin(pitch), np.cos(pitch)]])

    Rz = np.array([[np.cos(roll), -np.sin(roll), 0], [np.sin(roll), np.cos(roll), 0], [0, 0, 1]])

    R = Rx @ Rz

    P1 = (platform_center + R @ (P1_reference - platform_center))

    P2 = (platform_center + R @ (P2_reference - platform_center))

    P3 = (platform_center + R @ (P3_reference - platform_center))

    horn_length = 20
    rod_length = 30
    servo_reference = 165

    distance_1 = np.linalg.norm(P1 - Servo1)

    cos_1 = (rod_length ** 2 + distance_1 ** 2 - horn_length ** 2) / (2 * rod_length * distance_1)

    cos_1 = np.clip(cos_1, -1.0, 1.0)

    servo_1_angle = np.clip(servo_reference - np.degrees(np.arccos(cos_1)), min_servo_angle, max_servo_angle)

    distance_2 = np.linalg.norm(P2 - Servo2)

    cos_2 = (rod_length ** 2 + distance_2 ** 2 - horn_length ** 2) / (2 * rod_length * distance_2)

    cos_2 = np.clip(cos_2, -1.0, 1.0)

    servo_2_angle = np.clip(servo_reference - np.degrees(np.arccos(cos_2)), min_servo_angle, max_servo_angle)

    distance_3 = np.linalg.norm(P3 - Servo3)

    cos_3 = (rod_length ** 2 + distance_3 ** 2 - horn_length ** 2) / (2 * rod_length * distance_3)

    cos_3 = np.clip(cos_3, -1.0, 1.0)

    servo_3_angle = np.clip(servo_reference - np.degrees(np.arccos(cos_3)), min_servo_angle, max_servo_angle)

    print(f"Servo angles: "f"{servo_1_angle:.2f}, "f"{servo_2_angle:.2f}, "f"{servo_3_angle:.2f}")

    return (servo_1_angle, servo_2_angle, servo_3_angle)


def sent_command(command):

    try:

        response = requests.post(COMMAND_URL, data=command, timeout=4)

        result = response.text.strip()

        print(f"Sent command: [{command}] " f"-> ESP32 Replied: [{result}]")

        return result

    except requests.exceptions.Timeout:
        print("ESP32 command timed out")
        return None

    except requests.exceptions.RequestException as e:
        print(f"ESP32 command error: {e}")
        return None

def capture_image():

    try:

        response = requests.get(CAPTURE_URL, timeout=3, headers={ "Connection": "close"})

        if response.status_code == 200:

            frame = cv2.imdecode(np.frombuffer(response.content, dtype=np.uint8), cv2.IMREAD_COLOR)

            return frame

        print(f"Server error fetching image. "f"Status code: {response.status_code}")

        return None

    except requests.exceptions.Timeout:

        print("Camera request timed out")

        return None

    except requests.exceptions.RequestException as e:

        print(f"Camera request error: {e}")

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

                stable_frames = 0
                fire_sent = False
                last_servo_angles = None
                last_servo_time = 0

                while True:

                    time.sleep(0.05)

                    frame = cv2.flip(capture_image(), -1)

                    if frame is None:
                        continue

                    height, width = frame.shape[:2]

                    image_center_x = width // 2
                    image_center_y = height // 2

                    SCREEN_CENTER_X = image_center_x

                    gray_image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

                    display = frame.copy()


                    face = face_classifier.detectMultiScale(gray_image, scaleFactor=1.1, minNeighbors=5, minSize=(40, 40))

                    if len(face) > 0:

                        closest_face = min(face, key=lambda b: abs((b[0] + b[2] // 2) - SCREEN_CENTER_X))

                        face_x, face_y, face_w, face_h = (closest_face)

                        cv2.rectangle(display, (face_x, face_y), (face_x + face_w, face_y + face_h), (255, 0, 0), 2)

                        face_cropped_gray = gray_image[face_y:face_y + face_h, face_x:face_x + face_w]

                        lower_face = face_cropped_gray[face_h // 2:, :]

                        mouth = mouth_classifier.detectMultiScale(lower_face, scaleFactor=1.1, minNeighbors=15,  minSize=(20, 20))

                        if len(mouth) > 0:

                            mouth_box = max(mouth, key=lambda b: b[1])

                            mouth_x, mouth_y, mouth_w, mouth_h = (mouth_box)

                            mouth_y += face_h // 2

                            target_x = (face_x + mouth_x + mouth_w // 2)

                            target_y = ( face_y + mouth_y + mouth_h // 2)

                            print(f"Mouth detected at "f"({target_x}, {target_y})")

                            cv2.rectangle(display, (face_x + mouth_x, face_y + mouth_y), (face_x + mouth_x + mouth_w, face_y + mouth_y + mouth_h), (0, 255, 0), 2)

                            cv2.circle(display,(target_x, target_y), 4, (255, 255, 255), -1)

                            error_x = (target_x - image_center_x)

                            error_y = (target_y - image_center_y)

                            pitch = gain * error_y
                            roll = gain * error_x

                            IK_angles = IK(pitch, roll)

                            angles_valid = all(min_servo_angle <= angle <= max_servo_angle for angle in IK_angles)

                            if angles_valid:

                                now = time.monotonic()

                                angle_change = True

                                if last_servo_angles is not None:

                                    angle_change = (max(abs(IK_angles[i] - last_servo_angles[i]) for i in range(3)) >= SERVO_CHANGE_THRESHOLD)

                                if (angle_change and now - last_servo_time >= SERVO_UPDATE_INTERVAL):

                                    set_servos = sent_command(f"SET_SERVO_ANGLES "f"{IK_angles[0]:.2f} "f"{IK_angles[1]:.2f} "f"{IK_angles[2]:.2f}")

                                    if set_servos == "SERVO_ANGLES_SET":

                                        last_servo_angles = IK_angles
                                        last_servo_time = now


                                centered = (abs(error_x) <= AIM_TOLERANCE_X and abs(error_y) <= AIM_TOLERANCE_Y)

                                servo_settled = (now - last_servo_time >= 0.45)

                                if (centered and servo_settled):
                                    stable_frames += 1

                                else:
                                    stable_frames = 0

                                if (stable_frames >= STABLE_FRAMES_REQUIRED and not fire_sent):

                                    print("Target centered - FIRE")

                                    result = sent_command("FIRE")

                                    if result == "FIRE_OK":

                                        fire_sent = True

                                        print("Fire command accepted.")

                            else:

                                stable_frames = 0

                        else:

                            stable_frames = 0

                            print("Mouth not detected within the face.")

                    else:

                        stable_frames = 0

                        print("Face detection failed.")

                    cv2.imshow("Camera", display)

                    key = cv2.waitKey(1) & 0xFF

                    if key == ord("q"):

                        cv2.destroyAllWindows()
                        raise SystemExit

                    if fire_sent:

                        time.sleep(1.0)
                        break

    seen = {x for x in seen if psutil.pid_exists(x)}
    time.sleep(0.2)