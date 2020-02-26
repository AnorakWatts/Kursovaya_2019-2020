//
// Created by anorak on 12.11.2019.
//

#include "Game.hpp"
#if defined(_WIN32) || defined (_WIN64)
#include<Windows.h>
#endif

Game::Game()
{// По умолчанию не пауза и HD разрешение
    Theory=1;
    FeeldWidth=1280;
    FeeldHegth=720;

}


void Game::BUILD()
{
    ExplosionT.loadFromFile("Sprites/Explosion.png"); // Загрузка текстуры
    TextureCanBallWheel.loadFromFile("Sprites/21.png");
    BackGround.loadFromFile("Sprites/background.png");
    font.loadFromFile("Sprites/F.ttf");// Загрузка шрифта
    sf::Sprite*expSprite;
    for(int i=0;i<3;i++)
    {// Заполнение вектора с анимацией взрыва
        for(int j=0;j<3;j++)
        {
            expSprite=new sf::Sprite(ExplosionT, sf::IntRect(j * 300, i * 300, 300, 300));
            ExplosionAnime.push_back(*expSprite);
        }
    }
    delete(expSprite);
    Wintext.setCharacterSize(FeeldWidth/20);// Изменение размера текста под разрешение заданное ранее из меню
    cannonSP.setTexture(TextureCanBallWheel);// Назначение спрайту текстуры
    cannonSP.setTextureRect(sf::IntRect(36, 17, 107, 68));// Вырезание спрайта из текстуры
    cannonballSP.setTexture(TextureCanBallWheel);
    cannonballSP.setTextureRect(sf::IntRect(184, 46, 47, 47));
    wheelsSP.setTexture(TextureCanBallWheel);
    wheelsSP.setTextureRect(sf::IntRect(287,44,111,47));
    backgroundSP.setTexture(BackGround);
    backgroundSP.setScale(FeeldWidth / 1280, FeeldHegth / 720);// Изменение размера задника
    Wintext.setPosition(FeeldWidth / 2 - 100, FeeldHegth / 2 - 100);// Позиция поздравления с победой
    P1= new Player(FeeldWidth, FeeldHegth, sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::D, sf::Keyboard::Key::A,
                   sf::Keyboard::Key::E, sf::Keyboard::Key::Q, Cannonballs, GameClock, cannonSP, wheelsSP, 1, font,
                   sf::Keyboard::Key::LShift);// Создание игрока слева
    P2= new Player(FeeldWidth, FeeldHegth, sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Right,
                   sf::Keyboard::Key::Left, sf::Keyboard::Key::RBracket, sf::Keyboard::Key::LBracket, Cannonballs,
                   GameClock, cannonSP, wheelsSP, 0, font, sf::Keyboard::Key::RShift);// Создание игрока справа
    Wintext.setFont(font);// Шрифт текста победы
    Wintext.setFillColor(sf::Color::Red);// Цвет текста победы
}

Game::~Game(){
    delete(Gwindow);// Выделил почисти
    delete(P1);// Чисти
    delete(P2);// Почисти

}
    void Game::Play() {
    // Ререндер окна под размер из меню, защита от дурака Style::Close == Нефиг менять размер окна выбрал в настройках и не лезь
    delete(Gwindow);
    this->Gwindow=new sf::RenderWindow(sf::VideoMode(FeeldWidth, FeeldHegth), "Balistick Game", sf::Style::Close);
    Gwindow->setPosition(sf::Vector2i(0,0));// Окно в левый верхний угол
    Explosion*explosion;// Указатель на взрыв для их генерации и добавления в вектор взрывов
    sf::Time TM;// Время для его передачи где нужно
    while (Gwindow->isOpen())// Игра в цикле пока окно открыто
    {
        sf::Event event{};
        while (Gwindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)// Если окно закрыли то закрыть его
                Gwindow->close();
                else if(event.type==sf::Event::KeyPressed)
                    if(event.key.code==sf::Keyboard::T) {
                        Theory=!Theory;
                    }
            }
            TM=GameClock.getElapsedTime();// Время с предыдущего цикла
            Gwindow->clear();// Чисти окно потом все перерисуем
            Gwindow->draw(backgroundSP);// Рисуем задник
            // Движение и рисование всех ядер в векторе ядер см. Cannonball.hpp/cpp
            for (auto &i : Cannonballs) {
                i.Move(TM);
                i.Draw(*Gwindow, cannonballSP);
            }
            // Повторная отрисовка или смена спрайта Взрыва см. Explosion.hpp/cpp
            for(auto&i:Explosions)
            {
                i.DrawNextSprite(*Gwindow,GameClock,ExplosionAnime);
            }

            for (unsigned int i = 0; i < Cannonballs.size(); i++) {
                if (Cannonballs[i].deliteqm) {//Если ядро подлежит уничтожению создаем взрыв на его месте и удаляем ядро из вектора
                    explosion=new Explosion(Cannonballs[i],GameClock);
                    Explosions.push_back(*explosion);
                    Cannonballs.erase(Cannonballs.begin() + i);
                }
            }
            //Проверка на столкновение ядра i и j
            for(unsigned int i=0;i<Cannonballs.size();i++)
            {
                for(unsigned int j=i+1;j<Cannonballs.size();j++)
                {
                    if(Cannonballs[i]==Cannonballs[j])
                    {
                        Cannonballs[i].deliteqm=1;
                        Cannonballs[j].deliteqm=1;
                    }
                }
            }
            // Если взрыв подлежит уничтожению то удаляем его из вектора
            for(unsigned int i=0;i<Explosions.size();i++)
                if(Explosions[i].deliteqm)
                {
                   Explosions.erase(Explosions.begin()+i);
                }
            P1->TrytoGetDamage();// Праверка на попадание
            P2->TrytoGetDamage();
            P2->Draw(*Gwindow,Theory);//Отрисовка
            P2->Control();//Управление
            P1->Draw(*Gwindow,Theory);
            P1->Control();
            if(P1->is_deadqm()||P2->is_deadqm())//Если кто-то мертв
            {
                Gwindow->clear();
                Gwindow->draw(backgroundSP);
                if(P1->is_deadqm()&&P2->is_deadqm())
                {
                    WinStream << "DRAW";//Замена текста в поздравлении с победой
                    Wintext.setString(WinStream.str());
                    Gwindow->draw(Wintext);// Вывод поздравления
                }
                else if(P1->is_deadqm())
                {
                    WinStream << "Player   2   Wins!!\n\n\nCongratulations!!";
                    Wintext.setString(WinStream.str());
                    Gwindow->draw(Wintext);
                }
                else if(P2->is_deadqm())
                {
                    WinStream << "Player   1   Wins!!\n\n\nCongratulations!!";
                    Wintext.setString(WinStream.str());
                    Gwindow->draw(Wintext);
                }
                Gwindow->display();//Вывод окна с поздравлением
                #if defined(_WIN32) || defined (_WIN64)
                Sleep(3000);// Успеть прочесть
                #endif
                #if defined(linux) || defined(__linux)
                sleep(3);
                #endif
                Explosions.clear();//Чисти
                Cannonballs.clear();//Чисти
                ExplosionAnime.clear();//Чисти
                delete(Gwindow);//Выделил чисти
                delete(explosion);//Выделил почисти
                //Вернемся в меню оно 1280Х720
                this->Gwindow=new sf::RenderWindow(sf::VideoMode(1280, 720), "Balistick Game", sf::Style::Close);
                Gwindow->setPosition(sf::Vector2i(0,0));//В угол
                return;
            }
            Gwindow->display();//Если никто не победил то выводим окно с ядрами взрывами и игроками
        }
    }

void Game::Menu() {
    this->Gwindow=new sf::RenderWindow(sf::VideoMode(1280, 720), "Balistick Game", sf::Style::Close);
    //Вызывается из мэйна первой тч делаем окно
    //Защита от дурака Style::Close Выбрал в меню разрешение и окно не растягивается
    Gwindow->setPosition(sf::Vector2i(0,0));//Сместит окно в угол
    //Текстуры для меню Play1/2 Setings1/2 Exit1/2 (1-мышка не наведена,2-наведена)
    sf::Texture MenuBack,set1,set2,pl1,pl2,ex1,ex2,read;
    //Загрузка текстур
    MenuBack.loadFromFile("Sprites/Menu/Menuback.png");
    set1.loadFromFile("Sprites/Menu/settings.png");
    set2.loadFromFile("Sprites/Menu/settings1.png");
    pl1.loadFromFile("Sprites/Menu/Play.png");
    pl2.loadFromFile("Sprites/Menu/Play1.png");
    ex1.loadFromFile("Sprites/Menu/Exit.png");
    ex2.loadFromFile("Sprites/Menu/Exit1.png");
    read.loadFromFile("Sprites/Menu/readme.png");
    //Спрайты из тех текстур и их установка, резать не надо так как текстура вся уходит в спрайт
    sf::Sprite BackS,set1S,set2S,pl1S,pl2S,ex1S,ex2S,readS;
    BackS.setTexture(MenuBack);
    set1S.setTexture(set1);
    set2S.setTexture(set2);
    pl1S.setTexture(pl1);
    pl2S.setTexture(pl2);
    ex1S.setTexture(ex1);
    ex2S.setTexture(ex2);
    readS.setTexture(read);
    //Позиционирование и изменение размеров всех тех спрайтов
    BackS.setScale(FeeldWidth / BackS.getGlobalBounds().width, 720 / BackS.getGlobalBounds().height);
    pl1S.setScale(300/pl1S.getGlobalBounds().width,100/pl1S.getGlobalBounds().height);
    pl1S.setPosition(((FeeldWidth - pl1S.getGlobalBounds().width) / 2), ((FeeldHegth - pl1S.getGlobalBounds().height) / 3.3));
    pl2S.setScale(300/pl2S.getGlobalBounds().width,100/pl2S.getGlobalBounds().height);
    pl2S.setPosition(((FeeldWidth - pl2S.getGlobalBounds().width) / 2), ((FeeldHegth - pl2S.getGlobalBounds().height) / 3.3));
    set1S.setScale(250/set1S.getGlobalBounds().width,75/set1S.getGlobalBounds().height);
    set1S.setPosition(((FeeldWidth - set1S.getGlobalBounds().width) / 2), ((FeeldHegth - set1S.getGlobalBounds().height) / 1.8));
    set2S.setScale(250/set2S.getGlobalBounds().width,75/set2S.getGlobalBounds().height);
    set2S.setPosition(((FeeldWidth - set2S.getGlobalBounds().width) / 2), ((FeeldHegth - set2S.getGlobalBounds().height) / 1.8));
    ex2S.setScale(200/ex2S.getGlobalBounds().width,50/ex2S.getGlobalBounds().height);
    ex2S.setPosition(((FeeldWidth - ex2S.getGlobalBounds().width) / 2), ((FeeldHegth - ex2S.getGlobalBounds().height) / 1.3));
    ex1S.setScale(200/ex1S.getGlobalBounds().width,50/ex1S.getGlobalBounds().height);
    ex1S.setPosition(((FeeldWidth - ex1S.getGlobalBounds().width) / 2), ((FeeldHegth - ex1S.getGlobalBounds().height) / 1.3));
    readS.setScale(100/readS.getGlobalBounds().width,25/readS.getGlobalBounds().height);
    readS.setPosition(((FeeldWidth - readS.getGlobalBounds().width) / 2), ((FeeldHegth - readS.getGlobalBounds().height) / 1.2));

    while (Gwindow->isOpen()) {//Пока окно открыто
        sf::Event event{};
        while (Gwindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed)//Если закрыли закрыть окно
                Gwindow->close();
        }
        //Несколько вариации откисовки в зависимости от позиции мышки
        Gwindow->draw(BackS);
        //Если мышка там то
        if (((pl1S.getPosition().x < sf::Mouse::getPosition(*Gwindow).x)&&(sf::Mouse::getPosition(*Gwindow).x<(pl1S.getPosition().x + pl1S.getGlobalBounds().width)))&&((pl1S.getPosition().y < sf::Mouse::getPosition(*Gwindow).y)&&(sf::Mouse::getPosition(*Gwindow).y<(pl1S.getPosition().y + pl1S.getGlobalBounds().height)))) {
            Gwindow->draw(pl2S);
            Gwindow->draw(set1S);
            Gwindow->draw(ex1S);
            Gwindow->draw(readS);
            //Если нажали мынкой
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                this->BUILD();
                this->Play();
            }
        }
        else if (((set1S.getPosition().x < sf::Mouse::getPosition(*Gwindow).x)&&(sf::Mouse::getPosition(*Gwindow).x<(set1S.getPosition().x + set1S.getGlobalBounds().width)))&&((set1S.getPosition().y < sf::Mouse::getPosition(*Gwindow).y)&&(sf::Mouse::getPosition(*Gwindow).y<(set1S.getPosition().y + set1S.getGlobalBounds().height)))) {
            Gwindow->draw(set2S);
            Gwindow->draw(pl1S);
            Gwindow->draw(ex1S);
            Gwindow->draw(readS);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
              this->Settings();
            }
        }
        else if (((ex1S.getPosition().x < sf::Mouse::getPosition(*Gwindow).x)&&(sf::Mouse::getPosition(*Gwindow).x<(ex1S.getPosition().x + ex1S.getGlobalBounds().width)))&&((ex1S.getPosition().y < sf::Mouse::getPosition(*Gwindow).y)&&(sf::Mouse::getPosition(*Gwindow).y<(ex1S.getPosition().y + ex1S.getGlobalBounds().height)))) {
            Gwindow->draw(set1S);
            Gwindow->draw(pl1S);
            Gwindow->draw(ex2S);
            Gwindow->draw(readS);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                Gwindow->close();
                return;
            }
        }
        else if (((readS.getPosition().x < sf::Mouse::getPosition(*Gwindow).x)&&(sf::Mouse::getPosition(*Gwindow).x<(readS.getPosition().x + readS.getGlobalBounds().width)))&&((readS.getPosition().y < sf::Mouse::getPosition(*Gwindow).y)&&(sf::Mouse::getPosition(*Gwindow).y<(readS.getPosition().y + readS.getGlobalBounds().height))))
        {
            Gwindow->draw(set1S);
            Gwindow->draw(pl1S);
            Gwindow->draw(ex1S);
            Gwindow->draw(readS);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                MakeReadme();
                #if defined(linux) || defined(__linux)
                std::system("gedit README.txt");//открытие файла с инструкцией
                #endif         //Попытка в мультиплатформенность
                #if defined(_WIN32) || defined (_WIN64)
                std::system ("start notepad.exe README.txt");
                while(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                      {
                          Sleep(2);
                      }
                #endif
            }
        }
        else {
            Gwindow->draw(set1S);
            Gwindow->draw(pl1S);
            Gwindow->draw(ex1S);
            Gwindow->draw(readS);
        }
        Gwindow->display();// Рисуем все то меню
    }
}

void Game::Settings() {
    Gwindow->clear();// После меню чистим окно для настроек
    delete(Gwindow);
   Gwindow =new sf::RenderWindow(sf::VideoMode(1280, 720), "Balistick Game", sf::Style::Close);
   Gwindow->setPosition(sf::Vector2i(0,0));
    //Текстуры и спрайты для кнопок с разрешением
    //RXXX-мышка не тут
    //RXXX1-мышка тут
    sf::Texture MenuBack, R1080,R10801,R720,R7201,R900,R9001,R540,R5401,Back,Back1;
    sf::Sprite R1080S,R10801S,R720S,R7201S,R900S,R9001S,R540S,R5401S,BackGroundMS,BackS,Back1S;
    //Загрузка текстур для кнопок
    MenuBack.loadFromFile("Sprites/Menu/Menuback.png");
    R1080.loadFromFile("Sprites/Menu/Settings/1920x1080.png");
    R10801.loadFromFile("Sprites/Menu/Settings/1920x10801.png");
    R720.loadFromFile("Sprites/Menu/Settings/1280x720.png");
    R7201.loadFromFile("Sprites/Menu/Settings/1280x7201.png");
    R900.loadFromFile("Sprites/Menu/Settings/1600x900.png");
    R9001.loadFromFile("Sprites/Menu/Settings/1600x9001.png");
    R540.loadFromFile("Sprites/Menu/Settings/960x540.png");
    R5401.loadFromFile("Sprites/Menu/Settings/960x5401.png");
    Back.loadFromFile("Sprites/Menu/Settings/back.png");
    Back1.loadFromFile("Sprites/Menu/Settings/back1.png");
    //Спрайты для кнопок из текстур
    R1080S.setTexture(R1080);
    R540S.setTexture(R540);
    R720S.setTexture(R720);
    R900S.setTexture(R900);
    R10801S.setTexture(R10801);
    R5401S.setTexture(R5401);
    R7201S.setTexture(R7201);
    R9001S.setTexture(R9001);
    BackS.setTexture(Back);
    Back1S.setTexture(Back1);
    BackGroundMS.setTexture(MenuBack);
    //Изменение размера всех кнопок и позиционирование
    BackGroundMS.setScale(1280 / BackGroundMS.getGlobalBounds().width, 720 / BackGroundMS.getGlobalBounds().height);
    R540S.setScale(150/R540S.getGlobalBounds().width,50/R540S.getGlobalBounds().height);
    R540S.setPosition(((1280 - R540S.getGlobalBounds().width) / 2), ((720 - R540S.getGlobalBounds().height) / 3.3));
    R5401S.setScale(150/R5401S.getGlobalBounds().width,50/R5401S.getGlobalBounds().height);
    R5401S.setPosition(((1280 - R5401S.getGlobalBounds().width) / 2), ((720 - R5401S.getGlobalBounds().height) / 3.3));
    R720S.setScale(150/R720S.getGlobalBounds().width,50/R720S.getGlobalBounds().height);
    R720S.setPosition(((1280 - R720S.getGlobalBounds().width) / 2), ((720 - R720S.getGlobalBounds().height) / 2));
    R7201S.setScale(150/R7201S.getGlobalBounds().width,50/R7201S.getGlobalBounds().height);
    R7201S.setPosition(((1280 - R7201S.getGlobalBounds().width) / 2), ((720 - R7201S.getGlobalBounds().height) / 2));
    R900S.setScale(150/R900S.getGlobalBounds().width,50/R900S.getGlobalBounds().height);
    R900S.setPosition(((1280 - R900S.getGlobalBounds().width) / 2), ((720 - R900S.getGlobalBounds().height) / 1.6));
    R9001S.setScale(150/R9001S.getGlobalBounds().width,50/R9001S.getGlobalBounds().height);
    R9001S.setPosition(((1280 - R9001S.getGlobalBounds().width) / 2), ((720 - R9001S.getGlobalBounds().height) / 1.6));
    R1080S.setScale(150/R1080S.getGlobalBounds().width,50/R1080S.getGlobalBounds().height);
    R1080S.setPosition(((1280 - R1080S.getGlobalBounds().width) / 2), ((720 - R1080S.getGlobalBounds().height) / 1.3));
    R10801S.setScale(150/R10801S.getGlobalBounds().width,50/R10801S.getGlobalBounds().height);
    R10801S.setPosition(((1280 - R10801S.getGlobalBounds().width) / 2), ((720 - R10801S.getGlobalBounds().height) / 1.3));
    BackS.setScale(150/BackS.getGlobalBounds().width,50/BackS.getGlobalBounds().height);
    BackS.setPosition(((1280 - BackS.getGlobalBounds().width) / 2), ((720 - BackS.getGlobalBounds().height)));
    Back1S.setScale(150/Back1S.getGlobalBounds().width,50/Back1S.getGlobalBounds().height);
    Back1S.setPosition(((1280 - Back1S.getGlobalBounds().width) / 2), ((720 - Back1S.getGlobalBounds().height)));


    while (Gwindow->isOpen()) {// Пока окно открыто
        sf::Event event{};
        while (Gwindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed)//Закрыли, закрой
                Gwindow->close();
        }
        Gwindow->draw(BackGroundMS);//Задник
        //Варианты отрисовки кнопок от позиции мышки
        //Навел мышку выделили кнопку
        if (((R540S.getPosition().x < sf::Mouse::getPosition(*Gwindow).x) &&
             (sf::Mouse::getPosition(*Gwindow).x < (R540S.getPosition().x + R540S.getGlobalBounds().width))) &&
            ((R540S.getPosition().y < sf::Mouse::getPosition(*Gwindow).y) &&
             (sf::Mouse::getPosition(*Gwindow).y < (R540S.getPosition().y + R540S.getGlobalBounds().height)))) {
            Gwindow->draw(R5401S);
            Gwindow->draw(R720S);
            Gwindow->draw(R900S);
            Gwindow->draw(R1080S);
            Gwindow->draw(BackS);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                FeeldHegth = 540;
                FeeldWidth = 960;
            }
        } else if (((R720S.getPosition().x < sf::Mouse::getPosition(*Gwindow).x) &&
                    (sf::Mouse::getPosition(*Gwindow).x < (R720S.getPosition().x + R720S.getGlobalBounds().width))) &&
                   ((R720S.getPosition().y < sf::Mouse::getPosition(*Gwindow).y) &&
                    (sf::Mouse::getPosition(*Gwindow).y < (R720S.getPosition().y + R720S.getGlobalBounds().height)))) {
            Gwindow->draw(R540S);
            Gwindow->draw(R7201S);
            Gwindow->draw(R900S);
            Gwindow->draw(R1080S);
            Gwindow->draw(BackS);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                FeeldHegth = 720;
                FeeldWidth = 1280;
            }
        } else if (((R900S.getPosition().x < sf::Mouse::getPosition(*Gwindow).x) &&
                    (sf::Mouse::getPosition(*Gwindow).x < (R900S.getPosition().x + R900S.getGlobalBounds().width))) &&
                   ((R900S.getPosition().y < sf::Mouse::getPosition(*Gwindow).y) &&
                    (sf::Mouse::getPosition(*Gwindow).y < (R900S.getPosition().y + R900S.getGlobalBounds().height)))) {
            Gwindow->draw(R540S);
            Gwindow->draw(R720S);
            Gwindow->draw(R9001S);
            Gwindow->draw(R1080S);
            Gwindow->draw(BackS);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                FeeldHegth = 900;
                FeeldWidth = 1600;
            }
        }
        else if (((R1080S.getPosition().x < sf::Mouse::getPosition(*Gwindow).x) &&
                    (sf::Mouse::getPosition(*Gwindow).x < (R1080S.getPosition().x + R1080S.getGlobalBounds().width))) &&
                   ((R1080S.getPosition().y < sf::Mouse::getPosition(*Gwindow).y) &&
                    (sf::Mouse::getPosition(*Gwindow).y <
                     (R1080S.getPosition().y + R1080S.getGlobalBounds().height)))) {
            Gwindow->draw(R540S);
            Gwindow->draw(R720S);
            Gwindow->draw(R900S);
            Gwindow->draw(R10801S);
            Gwindow->draw(BackS);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                FeeldHegth = 1080;
                FeeldWidth = 1929;
            }
        }
        else if (((BackS.getPosition().x < sf::Mouse::getPosition(*Gwindow).x) &&
                  (sf::Mouse::getPosition(*Gwindow).x < (BackS.getPosition().x + BackS.getGlobalBounds().width))) &&
                 ((BackS.getPosition().y < sf::Mouse::getPosition(*Gwindow).y) &&
                  (sf::Mouse::getPosition(*Gwindow).y <
                   (BackS.getPosition().y + BackS.getGlobalBounds().height)))) {
            Gwindow->draw(R540S);
            Gwindow->draw(R720S);
            Gwindow->draw(R900S);
            Gwindow->draw(R1080S);
            Gwindow->draw(Back1S);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                return;
            }
        }
        else {
            Gwindow->draw(R540S);
            Gwindow->draw(R720S);
            Gwindow->draw(R900S);
            Gwindow->draw(R1080S);
            Gwindow->draw(BackS);
        }
        Gwindow->display();//Рисуем эту котовасию
    }
}

void Game::MakeReadme() {
    remove("README.txt");
    std::ofstream outfile ("README.txt");

    std::stringstream ss;
    ss<<"Control:\n    Left:\n    1)Mas+     E\n    2)Mas-     Q\n    3)Angle+   W\n    4)Angle-   S\n    5)Energy+  D\n    6)Energy-  A\n    7)Shoot    LShift\n"
    <<"Rigth:\n    1)Mas+     ]\n    2)Mas-     [\n    3)Angle+   UP\n    4)Angle-   Down\n    5)Energy+  Rigth\n    6)Energy-  Left\n    7)Shoot    RShift\nTheoryLines    T";

    outfile << ss.str();
    outfile.close();
}

