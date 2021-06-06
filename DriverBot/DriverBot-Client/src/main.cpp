#include <math.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>

#include <config.h>

#define motorOutputPin 5
#define motorDirectionPin 0

bool Direction;
int16_t Input;
int8 servoOffset = 0;
uint16_t Output;
StaticJsonDocument<128> doc;

Servo servo;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  long unsigned int startTime = micros();

  //Serial.print("Message arrived in topic: ");
  //Serial.println(topic);

  //Serial.println("Message:");

  if (strcmp((char *)payload, "Hello from web666-DriverBot") == 0 || strcmp((char *)payload, "Hello from webrd") == 0)
  {
    //Serial.println("Ping");
    //Serial.println((char *)payload);
    mqttClient.publish(mqttPubTopic, "Hello From ESP8266");

    return;
  }
  //Serial.println((char *)payload);
  deserializeJson(doc, payload);
  serializeJsonPretty(doc, Serial);

  JsonVariant ServoOffset = doc["servoOffset"];
  if(!ServoOffset.isNull()){
    servoOffset = ServoOffset;
    servoOffset = servoOffset * -1;
  }

  uint16 forward = doc["Forward"];
  uint16 backward = doc["Backward"];
  uint8 turning = int(doc["Turning"]) + 90;

  Input = forward - backward;
  Direction = (Input > 0 ? HIGH : LOW);
  Output = abs(Input);

  //Serial.println(Output);
  //Serial.println(turning);

  analogWrite(motorOutputPin, Output);
  digitalWrite(motorDirectionPin, Direction);

  servo.write(turning + servoOffset);
  Serial.println(micros() - startTime);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(motorDirectionPin, OUTPUT);
  pinMode(motorOutputPin, OUTPUT);

  Serial.begin(115200);

  servo.attach(14);

  WiFi.begin(ssid, password);

  int tries = 1;

  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //"clear" screen
    Serial.println(ssid);
    Serial.println(password);
    Serial.println();
    Serial.print("Connecting: X");
    Serial.println(tries);
    tries++;

    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);

  while (!mqttClient.connected())
  {
    Serial.println("Connecting to mqtt");
    digitalWrite(LED_BUILTIN, HIGH);

    if (mqttClient.connect(mqttClientName, mqttUser, mqttPassword))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed With state: ");
      Serial.print(mqttClient.state());
      delay(1000);
    }
  }

  digitalWrite(LED_BUILTIN, LOW);

  mqttClient.publish(mqttPubTopic, "Hello from ESP8266");
  mqttClient.subscribe(mqttSubTopic);
}

void loop()
{
  mqttClient.loop();
}