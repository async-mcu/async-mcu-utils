#include <async/Duration.h>

using namespace async;

void setup() {
  Serial.begin(9600);

  // Создаем две длительности
  auto duration1 = Duration::ms(1500); // 1.5 seconds
  auto duration2 = Duration::ms(3000); // 3 seconds

  // Addition
  auto sum = duration1.add(duration2);
  Serial.print("Sum: ");
  Serial.print(sum.get(Duration::SECONDS)); // 4.5 seconds (4500 ms)
  Serial.println(" seconds");

  // Subtraction
  auto diff = duration2.subtract(duration1);
  Serial.print("Difference: ");
  Serial.print(diff.get(Duration::SECONDS)); // 1.5 seconds (1500 ms)
  Serial.println(" seconds");

  // Создаем длительность
  auto duration = Duration::ms(3000); // 3 секунды

  // Проверяем, прошло ли 3 секунды с момента старта
  if (duration.after(async::Duration::now())) {
    Serial.println("3 seconds have not yet passed.");
  } else {
    Serial.println("3 seconds have passed.");
  }
  
  // Проверяем, прошло ли 2 секунды (в миллисекундах)
  if (duration.before(Duration::ms(2000))) {
    Serial.println("2 seconds have not yet passed.");
  } else {
    Serial.println("2 seconds have passed.");
  }
}

void loop() {
  auto from = Duration::now();
  delay(random(1000, 2000));

  Serial.print("Difference random: ");
  Serial.print(Duration::now().subtract(from).get(Duration::MILLIS)); 
  Serial.println(" millis");
}