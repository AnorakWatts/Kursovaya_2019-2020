//
// Created by anorak on 15.11.2019.
//
#include "Player.hpp"


class Explosion {
private:
    sf::Time LastSpriteChange;// Последнее время смены спрайта анимации
    std::pair<int,int>Coords;// Координаты взрыва
    int number_of_Sprite;// Номер спрайта
public:
    ~Explosion();//В деструкторе чистить нечего
    bool deliteqm;//Удалять ли его из вектора взрывов
    Explosion(Cannonball&canonball, sf::Clock&GameClock);//Конструктор
    void DrawNextSprite(sf::RenderWindow&Window,sf::Clock&GameClock,std::vector<sf::Sprite>&Anime);//Рисуем тот же спрайт или следующий
};