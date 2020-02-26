//
// Created by anorak on 26.11.2019.
//
#if defined(linux) || defined(__linux)
#include <unistd.h>
#endif
#if defined(_WIN32) || defined (_WIN64)
//#include <Windows.h>
#endif
#include <iostream>
#include <sstream>
#include <vector>
#include "SFML/Graphics.hpp"
#include <fstream>
#include <cmath>

class Cannonball {
public:
    //Конструктор
    Cannonball(int x, int y, double speedx, double speedy, sf::Time StartTime, double masscoof,bool LeftorRigth);
    ~Cannonball()= default;//В ядре у всего есть дефолтные деструкторы
    void Move(sf::Time T);//Функция движения
    void Draw(sf::RenderWindow&W,sf::Sprite&S);//Рисование ядра координаты же в привате
    bool operator == (const Cannonball&canonball);//В игре сравниваем ядра для столкновения
    bool deliteqm;//Удалять ли ядро

private:
    std::pair<double ,double >coords;//Координаты
    std::pair<double,double> speed;//Скорость по X и по Y
    std::pair<int,int>coordsbase;//Начальные координаты нужны для расчета текущих смотри Move
    sf::Time StartTime;//Для того же расчета текущих координат
    double  masscoof;//коофицент массы для размера спрайта и урона

    //Даем другим классам право лазить куда неможно
    friend class Explosion;//Чтобы делать взрывы
    friend class Player;//Чтобы проверять получение урона
    bool Player;//Во избежание самоурона(попаданий в самого себя)
};
