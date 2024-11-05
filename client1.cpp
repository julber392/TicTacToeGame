#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <limits>
#include <fcntl.h>
#include <fstream>
#include <thread>
class TicTacToeClient {
public:
    TicTacToeClient(const std::string& ip, int port) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            std::cerr << "Ошибка создания сокета\n";
            return;
        }

        sockaddr_in address = {};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
            std::cerr << "Неверный IP-адрес\n";
            close(sock);
            return;
        }

        if (connect(sock, (sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Ошибка подключения\n";
            close(sock);
            return;
        }
        initializeBoard();
        char mark;
        recv(sock, &mark, 1, 0);
        playerMarker = mark;
        std::cout << "You are playing " << playerMarker << std::endl;

    }
    void input(){
        int position;
        while (!flag){
            std::cin >> position;
            send(sock, &position, sizeof(position), 0);
        }
    }

    void play() {
        std::cout<<"Waiting for another player to connect..."<<std::endl;
        std::thread th1(&TicTacToeClient::input,this);
        while (true) {
            // Получаем обновленное состояние доски с сервера
            //recv(sock, board.data(), board.size(), 0);
            //int position;

            int length2;
            recv(sock, &length2, sizeof(length2), 0);
            std::vector<char> buffer2(length2);
            recv(sock, buffer2.data(), length2, 0);
            std::string message2(buffer2.begin(), buffer2.end());
            std::cout<<message2<<std::endl;

            if (message2[0]=='P'||message2[0]=='D'){//Player 1/2 win || Draw!
                flag=true;
                break;
            }
            //std::cin >> position;

            // Отправляем позицию хода серверу
            //send(sock, &position, sizeof(position), 0);

            /*int length;

            recv(sock, &length, sizeof(length), 0);
            std::vector<char> buffer(length);
            recv(sock, buffer.data(), length, 0);
            std::string message(buffer.begin(), buffer.end());
            std::cout<<message<<std::endl;

            if (message[0]=='P'||message[0]=='D'){
                break;
            }*/
        }
        th1.detach();
    }

private:
    int sock;
    char playerMarker;
    bool flag=false;
    std::vector<char> board;
    void initializeBoard() {
        board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    }
    /*void displayBoard() {
        std::cout << board[0] << "|" << board[1] << "|" << board[2] << "\n";
        std::cout << "-+-+-\n";
        std::cout << board[3] << "|" << board[4] << "|" << board[5] << "\n";
        std::cout << "-+-+-\n";
        std::cout << board[6] << "|" << board[7] << "|" << board[8] << "\n";
    }*/

};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <port>\n";
        return 1;
    }

    TicTacToeClient client(argv[1], std::stoi(argv[2]));
    client.play();
    return 0;
}
