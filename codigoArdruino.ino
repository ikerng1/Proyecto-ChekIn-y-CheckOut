#código en Arduino (ESP32-CAM)
#Este código utiliza la biblioteca esp32campara capturar imágenes con la cámara ESP32-CAM y enviarlas a través de HTTP utilizando el servidor web WebServer. El código define tres resoluciones de imagen diferentes (baja, media y alta) y tres funciones para manejar las solicitudes HTTP para cada resolución. La función serveJpg()se encarga de capturar la imagen y enviarla a través de HTTP.
#ya que solo estamos utilizando el ESP32-CAM para capturar la imagen y enviarla a través de HTTP. El código en Python se encarga de procesar la imagen y enviar el dato a través del socket.


#código en Arduino (ESP32-CAM)
#--------------------------------------

#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>

const char* WIFI_SSID = "ARRIS-CF12";
const char* WIFI_PASS = "B397F7B587D6BBDE";

WebServer server(80);

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %dbn", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}

void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");

  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);

  server.begin();
}

void loop()
{
  server.handleClient();
}


#----------------------------------------------------------------------------------------------------------------------
#----------------------------------------------------------------------------------------------------------------------
código en Arduino (ESP32-CAM):

/**
 * @file ESP32-CAM_WebServer.ino
 * @brief Ejemplo de servidor web para ESP32-CAM
 * @author Tu nombre
 * @date Fecha de creación
 */

#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>

/**
 * @brief Definición de las credenciales de Wi-Fi
 */
const char* WIFI_SSID = "ARRIS-CF12"; /**< SSID de la red Wi-Fi */
const char* WIFI_PASS = "B397F7B587D6BBDE"; /**< Contraseña de la red Wi-Fi */

/**
 * @brief Instancia del servidor web
 */
WebServer server(80);

/**
 * @brief Definición de las resoluciones de imagen
 */
static auto loRes = esp32cam::Resolution::find(320, 240); /**< Resolución baja (320x240) */
static auto midRes = esp32cam::Resolution::find(350, 530); /**< Resolución media (350x530) */
static auto hiRes = esp32cam::Resolution::find(800, 600); /**< Resolución alta (800x600) */

/**
 * @brief Función para servir una imagen JPEG
 */
void serveJpg()
{
  /**
   * @brief Captura una imagen con la cámara
   */
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %dbn", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  /**
   * @brief Envía la imagen a través de HTTP
   */
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

/**
 * @brief Función para manejar solicitudes HTTP para la resolución baja
 */
void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}

/**
 * @brief Función para manejar solicitudes HTTP para la resolución alta
 */
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

/**
 * @brief Función para manejar solicitudes HTTP para la resolución media
 */
void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}

/**
 * @brief Función de configuración
 */
void setup()
{
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");

  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);

  server.begin();
}

/**
 * @brief Función de bucle principal
 */
void loop()
{
  server.handleClient();
}