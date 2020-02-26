//
// Created by anorak on 12.11.2019.
//
#include "Explosion.hpp"
    class Game {
    public:
        Game();// Конструктор по умолчанию
        void  Menu();// Меню
        ~Game();// Деструктор
    private:
        void MakeReadme();//Защита от дурака удаляет и создает файл с инструкцией
        std::vector<Cannonball>Cannonballs;// Вектор ядер ибо их много
        std::vector<Explosion>Explosions;// Вектор взрывов их тоже много
        std::vector<sf::Sprite>ExplosionAnime;// Анимация взрыва, здесь чтобы не хранить кучу в куче взрывов, а передовать по ссылке
        sf::RenderWindow *Gwindow;// Указатель на окно чтобы его ререндорить + так его легче хранить
        float FeeldWidth,FeeldHegth;// Размеры поля
        Player *P1,*P2;// Указатели на игроков
        sf::Clock GameClock;// Внутреигровые часы
        sf::Sprite cannonSP,cannonballSP,backgroundSP,wheelsSP;// Спрайты пушки,ядра,задник и колеса
        sf::Texture TextureCanBallWheel;// Текстура для спрайтов выше
        sf::Texture ExplosionT;// Текстура со взрывами
        sf::Texture BackGround;// Текстура задника
        bool Theory;// Рисует или нет теоретическую кривую
        sf::Font font;// Шрифт
        std::ostringstream WinStream;// Строка поздравления с победой
        sf::Text Wintext;// Текст этой строки для вывода
        void BUILD();// Настройка перед началом игры
        void Play();// Циклическая функция игры
        void Settings();// Настройки(изменение разрешения == размер поля)
};