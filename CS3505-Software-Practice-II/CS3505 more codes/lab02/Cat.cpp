#include <iostream>

using namespace std;

class Cat{
    private:
        int lives;
    public:
        Cat(int initialLives) {
            lives = initialLives;
         }

        void reportLives() const{
            cout << "The cat has " << lives << " lives." << endl;
        }

        void loseLife(){
            if(lives > 0){
                lives--;
                cout << "Cat lost a life. Remaining lives: " << lives << endl;
            }else{
                cout << "The cat has no more lives left." << endl;
            }
        }
};

int main() {
    // Cat myCat(9);

    // myCat.reportLives();

    // myCat.loseLife();
    // myCat.loseLife();

    // myCat.reportLives();
    std::cout << "Hello, World!" << std::endl;

    return 0;
}