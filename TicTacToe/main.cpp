#include <iostream>
#include <vector>

class Game {
public:
    Game(size_t size, bool isPlayersFirstStep) :
    size_(size),
    fieldSize_(size * size),
    currentState_(0, NONE, isPlayersFirstStep)
    {}

    void printField() const noexcept;

    void makeStep(size_t x, size_t y);

private:
    enum GameType {WIN, LOOSE, NONE};

    struct GameState {
        size_t potential_;
        GameType result_;
        bool isPlayerStep_;
        std::vector<char> field_;

        GameState(size_t potential, GameType result, bool isPlayerStep) :
        potential_(potential),
        result_(result),
        isPlayerStep_(isPlayerStep),
        field_(fieldSize_, 0)
        {}
    };

    size_t size_;
    size_t fieldSize_;
    GameState currentState_;

    
};

int main() {

    return 0;
}