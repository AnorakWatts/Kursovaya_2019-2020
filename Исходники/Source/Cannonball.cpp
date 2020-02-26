//
// Created by anorak on 26.11.2019.
//

#include "Cannonball.hpp"

Cannonball::Cannonball(int x, int y, double speedx, double speedy, sf::Time StartTime, double masscoof,bool LeftorRigth)
{
    this->StartTime=StartTime;//Время начала полета
    //Координаты текущие и начальные
    coords.first=x;
    coords.second=y;
    coordsbase.first=x;
    coordsbase.second=y;
    //Скорость считается при выстреле см. Player::Shoot()
    speed.first=speedx;
    speed.second=speedy;
    //Удалять рано
    deliteqm=0;
    //Тоже считается при выстреле
    this->masscoof=masscoof;
    Player=LeftorRigth;
}
void Cannonball::Move(sf::Time T) {
    //Физика X=x0+Vx*t    Y=y0+Vy*t+(gt^2)/2
    coords.first=coordsbase.first+(speed.first*4*(T.asSeconds() - StartTime.asSeconds()));
    coords.second= coordsbase.second -speed.second*4*(T.asSeconds() - StartTime.asSeconds()) + (9.8 * pow(4*((T.asSeconds() - StartTime.asSeconds())), 2)) / 2;
}

void Cannonball::Draw(sf::RenderWindow &W, sf::Sprite &S) {
    float y;
    //Y начала земли
    y=((float)W.getSize().y-(125*((float)W.getSize().y/720)));
    y-=47*masscoof;
    S.setScale(masscoof,masscoof);
    S.setPosition(coords.first,coords.second);//Смена координат спрайта по ссылке
    W.draw(S);//Рисуем копию спрайта
    //Если за землей разрешаем удалять
    if(coords.second>y)
        deliteqm=1;
}

bool Cannonball::operator == (const Cannonball&canonball)
{//Если координаты совпадут то 1
    double x,y;
    x=this->coords.first-canonball.coords.first;
    y=this->coords.second-canonball.coords.second;
    if((abs(x)<30)&&(abs(y)<30))
        return 1;
    return 0;
}
