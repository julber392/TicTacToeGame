#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <thread>
class TicTacToeClient {
public:
    TicTacToeClient(const std::string& ip, int port) {
        //Подключение к серверу
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            std::cerr << "Error creating socket\n";
            return;
        }
        sockaddr_in address = {};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
            std::cerr << "Invalid IP address\n";
            close(sock);
            return;
        }
        if (connect(sock, (sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Connection error\n";
            close(sock);
            return;
        }
        char mark;
        recv(sock, &mark, 1, 0); //Получение очередности хода X или O
        playerMarker = mark;
        std::cout << "You are playing " << playerMarker << std::endl;

    }
    //Считывание хода игрока
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
            //Получения сообщения от сервера и его вывод
            int length2;
            recv(sock, &length2, sizeof(length2), 0);
            std::vector<char> buffer2(length2);
            recv(sock, buffer2.data(), length2, 0);
            std::string message2(buffer2.begin(), buffer2.end());
            std::cout<<message2<<std::endl;

            //Проверка сообщения о выигрыше или ничьей
            if (message2[0]=='P'||message2[0]=='D'){
                flag=true;
                break;
            }
        }
        th1.detach();
    }

private:
    int sock;
    char playerMarker;
    bool flag=false;
    std::vector<char> board;
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
