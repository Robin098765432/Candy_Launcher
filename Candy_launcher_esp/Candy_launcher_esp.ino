#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"

#include "board_config.h"

const char* ssid = "Jojanneke en Linda 2";
const char* password = "C00kiemonster!!";

httpd_handle_t camera_httpd = NULL;

static esp_err_t capture_handler(httpd_req_t *req) {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/jpeg");
  \
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

  if (esp_camera_init(&config) != ESP_OK) return;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  
  startCameraServer();
}

void loop() {
  delay(10000); 
}