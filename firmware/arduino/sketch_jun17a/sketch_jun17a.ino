// Определяем константы для цифрового и аналогового пинов
#define DO 2
#define AO A0
 
void setup() 
{
    // Инициализируем последовательное соединение
    Serial.begin(9600);
    // Устанавливаем пин DO как вход, пин AO работает как выход по умолчанию
    // pinMode(DO, INPUT);
}
 
void loop() 
{
    // Читаем цифровой сигнал с пина DO
    //int digital_signal = digitalRead(DO);
    // Читаем аналоговый сигнал с пина A0
    //int analog_signal  = analogRead(A0);
 
    // Если цифровой сигнал равен HIGH, то выводим сообщение о обнаружении магнитного поля
    //if (digital_signal == HIGH)
    //     Serial.println("Магнитное поле обнаружено!");
    // else 
    //     Serial.println("No.");
    
    // Выводим аналоговые показания
    //Serial.print("Показания на аналоговом выходе: ");
    //Serial.println(analog_signal);
    // Serial.println("----------------new read---------------");
    Serial.println(analogRead(A0));
    // Serial.println(analogRead(A0) / 1024.0 * 100.0);

    // Пауза в 1 секунду
    delay(200);
}