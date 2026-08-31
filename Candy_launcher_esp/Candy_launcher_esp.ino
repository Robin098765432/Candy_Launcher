#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include <ESP32Servo.h>
#include "board_config.h"

const char* ssid = "Jojanneke en Linda 2";
const char* password = "C00kiemonster!!";

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

const int SERVO1_PIN = 12;
const int SERVO2_PIN = 13;
const int SERVO3_PIN = 14;
const int SERVO4_PIN = 15;

const int IO2_PIN = 2;
const int PWM_VALUE = 255;

float curr_servo1_angle = 165;
float curr_servo2_angle = 165;
float curr_servo3_angle = 165;

float target_servo1_angle = 165;
float target_servo2_angle = 165;
float target_servo3_angle = 165;

bool servo_update_pending = false;

IPAddress local_IP(192, 168, 0, 120);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 0, 0);

httpd_handle_t camera_httpd = NULL;


bool fire_requested = false;


static esp_err_t capture_handler(httpd_req_t *req) {
  camera_fb_t *fb = esp_camera_fb_get();

  if (!fb) {httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/jpeg");
  httpd_resp_set_hdr(req, "Cache-Control", "no-store");
  httpd_resp_set_hdr(req, "Connection", "close");

  esp_err_t res = httpd_resp_send(req, (const char *)fb->buf, fb->len);

  esp_camera_fb_return(fb);

  return res;
}


bool readCommand(httpd_req_t *req, char *buffer, size_t buffer_size) {
  if (req->content_len <= 0) {
    return false;
  }

  if (req->content_len >= buffer_size) {
    return false;
  }

  int ret = httpd_req_recv(req, buffer, buffer_size - 1);

  if (ret <= 0) {
    return false;
  }

  buffer[ret] = '\0';

  return true;
}



static esp_err_t command_handler(httpd_req_t *req) {
  char buffer[128];

  if (!readCommand(req, buffer, sizeof(buffer))) {
    httpd_resp_send(req, "INVALID_COMMAND", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
  }

  String command = String(buffer);
  command.trim();


  if (command == "FIRE") {
    if (fire_requested) {
      httpd_resp_send(req, "FIRE_BUSY", HTTPD_RESP_USE_STRLEN);
      return ESP_OK;
    }

    fire_requested = true;
    httpd_resp_send(req, "FIRE_OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
  }


  if (command == "FLASH_OFF") {

    digitalWrite(LED_GPIO_NUM, LOW);

    httpd_resp_send(req, "LED_OFF", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
  }


  if (command == "FLASH_ON") {

    digitalWrite(LED_GPIO_NUM, HIGH);

    httpd_resp_send(req, "LED_ON", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
  }



  if (command.startsWith("SET_SERVO_ANGLES")) {

    int first_space = command.indexOf(' ');
    int second_space = command.indexOf(' ', first_space + 1);
    int third_space = command.indexOf(' ', second_space + 1);

    if (first_space == -1 || second_space == -1 || third_space == -1) {
      httpd_resp_send( req, "INVALID_SERVO_COMMAND", HTTPD_RESP_USE_STRLEN);

      return ESP_OK;
    }

    float angle1 = command.substring( first_space + 1, second_space).toFloat();

    float angle2 = command.substring( second_space + 1, third_space).toFloat();

    float angle3 = command.substring( third_space + 1).toFloat();

    if (angle1 < 90 || angle1 > 165 || angle2 < 90 || angle2 > 165 || angle3 < 90 || angle3 > 165) {
      httpd_resp_send(req, "INVALID_SERVO_ANGLE", HTTPD_RESP_USE_STRLEN);

      return ESP_OK;
    }

    target_servo1_angle = angle1;
    target_servo2_angle = angle2;
    target_servo3_angle = angle3;

    servo_update_pending = true;

    httpd_resp_send( req, "SERVO_ANGLES_SET", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
  }


  httpd_resp_send( req, "UNKNOWN_COMMAND", HTTPD_RESP_USE_STRLEN);

  return ESP_OK;
}

void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  config.server_port = 80;
  config.max_open_sockets = 4;
  config.lru_purge_enable = true;

  httpd_uri_t capture_uri = {.uri = "/capture", .method = HTTP_GET, .handler = capture_handler, .user_ctx = NULL};

  httpd_uri_t command_uri = {.uri = "/send_command", .method = HTTP_POST, .handler = command_handler, .user_ctx = NULL};

  if (httpd_start(&camera_httpd, &config) == ESP_OK) {

    httpd_register_uri_handler(camera_httpd, &capture_uri);

    httpd_register_uri_handler( camera_httpd, &command_uri);
  }
}


void processServoMovement() {

  if (!servo_update_pending) {
    return;
  }

  servo_update_pending = false;

  float angle1 = target_servo1_angle;
  float angle2 = target_servo2_angle;
  float angle3 = target_servo3_angle;

  servo1.write(angle1);

  servo2.write(angle2);

  servo3.write(angle3);

  curr_servo1_angle = angle1;
  curr_servo2_angle = angle2;
  curr_servo3_angle = angle3;
}


void processFire() {
  if (!fire_requested) {
    return;
  }

  ledcWrite(IO2_PIN, 155);
  delay(1000);
  servo4.write(180);
  delay(1500);
  servo4.write(0);
  ledcWrite(IO2_PIN, 0);

  fire_requested = false;
}

bool initializeCamera() {

  camera_config_t config = {};

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;

  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;

  if (psramFound()) {
    config.fb_count = 2;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_LATEST;
  }
  else {
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    return false;
  }

  return true;
}

void setup() {

  Serial.begin(115200);

  delay(500);

  pinMode(LED_GPIO_NUM, OUTPUT);
  digitalWrite(LED_GPIO_NUM, LOW);

  ledcAttach(IO2_PIN, 5000, 8);
  ledcWrite(IO2_PIN, 0);

  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);
  servo4.setPeriodHertz(50);

  servo1.attach(SERVO1_PIN, 1000, 2000);
  servo2.attach(SERVO2_PIN, 1000, 2000);
  servo3.attach(SERVO3_PIN, 1000, 2000);
  servo4.attach(SERVO4_PIN, 500, 2500);

  servo1.write(curr_servo1_angle);
  servo2.write(curr_servo2_angle);
  servo3.write(curr_servo3_angle);
  servo4.write(0);

  delay(300);

  if (!initializeCamera()) {
    ESP.restart();
  }

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("WiFi config failed");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  startCameraServer();
}

void loop() {

  processServoMovement();

  processFire();

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
  }

  delay(5);
}