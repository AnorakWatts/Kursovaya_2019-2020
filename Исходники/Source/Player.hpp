#include "Cannonball.hpp"
//
// Created by anorak on 11.11.2019.
//
class Player
{
public:
    Player(float FeeldL, float FeeldH, sf::Keyboard::Key anglePlus, sf::Keyboard::Key angleMinus, sf::Keyboard::Key energyPlus,
           sf::Keyboard::Key energyMinus, sf::Keyboard::Key massPlus, sf::Keyboard::Key massMinus, std::vector<Cannonball> &Cannonslink, sf::Clock &ClockLink,
           sf::Sprite &CSP, sf::Sprite &WheelSPL, bool LeftPlayer, sf::Font &textFont, sf::Keyboard::Key shootkey);
    void Control();// Метод управления
    void Draw(sf::RenderWindow&W,bool T);//Метод отрисовки
    ~Player();// Деструктор(Пустой, так как все переменные имеют свои деструктооры срабатывающие автомотически
    void TrytoGetDamage();// Проверка на попадание
    bool is_deadqm();// Проверка на смерть


private:
    std::pair<double ,double >coords;// Координаты пушки
    int FeeldWidth;// Длинна игрового поля
    double  mas;// Масса будущего снаряда
    double angle;// Угол приложения энергии к снаряду
    double energymax;// Максимально возможная энергия
    double  procent;// Процент от макс. энергии приложенный к след снаряду
    void Shoot();// Метод стрельбы
    void MakeTheory();// Метод расчета теоретической траектории полета
    // Кнопки
    sf::Keyboard::Key MassPlusskey,MassMinuskey,AnglePlusskey,AngleMinuskey,EnergyPluskey,EnergyMinuskey,ShootKey;
    std::vector<Cannonball>&Cannonballslink;// Ссылка на вектор ядер из класса игры
    sf::Clock&Clocklink;// Ссылка на игровые часы
    sf::Time Tcooldown;// Время последнего нажатия на кнопку нужно для ограничения скорости изменения параметров
    sf::Vertex Trace[5000];// Вектор точек теоретической кривой
    sf::Sprite CannonSP,WheelSP;// Спрайты Пушки и колес
    bool LeftPosition;// Слева игрок или справа
    int HP;// Здоровье
    sf::Text hptext,parametrs;// Текст для вывода здоровья и параметров выстрела
};