// #include <iostream>
// #include <vector>
// #include <cstring>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <cstdlib>
// #include <ctime>

// #define PORT 8080
// #define GRID_SIZE 10

// struct Ship {
//     int size;
//     bool isVertical;
//     int startX, startY;
// };

// void initializeGrid(std::vector<std::vector<char>>& grid) {
//     for (int i = 0; i < GRID_SIZE; ++i) {
//         for (int j = 0; j < GRID_SIZE; ++j) {
//             grid[i][j] = '.';
//         }
//     }
// }

// bool canPlaceShip(const std::vector<std::vector<char>>& grid, const Ship& ship) {
//     if (ship.isVertical) {
//         if (ship.startX + ship.size > GRID_SIZE) return false;
//         for (int i = 0; i < ship.size; ++i) {
//             if (grid[ship.startX + i][ship.startY] == 'S') return false;
//         }
//     } else {
//         if (ship.startY + ship.size > GRID_SIZE) return false;
//         for (int i = 0; i < ship.size; ++i) {
//             if (grid[ship.startX][ship.startY + i] == 'S') return false;
//         }
//     }
//     return true;
// }

// void placeShip(std::vector<std::vector<char>>& grid, const Ship& ship) {
//     if (ship.isVertical) {
//         for (int i = 0; i < ship.size; ++i) {
//             grid[ship.startX + i][ship.startY] = 'S';
//         }
//     } else {
//         for (int i = 0; i < ship.size; ++i) {
//             grid[ship.startX][ship.startY + i] = 'S';
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

// bool isGameOver(const std::vector<std::vector<char>>& grid) {
//     int hitCount = 0;
//     for (int i = 0; i < GRID_SIZE; ++i) {
//         for (int j = 0; j < GRID_SIZE; ++j) {
//             if (grid[i][j] == 'H') {
//                 ++hitCount;
//             }
//         }
//     }
//     return hitCount >= 17;
// }

// void randomlyPlaceShips(std::vector<std::vector<char>>& grid) {
//     std::vector<int> shipSizes = {5, 4, 3, 3, 2};
//     srand(time(0));

//     for (int size : shipSizes) {
//         Ship ship;
//         ship.size = size;
//         bool placed = false;

//         while (!placed) {
//             ship.isVertical = rand() % 2;
//             ship.startX = rand() % GRID_SIZE;
//             ship.startY = rand() % GRID_SIZE;

//             if (canPlaceShip(grid, ship)) {
//                 placeShip(grid, ship);
//                 placed = true;
//             }
//         }
//     }
// }

// void computerAttack(std::vector<std::vector<char>>& playerGrid, std::vector<std::vector<char>>& computerAttackGrid, int& x, int& y) {
//     bool validAttack = false;
//     while (!validAttack) {
//         x = rand() % GRID_SIZE;
//         y = rand() % GRID_SIZE;
//         if (computerAttackGrid[x][y] == '.') {
//             validAttack = true;
//             if (playerGrid[x][y] == 'S') {
//                 playerGrid[x][y] = 'H';
//                 computerAttackGrid[x][y] = 'X';
//                 std::cout << "Server attack at (" << x << ", " << y << ") and Hit!" << std::endl;
//             } else {
//                 playerGrid[x][y] = 'M';
//                 computerAttackGrid[x][y] = 'O';
//                 std::cout << "Server attack at (" << x << ", " << y << ") and Miss!" << std::endl;
//             }
//         }
//     }
// }

// 	// Add a newline at the end of each message sent to the client
// 	void sendMessage(int socket, const char* message) {
// 		char buffer[1024];
// 		snprintf(buffer, sizeof(buffer), "%s\n", message);
// 		send(socket, buffer, strlen(buffer), 0);
// 	}

// int main() {
//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     int opt = 1;
//     int addrlen = sizeof(address);

//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
//         perror("setsockopt");
//         exit(EXIT_FAILURE);
//     }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     if (listen(server_fd, 3) < 0) {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }

//     if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
//         perror("accept");
//         exit(EXIT_FAILURE);
//     }

//     std::vector<std::vector<char>> playerGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
//     std::vector<std::vector<char>> computerGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
//     std::vector<std::vector<char>> computerAttackGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
//     initializeGrid(playerGrid);
//     initializeGrid(computerGrid);
//     initializeGrid(computerAttackGrid);

//     // Computer places ships randomly
//     randomlyPlaceShips(computerGrid);

//     // Display computer's ship placement
//     std::cout << "Computer's ship placement:" << std::endl;
//     printGrid(computerGrid);



//     // Game loop
//     while (true) {
//     // Player's attack
//     char buffer[1024] = {0};
//     int bytesRead = read(new_socket, buffer, 1024);
//     if (bytesRead <= 0) {
//         std::cerr << "Read error or connection closed by client" << std::endl;
//         break;
//     }
//     buffer[bytesRead] = '\0';
//     std::cout << "Received data from user: " << buffer << std::endl;

//     int x, y;
//     sscanf(buffer, "%d %d", &x, &y);

//     // Ensure coordinates are within bounds
//     if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
//         std::cerr << "Received out of bounds coordinates from client: (" << x << ", " << y << ")" << std::endl;
//         sendMessage(new_socket, "Coordinates out of bounds");
//         continue;
//     }

//     if (computerGrid[x][y] == 'S') {
//         computerGrid[x][y] = 'H';
//         sendMessage(new_socket, "Hit");
//     } else {
//         computerGrid[x][y] = 'M';
//         sendMessage(new_socket, "Miss");
//     }

//     std::cout << "Computer's ship placement after player's attack:" << std::endl;
//     printGrid(computerGrid);

//     if (isGameOver(computerGrid)) {
//         sendMessage(new_socket, "Game Over, you win!");
//         break;
//     }

//     // Computer's attack
//     computerAttack(playerGrid, computerAttackGrid, x, y);
//     if (playerGrid[x][y] == 'H') {
//         sendMessage(new_socket, "Hit");
//         char coordBuffer[20];
//         sprintf(coordBuffer, "%d %d", x, y);
//         sendMessage(new_socket, coordBuffer);
//     } else {
//         sendMessage(new_socket, "Miss");
//         char coordBuffer[20];
//         sprintf(coordBuffer, "%d %d", x, y);
//         sendMessage(new_socket, coordBuffer);
//     }

//     std::cout << "Computer's attack grid:" << std::endl;
//     printGrid(computerAttackGrid);

//     if (isGameOver(playerGrid)) {
//         sendMessage(new_socket, "Game Over, you lose!");
//         break;
//     }
// }

        

//     close(new_socket);
//     close(server_fd);
//     return 0;
// }

#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <sstream>

#define PORT 8080
#define GRID_SIZE 10

struct Ship {
    int size;
    bool isVertical;
    int startX, startY;
};

void initializeGrid(std::vector<std::vector<char>>& grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = '.';
        }
    }
}

bool canPlaceShip(const std::vector<std::vector<char>>& grid, const Ship& ship) {
    if (ship.isVertical) {
        if (ship.startX + ship.size > GRID_SIZE) return false;
        for (int i = 0; i < ship.size; ++i) {
            if (grid[ship.startX + i][ship.startY] == 'S') return false;
        }
    } else {
        if (ship.startY + ship.size > GRID_SIZE) return false;
        for (int i = 0; i < ship.size; ++i) {
            if (grid[ship.startX][ship.startY + i] == 'S') return false;
        }
    }
    return true;
}

void placeShip(std::vector<std::vector<char>>& grid, const Ship& ship) {
    if (ship.isVertical) {
        for (int i = 0; i < ship.size; ++i) {
            grid[ship.startX + i][ship.startY] = 'S';
        }
    } else {
        for (int i = 0; i < ship.size; ++i) {
            grid[ship.startX][ship.startY + i] = 'S';
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

void randomlyPlaceShips(std::vector<std::vector<char>>& grid) {
    std::vector<int> shipSizes = {5, 4, 3, 3, 2};
    srand(time(0));

    for (int size : shipSizes) {
        Ship ship;
        ship.size = size;
        bool placed = false;

        while (!placed) {
            ship.isVertical = rand() % 2;
            ship.startX = rand() % GRID_SIZE;
            ship.startY = rand() % GRID_SIZE;

            if (canPlaceShip(grid, ship)) {
                placeShip(grid, ship);
                placed = true;
            }
        }
    }
}

void computerAttack(std::vector<std::vector<char>>& computerAttackGrid, int& x, int& y) {
    bool validAttack = false;
    char buffer[1024] = {0};
    while (!validAttack) {
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
        if (computerAttackGrid[x][y] == '.') {
            validAttack = true;
        }
    }
}

void sendMessage(int socket, const char* message) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);
    send(socket, buffer, 1024, 0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::vector<std::vector<char>> playerGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
    std::vector<std::vector<char>> computerGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
    std::vector<std::vector<char>> computerAttackGrid(GRID_SIZE, std::vector<char>(GRID_SIZE));
    initializeGrid(playerGrid);
    initializeGrid(computerGrid);
    initializeGrid(computerAttackGrid);

    // Computer places ships randomly
    randomlyPlaceShips(computerGrid);

    // Display computer's ship placement
    std::cout << "Computer's ship placement:" << std::endl;
    printGrid(computerGrid);

    bool gameOver = false;
    while (!gameOver) {
        // 1. receive attack from client
        char buffer[1024] = {0};
        int bytesRead = read(new_socket, buffer, 1024);
        if (bytesRead <= 0) {
            std::cerr << "Read error or connection closed by client" << std::endl;
            break;
        }
        buffer[bytesRead] = '\0';
        std::cout << "Received data from user: " << buffer << std::endl;

        int x, y;
        sscanf(buffer, "%d %d", &x, &y);

        // Ensure coordinates are within bounds
        if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
            std::cerr << "Received out of bounds coordinates from client: (" << x << ", " << y << ")" << std::endl;
            sendMessage(new_socket, "Coordinates out of bounds");
            continue;
        }

        // 2. tell client if it's missed or hit

        if (computerGrid[x][y] == 'S' || computerGrid[x][y] == 'H') {
            computerGrid[x][y] = 'H';
            if (isGameOver(computerGrid)) {
                sendMessage(new_socket, "Game Over");
                gameOver = true;
            } else {
                sendMessage(new_socket, "Hit");
            }
        } else {
            computerGrid[x][y] = 'M';
            sendMessage(new_socket, "Miss");
        }
		
        std::cout << "Computer's ship placement after player's attack:" << std::endl;
        
        
        printGrid(computerGrid);

        if (gameOver) {
            std::cout << "Computer loss :( " << std::endl;
            break;
        }
        

        // 3. Computer's attack
        bool validAttack = false;
        while (!validAttack) {
            x = rand() % GRID_SIZE;
            y = rand() % GRID_SIZE;
            if (computerAttackGrid[x][y] == '.') {
                validAttack = true;
            }
        }


        memset(buffer, 0, 1024);
        sprintf(buffer, "%d %d", x, y);

        std::cout << "buffer = " << buffer << std::endl;

        send(new_socket, buffer, 1024, 0);



        std::cout << "x = " << x << ", y = " << y << std::endl;
        // 4. receive from client if we're missed or hit
        memset(buffer, 0, 1024);
        bytesRead = read(new_socket, buffer, 1024);
        if (bytesRead <= 0) {
            std::cerr << "Read error or connection closed by client" << std::endl;
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
                computerAttackGrid[x][y] = 'O';
                std::cout << "we win" << std::endl;
                break;
            } else if (line.find("Hit") == 0) {
                computerAttackGrid[x][y] = 'X';
                std::cout << "Hit!" << std::endl;
            } else if (line.find("Miss") == 0) {
                computerAttackGrid[x][y] = 'O';
                std::cout << "Miss!" << std::endl;
            } 
        }

        std::cout << "Computer's attack grid:" << std::endl;
        printGrid(computerAttackGrid);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
