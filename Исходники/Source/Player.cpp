//
// Created by anorak on 11.11.2019.
//

#include <iostream>
#include "Player.hpp"

Player::Player(float FeeldL, float FeeldH, sf::Keyboard::Key anglePlus, sf::Keyboard::Key angleMinus, sf::Keyboard::Key energyPlus,
               sf::Keyboard::Key energyMinus, sf::Keyboard::Key massPlus, sf::Keyboard::Key massMinus, std::vector<Cannonball> &Cannonslink,
               sf::Clock &ClockLink,
               sf::Sprite &CannonSPL, sf::Sprite&WheelSPL, bool LeftPlayer, sf::Font &textFont, sf::Keyboard::Key shootkey)
        : Cannonballslink(Cannonslink), Clocklink(ClockLink), CannonSP(CannonSPL), WheelSP(WheelSPL){
    if(LeftPlayer)
        coords.first=0;// Если слево то координата Х=0
    else
        coords.first=FeeldL;// Если справо то Х==длинна поля
    coords.second=(FeeldH-(125*(FeeldH/720)));// Координаты Y так как высота фона 720 а 125 снизу из них земля
    coords.second-=47*2;// Высота колес и пушки таким образом координата по Y==Y верхнего левого угла спрайта пушки
    //Кнопки
    MassMinuskey=massMinus;
    MassPlusskey=massPlus;
    AngleMinuskey=angleMinus;
    AnglePlusskey=anglePlus;
    EnergyMinuskey=energyMinus;
    EnergyPluskey=energyPlus;
    ShootKey=shootkey;
    //Текст
    hptext.setFont(textFont);// Шрифт для текста
    hptext.setString("HP=100");// Выводимый текст
    hptext.setFillColor(sf::Color::Red);// Цвет текста
    parametrs.setFont(textFont);
    parametrs.setString("Mas: 30\nAngle: 45\nEnergy %: 50");
    parametrs.setFillColor(sf::Color::Red);
    //Переменные
    angle=45;
    procent=0.5;
    energymax=350*FeeldL;
    mas=30;
    Tcooldown=ClockLink.getElapsedTime();// Просто для начала работы, эта переменная нужна для другой функции
    LeftPosition=LeftPlayer;
    HP=100;
    FeeldWidth=FeeldL;
    //Подгонка координат что-то по размерам спрайта что-то ручками методом научного тыка
    if(LeftPlayer) {
        CannonSP.setPosition(coords.first+15, coords.second);//Координаты левого верхнего угла спрайта
        WheelSP.setPosition(coords.first,coords.second+47);
        coords.first=coords.first+107;
    }
    else
    {
        CannonSP.setPosition(coords.first-15, coords.second);
        WheelSP.setPosition(coords.first,coords.second+47);
        coords.first=coords.first-107;
        coords.second+=10;
        WheelSP.setScale(-1,1);//Разварот спрайтов для правого спрайта
        CannonSP.setScale(-1,1);
    }
    hptext.setPosition(coords.first,100);//Координаты текста
    parametrs.setPosition(coords.first,300);
    if(!LeftPlayer)
        parametrs.setPosition(coords.first-100,300);
}

void Player::Shoot() {
    double v;//Скорость
        v=(sqrt(energymax*procent* 2 / mas));//E=(mv^2)/2
    sf::Time shootTime=Clocklink.getElapsedTime();// Время выстрела нужно для расчета полета см.Cannonball
    if(LeftPosition) {
        // Если слева то скорость считается как указано ниже составляющие скорости по OX и OY
        Cannonball C(coords.first, coords.second, v * cos(angle * M_PI / 180), v * sin(angle * M_PI / 180), shootTime, mas / 50,LeftPosition);
        Cannonballslink.push_back(C);// Добавление ядра в вектор ядер из класса игры
    }
    else
    {
        // Тоже что выше но по Y скорость отрицательная так как из большого Y надо попасть в 0
        Cannonball C(coords.first, coords.second, -v * cos(angle * M_PI / 180), v * sin(angle * M_PI / 180), shootTime, mas / 50,LeftPosition);
        Cannonballslink.push_back(C);
    }

}
void Player::Control() {
    // Если с прошлого нажатия прошло более 300 милисекунд
    // Формально с прошлого принятия действия так как проверяется удерживается ли кнопка
    if (((Clocklink.getElapsedTime() - Tcooldown).asMilliseconds() ) > 300) {

        if ((sf::Keyboard::isKeyPressed(ShootKey))){//Если кнопка такая-то нажата/зажата то делай то
            Shoot();// Выстрел
        }
        if (sf::Keyboard::isKeyPressed(AnglePlusskey)) {
            if (angle < 90) {  // Угол не может быть более 90 градусов
                angle += 1;
            }
        }
        if (sf::Keyboard::isKeyPressed(AngleMinuskey)) {
            if (angle > -30) {// И менее -30
                angle -= 1;
            }
        }
        if (sf::Keyboard::isKeyPressed(EnergyPluskey)) {
            if (procent < 1) // Проценты 1%==0.01   Не более 1==100%
                procent += 0.05;
        }
        if (sf::Keyboard::isKeyPressed(EnergyMinuskey)) {
            if (procent > 0.05)// Не менее 0.05==5%
                procent -= 0.05;
        }
        if (sf::Keyboard::isKeyPressed(MassPlusskey)) {
            if (mas < 50)   // Масса не более 50
                mas += 5;
        }
        if (sf::Keyboard::isKeyPressed(MassMinuskey)) {
            if (mas > 10)     // Не менее 10
                mas -= 5;
        }
        Tcooldown=Clocklink.getElapsedTime();// После действия обновляем кулдаун
        MakeTheory();// Перерасчет теоретической кривой
        std::ostringstream parametrostrem;// Изменение строки параметров
        parametrostrem << "Mas: " << mas << std::endl << "Angle: " << angle << std::endl << "Energy %:" << procent * 100;
        parametrs.setString(parametrostrem.str());
    }
}
Player::~Player() = default;//В прототипах я писал почему деструктор не требует усилий

void Player::Draw(sf::RenderWindow&W,bool T)
{
    W.draw(CannonSP);// Вывод спрайта в окно SFML по ссылке
    W.draw(WheelSP);
    if(T)
        W.draw(Trace, 5000, sf::LinesStrip);// Вывод теор. кривой
    W.draw(hptext);// Вывод текста
    W.draw(parametrs);
}

void Player::MakeTheory() {
    double v;//Скорость
    double size;//Длинна кривой
    v=(sqrt(energymax*procent* 2 / mas));//E=(mv^2)/2
    size= ((FeeldWidth / 3) - coords.first) / 200;//Методом подгона
    if(!LeftPosition)
        size=size/2;//Подгона ручками методом тыка
    if(LeftPosition)
   for(int i=0;i<5000;i++)
    {
       //Координата точки графика
        Trace[i].position=sf::Vector2f(coords.first + i * size / 5000 * v * cos(angle * M_PI / 180), coords.second - i * size / 5000 * v * sin(angle * M_PI / 180) + (9.8 * pow(i * size / 5000, 2)) / 2+15);
        Trace[i].color=sf::Color::Blue;//Цвет
    }
    if(!LeftPosition)
    for(int i=0;i<5000;i++)
    {
        //Скорость в разные стороны для левого и правого
        Trace[i].position=sf::Vector2f(coords.first + i * size / 5000 * v * cos(angle * M_PI / 180), coords.second + i * size / 5000 * v * sin(angle * M_PI / 180) + (9.8 * pow(i * size / 5000, 2)) / 2+5);
        Trace[i].color=sf::Color::Red;
    }
}

void Player::TrytoGetDamage() {
    for(int i=0;i<Cannonballslink.size();i++) {
        if(LeftPosition!=Cannonballslink[i].Player)
        {
            // Разница координат между тем кто пытается получить урон и ядром
            // И ядром и его начальными координатами
            int xdif = abs(this->coords.first - Cannonballslink[i].coords.first);
            int ydif = abs(this->coords.second - Cannonballslink[i].coords.second);
            if (xdif < 30 && ydif < 47) // Если попал -HP
            {
                HP -= 25 * Cannonballslink[i].masscoof;
                std::ostringstream HPstream;
                HPstream << "HP " << !LeftPosition + 1 << "= " << HP;// Изменение текста HP
                hptext.setString(HPstream.str());
                Cannonballslink[i].deliteqm = 1; // Пометка уничтожения ядра
            }
        }
    }
}

bool Player::is_deadqm() {
    if(HP<=0)
        return true;// СМЕРТЬ!
    return false;
}

