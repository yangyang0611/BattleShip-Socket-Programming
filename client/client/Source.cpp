// #include <iostream>
// #include <vector>
// #include <cstring>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <limits>
// #include <sstream>

// #define PORT 8080
// #define GRID_SIZE 10

// struct Ship {
//     int startX, startY, endX, endY;
// };

// void initializeGrid(std::vector<std::vector<char>>& grid) {
//     for (int i = 0; i < GRID_SIZE; ++i) {
//         for (int j = 0; j < GRID_SIZE; ++j) {
//             grid[i][j] = '.';
//         }
//     }
// }

// bool canPlaceShip(const std::vector<std::vector<char>>& grid, const Ship& ship) {
//     if (ship.startX == ship.endX) {
//         int startY = std::min(ship.startY, ship.endY);
//         int endY = std::max(ship.startY, ship.endY);
//         if (endY >= GRID_SIZE) return false;
//         for (int i = startY; i <= endY; ++i) {
//             if (grid[ship.startX][i] == 'S') return false; // Check for overlap
//         }
//     } else if (ship.startY == ship.endY) {
//         int startX = std::min(ship.startX, ship.endX);
//         int endX = std::max(ship.startX, ship.endX);
//         if (endX >= GRID_SIZE) return false;
//         for (int i = startX; i <= endX; ++i) {
//             if (grid[i][ship.startY] == 'S') return false; // Check for overlap
//         }
//     } else {
//         return false; // Ships must be placed in a straight line
//     }
//     return true;
// }

// void placeShip(std::vector<std::vector<char>>& grid, const Ship& ship) {
//     if (ship.startX == ship.endX) {
//         int startY = std::min(ship.startY, ship.endY);
//         int endY = std::max(ship.startY, ship.endY);
//         for (int i = startY; i <= endY; ++i) {
//             grid[ship.startX][i] = 'S';
//         }
//     } else {
//         int startX = std::min(ship.startX, ship.endX);
//         int endX = std::max(ship.startX, ship.endX);
//         for (int i = startX; i <= endX; ++i) {
//             grid[i][ship.startY] = 'S';
//         }
//     }
// }

// void printGrid(const std::vector<std::vector<char>>& grid) {
//     for (int i = 0; i < GRID_SIZE; ++i) {
//         for (int j = 0; j < GRID_SIZE; ++j) {
//             std::cout << grid[i][j] << ' ';
//         }
//         std::cout << std::endl;
//     }
// }

// int main() {
//     int sock = 0;
//     struct sockaddr_in serv_addr;
//     char buffer[1024] = {0};

//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         std::cerr << "Socket creation error" << std::endl;
//         return -1;
//     }

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//         std::cerr << "Invalid address/ Address not supported" << std::endl;
//         return -1;
//     }

//     if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//         std::cerr << "Connection failed" << std::endl;
//         return -1;
//     }

//     std::vector<std::vector<char>> playerGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
//     std::vector<std::vector<char>> attackGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
//     initializeGrid(playerGrid);
//     initializeGrid(attackGrid);

//     // Place ships
//     int shipSizes[] = {5, 4, 3, 3, 2};
//     for (int size : shipSizes) {
//         Ship ship;
//         bool placed = false;
//         while (!placed) {
//             std::cout << "Enter the start and end coordinates for a ship of size " << size << " (format: startX startY endX endY): ";
//             std::cin >> ship.startX >> ship.startY >> ship.endX >> ship.endY;
//             if ((ship.startX == ship.endX && abs(ship.endY - ship.startY) + 1 == size) ||
//                 (ship.startY == ship.endY && abs(ship.endX - ship.startX) + 1 == size)) {
//                 if (canPlaceShip(playerGrid, ship)) {
//                     placeShip(playerGrid, ship);
//                     placed = true;
//                 } else {
//                     std::cout << "Invalid placement, overlapping or out of bounds. Try again.\n";
//                 }
//             } else {
//                 std::cout << "Invalid ship size or not in a straight line. Try again.\n";
//             }
//         }
//     }

//     std::cout << "My ship placement:" << std::endl;
//     printGrid(playerGrid);

//     bool gameOver = false;

//     while (!gameOver) {
//         int x, y;
//         std::cout << "Enter coordinates to attack (x y): ";
//         std::cin >> x >> y;

//         // Ensure coordinates are within bounds
//         if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
//             std::cerr << "Coordinates out of bounds. Please enter valid coordinates." << std::endl;
//             continue;
//         }

//         sprintf(buffer, "%d %d", x, y);
//         send(sock, buffer, strlen(buffer), 0);
//         std::cout << "Sent attack coordinates: " << buffer << std::endl;

//         int bytesRead = read(sock, buffer, 1024);
//         if (bytesRead <= 0) {
//             std::cerr << "Read error or connection closed by server" << std::endl;
//             break;
//         }
//         buffer[bytesRead] = '\0';
//         std::string response(buffer);
//         std::cout << "Received response: " << response << std::endl;

//         std::istringstream responseStream(response);
//         std::string line;
//         while (std::getline(responseStream, line)) {
//             if (line.find("Game Over") != std::string::npos) {
//                 gameOver = true;
//                 std::cout << line << std::endl;
//                 break;
//             } else if (line.find("Hit") != std::string::npos) {
//                 attackGrid[x][y] = 'X';
//                 std::cout << "Hit!" << std::endl;
//             } else if (line.find("Miss") != std::string::npos) {
//                 attackGrid[x][y] = 'O';
//                 std::cout << "Miss!" << std::endl;
//             } else {
//                 int a, b;
//                 sscanf(line.c_str(), "%d %d", &a, &b);
//                 if (a < 0 || a >= GRID_SIZE || b < 0 || b >= GRID_SIZE) {
//                     std::cerr << "Received out of bounds coordinates from server." << std::endl;
//                     continue;
//                 }
//                 if (line.find("Hit") != std::string::npos) {
//                     playerGrid[a][b] = 'H';
//                     std::cout << "Server attack at (" << a << ", " << b << ") and Hit!" << std::endl;
//                 } else if (line.find("Miss") != std::string::npos) {
//                     playerGrid[a][b] = 'M';
//                     std::cout << "Server attack at (" << a << ", " << b << ") and Miss!" << std::endl;
//                 }
//             }
//         }

//         std::cout << "My attack grid:" << std::endl;
//         printGrid(attackGrid);

//         std::cout << "My ship placement after computer's attack:" << std::endl;
//         printGrid(playerGrid);
//     }

//     close(sock);
//     return 0;
// }

#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits>
#include <sstream>

#define PORT 8080
#define GRID_SIZE 10

struct Ship {
    int startX, startY, endX, endY;
};

void initializeGrid(std::vector<std::vector<char>>& grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = '.';
        }
    }
}

bool canPlaceShip(const std::vector<std::vector<char>>& grid, const Ship& ship) {
    if (ship.startX == ship.endX) {
        int startY = std::min(ship.startY, ship.endY);
        int endY = std::max(ship.startY, ship.endY);
        if (endY >= GRID_SIZE) return false;
        for (int i = startY; i <= endY; ++i) {
            if (grid[ship.startX][i] == 'S') return false; // Check for overlap
        }
    } else if (ship.startY == ship.endY) {
        int startX = std::min(ship.startX, ship.endX);
        int endX = std::max(ship.startX, ship.endX);
        if (endX >= GRID_SIZE) return false;
        for (int i = startX; i <= endX; ++i) {
            if (grid[i][ship.startY] == 'S') return false; // Check for overlap
        }
    } else {
        return false; // Ships must be placed in a straight line
    }
    return true;
}

bool isGameOver(const std::vector<std::vector<char>>& grid) {
    int hitCount = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == 'H') {
                ++hitCount;
            }
        }
    }
    return hitCount >= 17;
}

void placeShip(std::vector<std::vector<char>>& grid, const Ship& ship) {
    if (ship.startX == ship.endX) {
        int startY = std::min(ship.startY, ship.endY);
        int endY = std::max(ship.startY, ship.endY);
        for (int i = startY; i <= endY; ++i) {
            grid[ship.startX][i] = 'S';
        }
    } else {
        int startX = std::min(ship.startX, ship.endX);
        int endX = std::max(ship.startX, ship.endX);
        for (int i = startX; i <= endX; ++i) {
            grid[i][ship.startY] = 'S';
        }
    }
}

void printGrid(const std::vector<std::vector<char>>& grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            std::cout << grid[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

void sendMessage(int socket, const char* message) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s\n", message);
    send(socket, buffer, strlen(buffer), 0);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::vector<std::vector<char>> playerGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
    std::vector<std::vector<char>> attackGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
    initializeGrid(playerGrid);
    initializeGrid(attackGrid);

    // Place ships
    int shipSizes[] = {5, 4, 3, 3, 2};
    for (int size : shipSizes) {
        Ship ship;
        bool placed = false;
        while (!placed) {
            std::cout << "Enter the start and end coordinates for a ship of size " << size << " (format: startX startY endX endY): ";
            std::cin >> ship.startX >> ship.startY >> ship.endX >> ship.endY;
            if ((ship.startX == ship.endX && abs(ship.endY - ship.startY) + 1 == size) ||
                (ship.startY == ship.endY && abs(ship.endX - ship.startX) + 1 == size)) {
                if (canPlaceShip(playerGrid, ship)) {
                    placeShip(playerGrid, ship);
                    placed = true;
                } else {
                    std::cout << "Invalid placement, overlapping or out of bounds. Try again.\n";
                }
            } else {
                std::cout << "Invalid ship size or not in a straight line. Try again.\n";
            }
        }
    }

    std::cout << "My ship placement:" << std::endl;
    printGrid(playerGrid);

    bool gameOver = false;

    while (!gameOver) {
        // 1. start attacking to server
        int x, y;
        std::cout << "Enter coordinates to attack (x y): ";
        std::cin >> x >> y;

        // Ensure coordinates are within bounds
        if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
            std::cerr << "Coordinates out of bounds. Please enter valid coordinates." << std::endl;
            continue;
        }

        memset(buffer, 0, 1024);
        sprintf(buffer, "%d %d", x, y);
        send(sock, buffer, 1024, 0);
        std::cout << "Sent attack coordinates: " << buffer << std::endl;

        // 2. receive from server if we've missed or hit
        int bytesRead = read(sock, buffer, 1024);
        if (bytesRead <= 0) {
            std::cerr << "Read error or connection closed by server" << std::endl;
            break;
        }
        buffer[bytesRead] = '\0';
        std::string response(buffer);
        std::cout << "Received response: " << response << std::endl;

        std::istringstream responseStream(response);
        std::string line;
        // std::cout << "LINE IS: " << line << std::endl;
        while (std::getline(responseStream, line)) {
            if (line.find("Game Over") == 0) {
                gameOver = true;
                std::cout << "We Win" << std::endl;
                break;
            } else if (line.find("Hit") == 0) {
                attackGrid[x][y] = 'X';
                std::cout << "Hit!" << std::endl;
            } else if (line.find("Miss") == 0) {
                attackGrid[x][y] = 'O';
                std::cout << "Miss!" << std::endl;
            } 
        }

        if (gameOver)
            break;

        std::cout << "My attack grid:" << std::endl;
        printGrid(attackGrid);

        // 3. receive attack from server
        //memset(buffer, 0, 1024);

        
        bytesRead = read(sock, buffer, 1024);
        if (bytesRead <= 0) {
            std::cerr << "Read error or connection closed by client" << std::endl;
            break;
        }

        std::cout << "bytesRead = " << bytesRead << std::endl;

        buffer[bytesRead] = '\0';
        std::cout << "Received data from server: " << buffer << std::endl;

        sscanf(buffer, "%d %d", &x, &y);

        std::cout << "x = " << x << ", y = " << y << std::endl;


        // 4. tell server if it's missed or hit
        if (playerGrid[x][y] == 'S' || playerGrid[x][y] == 'H') {
            playerGrid[x][y] = 'H';
            if (isGameOver(playerGrid)) {
                sendMessage(sock, "Game Over");
                std::cout << "We lose" << std::endl;
                gameOver = true;
            } else {
                sendMessage(sock, "Hit");
            }
        } else {
            playerGrid[x][y] = 'M';
            sendMessage(sock, "Miss");
        }

        std::cout << "My ship placement after computer's attack:" << std::endl;
        printGrid(playerGrid);
    }

    close(sock);
    return 0;
}



// server         client
//              <-  0,0
//   Miss        -> 

//  1,2        ->  ??
//   ??          <-   Miss