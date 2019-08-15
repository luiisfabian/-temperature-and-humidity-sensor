
// INCLUIMOS LA LIBERIA DE LiquidCrystal_I2C de Frank de Brabander


#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <DHT.h>
//los diferentes que se acoplen a tu lcd
LiquidCrystal_I2C lcd(0x27,20,4); //0x20 o 0x27


// Incluimos librería


// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
// establecemos los pines desde nuestro arduino
int Alarma = 4;
int LedTemperatura = 3;
int LedTemperaturaGreen = 5;




void setup() {
 // Inicializamos comunicación serie
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();          // Activar luz de fondo 
  lcd.clear();              // Borrar LCD
 
  // Comenzamos el sensor DHT
  dht.begin();

  //establedemos pines
  pinMode(Alarma,OUTPUT);
  pinMode(LedTemperatura, OUTPUT);
  pinMode(LedTemperaturaGreen, OUTPUT);

}

void loop() {



delay(200);

  // Leemos la humedad relativa
  float humedad_relativa = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float temperatura_centigrados = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float grados_fahrenheir = dht.readTemperature(true);

  // Comprobamos si ha habido algún error en la lectura
  if (isnan(humedad_relativa) || isnan(temperatura_centigrados) || isnan(grados_fahrenheir)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  // Calcular el índice de calor en Fahreheit
  float Indice_calor_fahreheit = dht.computeHeatIndex(temperatura_centigrados, humedad_relativa);
  // Calcular el índice de calor en grados centígrados
  float Indice_calor_centigrados = dht.computeHeatIndex(temperatura_centigrados, humedad_relativa, false);

  Serial.print("Humedad: ");
  Serial.print(humedad_relativa);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(temperatura_centigrados);
  Serial.print(" *C ");
  Serial.print(grados_fahrenheir);
  Serial.print(" *F\t");
  Serial.print("Índice de calor: ");
  Serial.print(Indice_calor_centigrados);
  Serial.print(" *C ");
  Serial.print(Indice_calor_fahreheit);
  Serial.println(" *F");

    if(Indice_calor_centigrados>20.50)
  //if (Indice_calor_centigrados > 20.30)
  {
    //enciende alarma y led
    digitalWrite(Alarma, HIGH);
    digitalWrite(LedTemperatura, HIGH);// Encendemos led  alerta
    digitalWrite(LedTemperaturaGreen, LOW);// Apagamos led de no alerta
    tone( 12, 1000, 500);//establece el tono
  }
  else {
    //apaga alarma y led

    digitalWrite(Alarma, LOW); //apagamos alarma
    digitalWrite(LedTemperatura, LOW); // Apagamos led de alerta
    digitalWrite(LedTemperaturaGreen, HIGH); // Encendemos led de no alerta

  }
  lcd.begin(20, 4);
  lcd.setCursor(1,0);       // coordenadas LCD (x,y)
  lcd.print("LA TEMPERATURA ES");   // Mensaje de inicio 
   lcd.setCursor(2,2);       // coordenadas LCD (x,y)
   lcd.print(Indice_calor_centigrados);  // Mensaje de inicio
  lcd.print(" GRADOS");  // Mensaje de inicio
  



}
