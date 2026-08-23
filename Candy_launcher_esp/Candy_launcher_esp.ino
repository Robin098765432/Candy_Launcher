#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include <ESP32Servo.h>
#include <math.h>
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

IPAddress local_IP(192, 168, 0, 120);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 0, 0);

httpd_handle_t camera_httpd = NULL;

static esp_err_t capture_handler(httpd_req_t *req) {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/jpeg");
  
  esp_err_t res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
  
  esp_camera_fb_return(fb);
  return res;
}

static esp_err_t command_handler(httpd_req_t *req) {
  char buffer[100];//makes a buffer for message
  int ret = httpd_req_recv(req, buffer, min((int)req->content_len, 99)); //feches message
  if (ret <= 0) {// error check
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }
  buffer[ret] = '\0';// sealing

  String command = String(buffer);
  Serial.print("Text Command Received: ");
  Serial.println(command);

  if (command == "FIRE") {

    httpd_resp_send(req, "FIRE_OK", HTTPD_RESP_USE_STRLEN);

  } else if (command == "FLASH_OFF") {

    digitalWrite(LED_GPIO_NUM, LOW);
    httpd_resp_send(req, "LED_OFF", HTTPD_RESP_USE_STRLEN);

  } else if (command == "FLASH_ON") {

    digitalWrite(LED_GPIO_NUM, HIGH);
    httpd_resp_send(req, "LED_ON", HTTPD_RESP_USE_STRLEN);

  } else if (command.startsWith("SET_SERVO_ANGLES")) {

    int first_value = command.indexOf(' ');
    int second_value = command.indexOf(' ', first_value + 1);
    int third_value = command.indexOf(' ', second_value + 1);

    if (first_value != -1 && second_value != -1 && third_value != -1) {

      float angle1 = command.substring(first_value + 1, second_value).toFloat();
      float angle2 = command.substring(second_value + 1, third_value).toFloat();
      float angle3 = command.substring(third_value + 1).toFloat();


      if (abs(angle1 - curr_servo1_angle) < 2 && abs(angle2 - curr_servo2_angle) < 2 && abs(angle3 - curr_servo3_angle) < 2) {
        httpd_resp_send(req, "FIRING", HTTPD_RESP_USE_STRLEN);
        analogWrite(IO2_PIN, PWM_VALUE);
        delay(300);
        servo4.write(180);
        delay(800);
        servo4.write(0);
        analogWrite(IO2_PIN, 0);
      }
      else {
        servo1.write(angle1);
      servo2.write(angle2);
      servo3.write(angle3);

      curr_servo1_angle = angle1;
      curr_servo2_angle = angle2;
      curr_servo3_angle = angle3;

      httpd_resp_send(req, "SERVO_ANGLES_SET", HTTPD_RESP_USE_STRLEN);
      }


      

    }

  } else {

    httpd_resp_send(req, "UNKNOWN_COMMAND", HTTPD_RESP_USE_STRLEN);

  }

  return ESP_OK;
  }

void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t capture_uri = { .uri = "/capture", .method = HTTP_GET, .handler = capture_handler, .user_ctx = NULL };
  httpd_uri_t command_uri = { .uri = "/send_command", .method = HTTP_POST, .handler = command_handler, .user_ctx = NULL };

  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &capture_uri);
    httpd_register_uri_handler(camera_httpd, &command_uri);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_GPIO_NUM, OUTPUT);
  delay(500);//power stabilise
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);
  servo4.setPeriodHertz(50);
  servo1.attach(SERVO1_PIN, 500, 2500);
  servo2.attach(SERVO2_PIN, 500, 2500);
  servo3.attach(SERVO3_PIN, 500, 2500);
  servo4.attach(SERVO4_PIN, 1000, 2000);
  pinMode(IO2_PIN, OUTPUT);
  servo4.write(0);

  servo1.write(curr_servo1_angle);
  servo2.write(curr_servo2_angle);
  servo3.write(curr_servo3_angle);

  camera_config_t config;
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
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x. Retrying loop...\n", err);
    delay(500);
    ESP.restart(); // auto reset
  }

  if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  
  startCameraServer();
}

void loop() {
  delay(10000); 
}