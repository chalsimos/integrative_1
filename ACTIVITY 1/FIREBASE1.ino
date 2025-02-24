#include <WiFi.h>
#include <FirebaseESP32.h>

// Replace with your network credentials
#define WIFI_SSID "CCSTemporaryWIFI"
#define WIFI_PASSWORD "IT_WIFI_2024"


// Replace with your Firebase project URL and secret
#define FIREBASE_HOST "your-firebase-database-url.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-database-secret"

// Define Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Generate some data to send
  int sensorValue = analogRead(34); // Example: Read from an analog sensor
  String path = "/sensorData/value";

  // Send data to Firebase
  if (Firebase.setInt(firebaseData, path, sensorValue)) {
    Serial.println("Data sent successfully!");
    Serial.println("Path: " + firebaseData.dataPath());
    Serial.println("Type: " + firebaseData.dataType());
    Serial.println("Value: " + String(firebaseData.intData()));
  } else {
    Serial.println("Failed to send data.");
    Serial.println("Reason: " + firebaseData.errorReason());
  }

  delay(5000); // Wait 5 seconds before sending the next data
}
