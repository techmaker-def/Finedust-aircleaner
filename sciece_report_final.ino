#include <HuemonelabKit.h>

int speed = 6;

int plus = 5;

int minus = 4;

//블루투스와 RGB LED 코드는 휴몬랩 라이브러리 사용
Bluetooth BT(2,3); // 블루투스 TX, RX핀 설정
RGBLed rgb(11,12,13); //RGB LED (red,green,blue)핀 설정

//미세먼지센서 소스 코드
int dustPin = A0; //미세먼지센서의 아날로그 핀 설정
int ledPower = 10; //미세먼지센서의 적외선감지 핀 설정

int samplingTime = 280; //샘플링 시간 설정 (내부 LED를 켠 후 출력 신호 측정)
int deltaTime = 40; // LED를 다시 끄기 전 40(마이크로 초)동안 일시 정지
int sleepTime = 9680;

float voMeasured = 0; //미세먼지센서가 그대로 읽어오는 아날로그 값
float calcVoltage = 0; // 미세먼지센서가 측정하는 전기신호 (IR Diode로 들어오는 빛 주기)
float dustDensity = 0; //미센먼지 밀도 계산식으로 환산된 값

void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT); //미세먼지 센서 적외선 LED 핀모드 설정
  BT.begin(9600); //휴몬랩 라이브러리 블루투스 설정
  pinMode(speed, OUTPUT);
  pinMode(plus, OUTPUT);
  pinMode(minus, OUTPUT);
}


void loop(){
  digitalWrite(ledPower,LOW); // 미세먼지 Led 켜기
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(dustPin); //미세먼지 값 읽기
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // 미세먼지 Led 끄기
  delayMicroseconds(sleepTime);

  //먼지 밀도를 출력 전압(vo)에 비례하여 ㎍/m³단위의 먼지 밀도로 매핑 (Chris Nafis의 계산법 참고)
calcVoltage = voMeasured*(5.0/1024.0);
dustDensity = (0.17*calcVoltage-0.1)*1000;

//시리얼모니터/블루투스에 미세먼지 농도값 표시
  Serial.println(dustDensity);
  BT.print(dustDensity); //휴몬랩 라이브러리 블루투스 출력 함수
  delay(2000);

  if (BT.available() ) {
    char text = BT.read();
    if (text == '0') {
      digitalWrite(plus, HIGH);
      digitalWrite(minus, LOW);
      analogWrite(speed, 255);
    } else {
      digitalWrite(plus, HIGH);
      digitalWrite(minus, LOW);
      analogWrite(speed, 0);
    }
  }
}
  
  
