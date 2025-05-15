#include <WiFi.h>
#include <FirebaseESP32.h>

// WiFi Credentials
#define WIFI_SSID "Alien"
#define WIFI_PASS "alien'spassword"

// Firebase Config
#define FIREBASE_HOST "helloworld-423ad-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "IZi1dT2Nsl62Ve8pd1lORPddMdZXbPnQdu10soJM"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int counter = 0;

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Optional: Set timeout options
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  
  Serial.println("Firebase initialized");
}

void loop() {
  // Increment counter every minute
  counter++;
  
  // Send to Firebase
  if (Firebase.setInt(fbdo, "/counter", counter)) {
    Serial.println("Sent: " + String(counter));
  } else {
    Serial.println("Failed: " + fbdo.errorReason());
    Serial.println("Firebase connection test:");
    if (Firebase.ready()) {
      Serial.println("Firebase is ready");
    } else {
      Serial.println("Firebase not ready");
    }
  }

  delay(60000); // 1 minute delay
}