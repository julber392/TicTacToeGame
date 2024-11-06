#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <sstream>

class TicTacToeServer {
public:
    TicTacToeServer(int port) : port(port), currentPlayer(0) {
        initializeBoard();
    }

    void run() {
        //Создание сервера
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            std::cerr << "Error creating socket\n";
            return;
        }

        sockaddr_in address = {};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        address.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_fd, (sockaddr *) &address, sizeof(address)) < 0) {
            std::cerr << "Binding error\n";
            close(server_fd);
            return;
        }

        listen(server_fd, 2);
        std::cout << "Waiting for players...\n";

        player_sockets[0] = accept(server_fd, nullptr, nullptr);
        std::cout << "Player 1 connected.\n " << std::endl;
        send(player_sockets[0], "X", 1, 0);

        player_sockets[1] = accept(server_fd, nullptr, nullptr);
        std::cout << "Player 2 connected.\n " << std::endl;
        send(player_sockets[1], "O", 1, 0);
        std::thread th1(&TicTacToeServer::receiving, this, 0);
        std::thread th2(&TicTacToeServer::receiving, this, 1);

        sendToAll(displayBoard());
        sendMessage(0,"Your turn. Select position (1-9):");
        while (true){

        }
        close(server_fd);
        th1.join();
        th2.join();
    }

    //Получение сообщения с номером хода от игрока
    void sending(int player, int message) {
        if (currentPlayer != player) {
            std::cout<<"currentPlayer = "<<currentPlayer<<std::endl;
            sendMessage(player, "Not your move\n");
        } else {

            std::cout<<displayBoard();
            char marker = currentPlayer == 0 ? 'X' : 'O';
            //Проверка и постановка хода игрока
            if (!placeMarker(message, marker)) {
                sendMessage(currentPlayer, "Illegal move. Try again.");
                return;
            }
            //Проверка выигрыша
            if (checkWin()) {
                std::string winMessage = "Player ";
                winMessage += (currentPlayer == 0 ? "1" : "2");
                winMessage += " win!";
                sendToAll(winMessage);
                return;
            }
            //Проверка ничьи
            if (draw()) {
                std::string winMessage = "Draw!";
                sendToAll(winMessage);
                return;
            }

            sendToAll(displayBoard());
            sendMessage(currentPlayer, "Waiting for the other player to move");
            currentPlayer = 1 - currentPlayer; //смена хода игрока

            sendMessage(currentPlayer,"Your turn. Select position (1-9):");
        }
    }

    //Функция для получения сообщения от пользователя в номером id
    void receiving(int id) {
        int position;
        while (true) {
            recv(player_sockets[id], &position, sizeof(position), 0);
            sending(id,position);
        }
    }

private:
    int port;
    int player_sockets[2];
    int currentPlayer = 0;

    std::vector<char> board;  // Игровое поле, представленное вектором

    //Отправление сообщения player
    void sendMessage(int player, std::string message) {
        int length = message.size();
        send(player_sockets[player], &length, sizeof(length), 0);
        send(player_sockets[player], message.c_str(), length, 0);
    }

    //Проверка на ничью
    bool draw() {
        for (auto ch: board) {
            if (ch != 'O' && ch != 'X') {
                return false;
            }
        }
        return true;
    }

    //Отправка сообщения message всем игрокам
    void sendToAll(const std::string &message) {
        sendMessage(currentPlayer,message);
        int cur=1-currentPlayer;
        std::cout<<displayBoard();
        sendMessage(cur,message);
    }

    // Функция для проверки выигрышной комбинации
    bool checkWin() const {
        // Определяем все возможные выигрышные комбинации
        const int winPatterns[8][3] = {
                {0, 1, 2},
                {3, 4, 5},
                {6, 7, 8},  // горизонтальные
                {0, 3, 6},
                {1, 4, 7},
                {2, 5, 8},  // вертикальные
                {0, 4, 8},
                {2, 4, 6}              // диагонали
        };

        // Проверяем каждый выигрышный шаблон
        for (const auto &pattern: winPatterns) {
            if (board[pattern[0]] == board[pattern[1]] &&
                board[pattern[1]] == board[pattern[2]]) {
                return true; // Выигрышная комбинация найдена
            }
        }
        return false; // Выигрышная комбинация не найдена
    }

    // Функция для проверки и размещения символа на поле
    bool placeMarker(int position, char marker) {
        if (position < 1 || position > 9 || board[position - 1] == 'X' || board[position - 1] == 'O') {
            std::cout << "Illegal move. Try again." << std::endl;
            return false;
        }
        board[position - 1] = marker;
        return true;
    }

    // Функция для отображения поля
    std::string displayBoard() const {
        std::stringstream str;
        str << board[0] << "|" << board[1] << "|" << board[2] << "\n";
        str << "-+-+-\n";
        str << board[3] << "|" << board[4] << "|" << board[5] << "\n";
        str << "-+-+-\n";
        str << board[6] << "|" << board[7] << "|" << board[8] << std::endl;
        return str.str();
    }

    // Инициализация поля с номерами ячеек
    void initializeBoard() {
        board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    }
};

int main(int argc, char *argv[]) {
    if (argc>1){
        int port=std::atoi(argv[1]);
        TicTacToeServer server(port);
        server.run();
    }
    else{
        std::cerr<<"No valid port";
        return 1;
    }
    return 0;
}
