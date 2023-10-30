#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#define grid_width 160
#define grid_height 120

bool grid[grid_width][grid_height] = { 0 };
bool nextGrid[grid_width][grid_height] = { 0 };
float framerate = 30.0f; // Vitesse initiale (30 FPS)

void updateGameOfLife() {
    // Loop through each cell in the grid
   
    for (int x = 0; x < grid_width; x++) {
        for (int y = 0; y < grid_height; y++) {
            int liveNeighbors = 0;

            // Count the number of live neighbors
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < grid_width && ny >= 0 && ny < grid_height && grid[nx][ny]) {
                        liveNeighbors++;
                    }
                }
            }

            // Apply Conway's rules
            if (grid[x][y]) {
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    nextGrid[x][y] = false;  // Cell dies
                }
                else {
                    nextGrid[x][y] = true;   // Cell survives
                }
            }
            else {
                if (liveNeighbors == 3) {
                    nextGrid[x][y] = true;   // New cell is born
                }
                else {
                    nextGrid[x][y] = false;
                }
            }
        }
       
    }
   

    std::swap(grid, nextGrid);
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game of Life", sf::Style::Default, settings);
    

    window.setFramerateLimit(framerate);

    sf::RectangleShape tile_shape(sf::Vector2f(5, 5));
    tile_shape.setFillColor(sf::Color::Green);

    bool isSimulationRunning = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if (!isSimulationRunning) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int x = mousePos.x / 5;
                    int y = mousePos.y / 5;

                    if (event.mouseButton.button == sf::Mouse::Left) {
                        grid[x][y] = true; // Set the cell as alive
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        grid[x][y] = false; // Set the cell as dead
                    }
                }
            }

            if (event.key.code == sf::Keyboard::Space) {
                isSimulationRunning = !isSimulationRunning;

                if (isSimulationRunning) {
                    framerate = 2.0f;
                }
                else {
                    framerate = 30.0f;
                }

                window.setFramerateLimit(static_cast<unsigned int>(framerate));
            }

        }
        static int a = 0; // Déplacez cette variable statique à l'extérieur de la boucle pour garder la trace du nombre d'itérations

        if (isSimulationRunning) {
            updateGameOfLife();
            a++;
        }

        window.clear(sf::Color::Black);

        for (int x = 0; x < grid_width; x++) {
            for (int y = 0; y < grid_height; y++) {
                if (grid[x][y]) {
                    tile_shape.setPosition(5 * x, 5 * y);
                    window.draw(tile_shape);
                }
            }
        }
        
        
        std::cout << "nombre iterations : " << a << std::endl;
      
        window.display();
    }

    return 0;
}
