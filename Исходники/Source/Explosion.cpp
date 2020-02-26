//
// Created by anorak on 15.11.2019.
//
#include "Explosion.hpp"

Explosion:: Explosion(Cannonball&canonball, sf::Clock&GameClock)
{//Взрыв там же где ядро но чуть в бок 1.размер ядра 2.Спрайт взрыва большой,его надо сместить тч -150
    Coords.first=canonball.coords.first+(47*canonball.masscoof)-150;
    Coords.second=canonball.coords.second+(47*canonball.masscoof)-150;
    number_of_Sprite=0;//Начинаем с 0го спрайта
    deliteqm=0;//Его убивать не надо
    LastSpriteChange=GameClock.getElapsedTime();//Время создания для времени между кадрами анимации
}

void Explosion::DrawNextSprite(sf::RenderWindow&Window,sf::Clock&GameClock,std::vector<sf::Sprite>&Anime)
{
    if(number_of_Sprite==Anime.size()) {//Если мы все прошли то удаляем взрыв
        deliteqm = 1;
    }
    else if((GameClock.getElapsedTime()-LastSpriteChange).asMilliseconds()>50)
    {//Если прошло более 50мс то меняем кадр
        Anime[number_of_Sprite].setPosition(Coords.first,Coords.second);
        Window.draw(Anime[number_of_Sprite]);
        LastSpriteChange=GameClock.getElapsedTime();
        number_of_Sprite++;
    }
    else
    {//Иначе рисуем старый кадр
        Anime[number_of_Sprite].setPosition(Coords.first,Coords.second);
        Window.draw(Anime[number_of_Sprite]);
    }
}

Explosion::~Explosion(){}
