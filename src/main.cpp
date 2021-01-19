#include <WiFi.h>
#include <string>
#include "FirebaseESP32.h"

#define WIFI_SSID "Maria-2.4"
#define WIFI_PASSWORD "vianasil"

#define FIREBASE_HOST "https://meuconforto-c4a46.firebaseio.com"
#define FIREBASE_AUTH "yzU3A1BXIFxNhDHWy7pA9jT3ZpTD60YNpfG5RL9Z"


FirebaseData data;

String nodo = "/rooms";
bool iterar = true;


void setup() {
	Serial.begin(9600);
	Serial.println();

	pinMode(4,OUTPUT);
	pinMode(5,OUTPUT);
	pinMode(2,OUTPUT);
	pinMode(21,OUTPUT);
	pinMode(18,OUTPUT);
	pinMode(15,OUTPUT);
	pinMode(19,OUTPUT);
	pinMode(22,OUTPUT);
	pinMode(23,OUTPUT);
	

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("Conectando ao Wi-Fi");
	while (WiFi.status() != WL_CONNECTED){
		Serial.print(".");
		delay(300);
	}
	Serial.println();

	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
	Firebase.reconnectWiFi(true);
}

int strToInt(String str){
	char carray[str.length()+1];
	str.toCharArray(carray, sizeof(carray));
	return atoi(carray);
}

void objeto(){
	Firebase.getJSON(data, nodo);
	FirebaseJson &json = data.jsonObject();
	//Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    //Serial.println(jsonStr);
	Serial.println();
    Serial.println("Verificando portas e power");
    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";

	//operadores
	int porta;
	String power="";

    int type = 0;
    for (size_t i = 0; i < len; i++){
        json.iteratorGet(i, type, key, value);

		//condicionais
		if(key=="porta"){
			porta=strToInt(value);
			/* Serial.print("Porta: ");
			Serial.print(porta);
			Serial.print(", "); */
		}
		if(key=="power"){
			power=value;
			/* Serial.print("Power: ");
			Serial.println(power); */
			if(power=="on")
				digitalWrite(porta, HIGH);
			else
				digitalWrite(porta, LOW);
			
			
		}
    }
    json.iteratorEnd();
}

void loop() {
	objeto();
	delay(1500);
}