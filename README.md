# **Proyecto cálculo de la gravedad**

## **Integrantes:**
1.  Pedro Juan Henao
2.  Juan David Oliveros

## **Descripción método y modelo**
La gravedad se puede calcular de distintas maneras, pero para este caso en concreto, utilizamos dos sensores separados una distancia h,
los cuales calcularán un tiempo t, consistente en la cantidad de tiempo que se demora un objeto en pasar del primer sensor al segundo sensor.
Para el cálculo como tal, usamos las fórmulas de caída libre:

$$
h = v_0 t + \frac{g t^2}{2}
$$

Suponiendo velocidad inicial igual a cero, nos queda:

$$
g = 2*\dfrac{h}{t^2}
$$

## **Descripción del Código**

### **Imports:**
```cpp
#include "mbed.h": Incluye la biblioteca principal de mbed, que proporciona funciones para trabajar con microcontroladores mbed.
#include <iostream>: Incluye la biblioteca de entrada/salida estándar de C++ para imprimir mensajes en la consola.
#include "TextLCD.h": Incluye la biblioteca para manejar la pantalla LCD de texto.
```

### **Inicialización de Objetos:**
```cpp
UnbufferedSerial serial(USBTX, USBRX, 9600);: Inicializa un objeto de comunicación serial sin búfer a través de los pines USBTX y USBRX con una velocidad de baudios de 9600.
TextLCD lcd (D2, D3, D4, D5, D6, D7, TextLCD::LCD16x2);: Inicializa un objeto de pantalla LCD de texto en los pines D2 a D7 con un formato de 16x2 caracteres.
AnalogIn lightSensor1(A0); y AnalogIn lightSensor2(A1);: Inicializa dos objetos de entrada analógica en los pines A0 y A1 para leer datos de sensores de luz.
Timer timer;: Inicializa un objeto temporizador.
```
### **Función Principal (main()):**

#### **Verificaciones:**
Se imprime un mensaje en la consola y en la pantalla LCD para indicar el inicio del cálculo de la gravedad.
```cpp
std::cout << "Inicia el cálculo de la gravedad: " << std::endl;
lcd.locate(0, 0);
lcd.printf("Tremendas conexiones");
```

#### **Inicialización de variables:**
Se inicializan variables booleanas paso1 y paso2 con el fin de definir un orden para las operaciones siguientes.
Además, se inicializan el treshold, que ayuda a definir si los sensores de luz están recibiendo luz o no, y el height, la cual indica la distancia que existe entre los sensores previamente definidos.
```cpp
bool paso1 = true;
bool paso2 = false;
float treshold = 0.2;
const float height = 0.1;
```

#### **While infinito:**
En este se realizarán los cálculos de la gravedad. Para ello:
##### **Primer if:**
Verifica que a el primer sensor no le esté llegando luz, lo que significa que un objeto está pasando por ahí.
Lo que hace es iniciar el timer, y eventualmente hace el cambio de booleanos, lo que implica que lo único que puede pasar es que pasa por el siguiente sensor. 
Con esto nos aseguramos de que, si el objeto es muy grande, el timer no se esté reiniciando constantemente.
```cpp
if (lightSensor1.read() < treshold && paso1) {
    timer.reset();
    timer.start();
    paso1 = false;
    paso2 = true;
}
```
##### **Segundo if:**
Verifica que el objeto este tapando la luz del segundo sensor, siempre y cuando haya pasado inicialmente por el primer sensor.
Aquí se para el timer, y se calcula el tiempo en milisegundos, para posteriormente pasarlo a segundos dividiendo entre 1000. 
Con ello tenemos todo listo para el cálculo de la gravedad, el cual se explicó anteriormente.
Luego se muestra tanto en consola como en el LCD, teniendo en cuenta para este último, limpiar la pantalla y buscar la posición adecuada para imprimir el mensaje.
Finalmente se cambian los booleanos nuevamente para repetir el experimento sin ningún problema.
```cpp
else if (lightSensor2.read() < treshold && paso2){
    timer.stop(); 
    float timeInSeconds = duration_cast<milliseconds>(timer.elapsed_time()).count();
    timeInSeconds /= 1000;
    float gravity = 2*height/pow(timeInSeconds,2);
    std::cout << "La gravedad es de: " << gravity << " m/s^2" << std::endl;
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("La gravedad es: ");
    lcd.locate(0, 1);
    lcd.printf("%f m/s^2", gravity);
    paso1 = true; 
    paso2 = false;       
}
