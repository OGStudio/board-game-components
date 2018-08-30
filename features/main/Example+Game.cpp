FEATURE main.h/Setup
this->setupGame();

FEATURE main.h/TearDown
this->tearGameDown();

FEATURE main.h/Impl
private:
    mahjong::Solitaire *game;

    void setupGame()
    {
        this->game = new mahjong::Solitaire();
    }
    void tearGameDown()
    {
        delete this->game;
    }
