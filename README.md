# FabricBots2024 :turtle:

## Команда

* [Бочкарев Федор](https://t.me/Ger0r0r)

* [Курицына Кристина](https://t.me/k_kriiis)

* [Каплун Тамара](https://t.me/Sbighev)

* [Бочкарева Софья](https://t.me/Sofa_Sukhova)

## Задача
```sh
Построить робота без колес с дистанционным управлением для прохождения полосы препятствий на время
```
## Решение

Шагающий робот. Основной механизм состоит из зубчатой передачи для 4-х ног и двух моторов коллекторного типа (на правую и левую пару соответственно). В каждой паре между ногами сохраняется разность фаз $\pi$/2 за счет первоначальной сборки. Между правой и левой парой разность фаз не сохраняется. Дополнительной точной опоры по необходимости служит специально заниженное основание корпуса. Управление осуществляется с пульта с помощью радиомодулей.

![Робот-черепаха](https://github.com/Sofia-Sukhova/FabricBots2024/blob/main/common_files/turtle.jpg)

![](https://github.com/Sofia-Sukhova/FabricBots2024/blob/main/common_files/turtle.mp4)

## Материалы и технологии

### Робот
- Основной корпус и оси ног выполнены из фанеры с помощью технологии лазерной резки СО_2 лазером
- Шестерни механизма, обшивка и голова робота напечатаны из PETG пластика на 3d принтере
- Основания ног и крепежные кольца для панциря напечатаны из TPU пластика на 3d принтере
- Модуль управления выполнен на основе
  - Arduino Uno
  - платы-драйвера управления моторами
  - двух коллекторных моторов
  - радиомодуля с антенной
  - светодиодной матрицы (7 светодиодов)
  - батарейного отсека (аккумуляторы 18650 х3)
  - тумблера управления питанием


### Пульт Управления
- Корпус пульта выполнен из фанеры с помощью технологии лазерной резки СО_2 лазером
- Модуль управления выполнен на основе
  - Arduino Pro Micro
  - джойстика (управление движением робота)
  - тумблера и кнопки (управление светодиодной матрицей)
  - радиомодуля с антенной

## Трудности и проблемы

### Механизм

Изначально мы изготовили зубчатую передачу на основе деревянных шестерней, свободно вращающихся на оси, с передаточным числом механизма M=1. Оказалось, что при таких условиях механизму не хватает момента и мощности (из-за значительного трения между шестернями) для передвижения. 

Поэтому мы изменили механизм для достижения передаточного числа M=5, выполнили шестерни из PETG и закрепили их с помощью подшипников.

![Модель зубчатой передачи](https://github.com/Sofia-Sukhova/FabricBots2024/blob/main/common_files/mechan_model.jpg)
*Примерный вид механизма, реальные диаметры шестерней 10, 50 (х4) и 70 мм*

![Реализация механизма](https://github.com/Sofia-Sukhova/FabricBots2024/blob/main/common_files/mechan_real.jpg)

### Корпус

Основной трудностью при изготовлении корпуса было учесть все особенносьти робота (размеры, необходимоть дополнительной опоры, крепление мехнанизма, размещение управляющих элеметов), из-за чего пришлось несколько раз полностью перепроектировать основной корпус.

![Полная модель корпуса](https://github.com/Sofia-Sukhova/FabricBots2024/blob/main/common_files/mechan_model_whole.jpg)

В качестве дизайна робота была выбрана черепаха. Поэтому у нас также возникла трудность проектирования и изготовления панциря из плоских составных деталей и его крепления к основному корпусу.

![Модель панциря](https://github.com/Sofia-Sukhova/FabricBots2024/blob/main/common_files/pantzer_model_2.jpg)

### Управление

В процессе работы платы управления и радиомодулей несколько раз выходили из строя, что приводило к необходимости полной перепайки управляющей схемы.

Также возникли трудности с настройкой радиомодуля, для чего потребовалось создать многочисленные скетчи анализа подключения/работы радиомодуля. 



