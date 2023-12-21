#include <SPI.h>  // Подключаем библиотеку для работы с SPI-интерфейсом
#include <nRF24L01.h> // Подключаем файл конфигурации из библиотеки RF24
#include <RF24.h> // Подключаем библиотеку для работа для работы с модулем NRF24L01
#define PIN_LED 3  // Номер пина Arduino, к которому подключен светодиод
#define PIN_CE  10  // Номер пина Arduino, к которому подключен вывод CE радиомодуля
#define PIN_CSN 9 // Номер пина Arduino, к которому подключен вывод CSN радиомодуля
RF24 radio(PIN_CE, PIN_CSN); // Создаём объект radio с указанием выводов CE и CSN

int potValue[2]; // Создаём массив для приёма значений потенциометра

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT); // Настраиваем на выход пин светодиода
  radio.begin();  // Инициализация модуля NRF24L01
  radio.setChannel(5); // Обмен данными будет вестись на пятом канале (2,405 ГГц)
  radio.setDataRate (RF24_1MBPS); // Скорость обмена данными 1 Мбит/сек
  radio.setPALevel(RF24_PA_HIGH); // Выбираем высокую мощность передатчика (-6dBm)
  radio.openReadingPipe (1, 0x7878787878LL); // Открываем трубу ID передатчика
  radio.startListening(); // Начинаем прослушивать открываемую трубу
}

void loop() {
  if(radio.available()){ // Если в буфер приёмника поступили данные
    radio.read(&potValue, sizeof(potValue)); // Читаем показания потенциометра
    Serial.println(potValue[0]);
    Serial.println(potValue[1]);
  }
  if((potValue[0]==529)||(potValue[0]==530)&&(potValue[1]==513)||(potValue[1]==512)){
    digitalWrite(2, HIGH); //выключение двигателя
    digitalWrite(3, HIGH); //выключение двигателя
  }
  else{
    if((potValue[0]==0)&&((potValue[1]>500)&&(potValue[1]<518))){
      digitalWrite(2, LOW); //включение двигателя(прямо)
      digitalWrite(3, LOW);//включение двигателя
    }
    if(((potValue[0]==529)||(potValue[0]==530))&&((potValue[1]==1023)||(potValue[1]==1022))){
      digitalWrite(2, LOW); //включение двигателя(налево)
      digitalWrite(3,HIGH );//выключение двигателя
    }
    if(((potValue[0]==529)||(potValue[0]==530))&&((potValue[1]==0))){
      digitalWrite(2, HIGH); //выключение двигателя(направо)
      digitalWrite(3,LOW);//включение двигателя
    }
  }
}
