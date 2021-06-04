#include <math.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <config.h>

#define motorOutputPin 5
#define motorDirectionPin 0

bool Direction;
int16_t Input;
uint16_t Output;
StaticJsonDocument<128> doc;

Servo servo;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.println("Message:");

  deserializeJson(doc, payload);
  serializeJsonPretty(doc, Serial);

  uint16 forward = doc["Forward"];
  uint16 backward = doc["Backward"];
  uint8 turning = int(doc["Turning"])+90;

 
  Input = forward - backward;
  Direction = (Input > 0 ? HIGH : LOW);
  Output = abs(Input);


  Serial.println(Output);
  Serial.println(turning);

  analogWrite(motorOutputPin, Output);
  digitalWrite(motorDirectionPin, Direction);

  servo.write(turning);
 
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
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);

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

  mqttClient.publish(mqttTopic, "Hello from ESP8266");
  mqttClient.subscribe(mqttTopic);
}

void loop()
{
  mqttClient.loop();
}