#include <WiFi.h>
#include <string>
#include <FirebaseESP32.h>

#define WIFI_SSID "Maria-2.4"
#define WIFI_PASSWORD "vianasil"

#define FIREBASE_HOST "https://meuconforto-c4a46.firebaseio.com"
#define FIREBASE_AUTH "yzU3A1BXIFxNhDHWy7pA9jT3ZpTD60YNpfG5RL9Z"


//Define Firebase data object
FirebaseData data;

String nodo = "/users/wkfRKcTjSTbMDOG2qOiSRI4sme32/rooms";

//operadores
int porta;
String power="";

int strToInt(String str){
	char carray[str.length()+1];
	str.toCharArray(carray, sizeof(carray));
	return atoi(carray);
}

void streamTimeoutCallback(bool timeout)
{
	if (timeout){
		Serial.println();
		Serial.println("Stream timeout, resume streaming...");
		Serial.println();
	}
}

void objeto(StreamData data){
	FirebaseJson &json = data.jsonObject();
	//Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    //Serial.println(jsonStr);
	//Serial.println();
    Serial.println("Verificando portas e power");
    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";

    int type = 0;
    for (size_t i = 0; i < len; i++){
        json.iteratorGet(i, type, key, value);

		//condicionais
		if(key=="porta"){
			porta=strToInt(value);
			Serial.print("Porta: ");
			Serial.print(porta);
			Serial.print(", ");
		}
		if(key=="power"){
			power=value;
			Serial.print("Power: ");
			Serial.println(power);
			Serial.println();
			if(power=="on")
				digitalWrite(porta, HIGH);
			else
				digitalWrite(porta, LOW);
			
			
		}
    }
    json.iteratorEnd();
}

void setPinOutput(){
	pinMode(4,OUTPUT);
	pinMode(5,OUTPUT);
	pinMode(2,OUTPUT);
	pinMode(21,OUTPUT);
	pinMode(18,OUTPUT);
	pinMode(15,OUTPUT);
	pinMode(19,OUTPUT);
	pinMode(22,OUTPUT);
	pinMode(23,OUTPUT);
}

void connectWifi(){
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED){
		Serial.print(".");
		delay(300);
	}
	Serial.println();
	Serial.print("Connected with IP: ");
	Serial.println(WiFi.localIP());
	Serial.println();
}

void setup()
{
	Serial.begin(9600);
	setPinOutput();
	connectWifi();

	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
	Firebase.reconnectWiFi(true);

	if (!Firebase.beginStream(data, nodo)){
  		//Could not begin stream connection, then print out the error detail
		Serial.println(data.errorReason());
	}
	Firebase.setStreamCallback(data, objeto, streamTimeoutCallback, 8192);

}


void loop(){
	
}
