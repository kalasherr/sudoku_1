#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>

const int CELL_SIZE = 50;
const sf::Color LightBlue(135, 200, 250);
const sf::Color LightGray(200, 200, 200);
class Cell {
public:
    sf::Color color;
    int number;
    bool filled;
    bool modifiable;

    Cell() : color(sf::Color::White), number(0), filled(false), modifiable(true) {}

    Cell(sf::Color color, int number, bool filled, bool modifiable)
        : color(color), number(number), filled(filled), modifiable(modifiable) {}
};

const int ROWS = 9;
const int COLS = 9;

int solvedSudoku[ROWS][COLS] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};
void swapRows(Cell grid1[ROWS],Cell grid2[ROWS])
{
    Cell buffer[9];
    for (int i = 0; i < 9; ++i) {
        buffer[i] = grid1[i];
        grid1[i] = grid2[i];
        grid2[i] = buffer[i];
    }
}
void swapCols(Cell grid1[ROWS],Cell grid2[ROWS])
{
    Cell buffer[9];
    for (int i = 0; i < 9; ++i) {
        buffer[i] = grid1[i];
        grid1[i] = grid2[i];
        grid2[i] = buffer[i];
    }
}
void shuffleGrid(Cell grid[ROWS][COLS]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    std::uniform_int_distribution<> dis1(1, 2);
    int randomNumber = dis(gen);
    int buffer;
    for (int i = 0; i<9; i++)
    {
        for (int j = 0; j<9; j++)
        {
            randomNumber = dis(gen);
            if (randomNumber>5)
            {
                grid[i][j].number = solvedSudoku[i][j];
                grid[i][j].color = LightGray;
                grid[i][j].modifiable = false;
            }
        }
    }
    for (int i = 0; i<9; i+=3)
    {
        if (i!=8 && (randomNumber > 4))
        {
            swapRows(grid[i],grid[i+1]);
            swapRows(grid[i+2],grid[i+1]);
        }
        else if (i!=8 && (randomNumber >2))
        {
            swapRows(grid[i],grid[i+2]);
        }
    }
    if (randomNumber > 4)
        {
            swapRows(grid[0],grid[3]);
            swapRows(grid[1],grid[4]);
            swapRows(grid[2],grid[5]);
        }
    
}
int main() {
    sf::RenderWindow window(sf::VideoMode(400, 200), "Menu");

    sf::Font font;
    font.loadFromFile("better-vcr-5.4.ttf");

    sf::Text startText("Start", font, 24);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(155, 100);

    sf::RectangleShape startButton(sf::Vector2f(100, 40));
    startButton.setFillColor(sf::Color::Blue);
    startButton.setPosition(150, 100);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    if (startButton.getGlobalBounds().contains(mousePosF)) {
                        window.close();
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(startButton);
        window.draw(startText);
        window.display();
    }

    Cell grid[9][9];
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            grid[i][j].modifiable = true;
        }
    }

    shuffleGrid(grid);
    
    sf::RenderWindow gridWindow(sf::VideoMode(CELL_SIZE * 9, CELL_SIZE * 9), "Kalash Sudoku");

    int selectedRow = -1;
    int selectedCol = -1;
    std::string currentInput;

    while (gridWindow.isOpen()) {
        sf::Event event;
        while (gridWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gridWindow.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(gridWindow);
                    int row = mousePos.y / CELL_SIZE;
                    int col = mousePos.x / CELL_SIZE;

                    if (row >= 0 && row < 9 && col >= 0 && col < 9 && grid[row][col].modifiable) {
                        if (selectedRow != -1 && selectedCol != -1) {
                            grid[selectedRow][selectedCol].color = sf::Color::White;
                        }
                        selectedRow = row;
                        selectedCol = col;
                        grid[selectedRow][selectedCol].color = LightBlue;
                        currentInput.clear();
                    }
                }
            }
            if (event.type == sf::Event::TextEntered && selectedRow != -1 && selectedCol != -1) {
                if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    currentInput += static_cast<char>(event.text.unicode);
                    std::stringstream ss(currentInput);
                    int number;
                    ss >> number;
                    if (number >= 0 && number <= 9 && grid[selectedRow][selectedCol].modifiable) {
                        grid[selectedRow][selectedCol].number = number;
                        
                    }
                    for (int i = 0; i<9; i++)
                        {
                            if (i!=selectedRow && grid[selectedRow][selectedCol].number == grid[i][selectedCol].number)
                            {
                                grid[selectedRow][selectedCol].color = sf::Color::Magenta;
                            }
                            if (i!=selectedCol && grid[selectedRow][selectedCol].number == grid[selectedRow][i].number)
                            {
                                grid[selectedRow][selectedCol].color = sf::Color::Magenta;
                            }
                        }
                    if (grid[selectedRow][selectedCol].color != sf::Color::Magenta)
                        {
                            grid[selectedRow][selectedCol].color = sf::Color::White;
                        }
                    
                    
            
                    selectedRow = -1;
                    selectedCol = -1;
                } else if (event.text.unicode == 8 && !currentInput.empty()) {
                    currentInput.pop_back();
                    std::stringstream ss(currentInput);
                    int number;
                    if (ss >> number) {
                        grid[selectedRow][selectedCol].number = number;
                    } else {
                        grid[selectedRow][selectedCol].number = 0;
                    }
                }
            }
        }

        gridWindow.clear();

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
              sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                rectangle.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                rectangle.setOutlineThickness(1);
                rectangle.setOutlineColor(sf::Color::Black);
                rectangle.setFillColor(grid[i][j].color);

                sf::Text text;
                text.setFont(font);
                text.setString(grid[i][j].number != 0 ? std::to_string(grid[i][j].number) : "");
                text.setCharacterSize(20);
                text.setFillColor(sf::Color::Black);
                text.setPosition(rectangle.getPosition().x + 0.35f * CELL_SIZE, rectangle.getPosition().y + 0.35f * CELL_SIZE);

                gridWindow.draw(rectangle);
                gridWindow.draw(text);
            }
        }
        sf::RectangleShape thickLine;
        thickLine.setFillColor(sf::Color::Black);

        for (int i = 1; i < 3; ++i) {
            thickLine.setSize(sf::Vector2f(CELL_SIZE * 9, 3));
            thickLine.setPosition(0, i * 3 * CELL_SIZE - 1);
            gridWindow.draw(thickLine);
        }

        for (int j = 1; j < 3; ++j) {
            thickLine.setSize(sf::Vector2f(3, CELL_SIZE * 9));
            thickLine.setPosition(j * 3 * CELL_SIZE - 1, 0);
            gridWindow.draw(thickLine);
        }

        gridWindow.display();
    }

    return 0;
}