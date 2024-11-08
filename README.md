# TicTacToeGame
## _Описание_

Это многопользовательская консольная игра "Крестики-нолики", реализованная на C++. Проект состоит из сервера и клиента, взаимодействующих по TCP. Сервер управляет игрой между двумя игроками, проверяя ходы и состояние доски, и обрабатывает подключение двух клиентов для каждой партии. Каждый клиент отправляет свои ходы и получает обновления доски и статусы.


- Игра проходит на поле размером 3x3, нумерация ячеек:

                                1 | 2 | 3
                                ---------
                                4 | 5 | 6
                                ---------
                                7 | 8 | 9
-  Игроки по очереди выбирают ячейку для установки своего символа (X или O).
-   Побеждает тот, кто первым выстроит три символа по вертикали, горизонтали или диагонали.
- Если все ячейки заняты и нет выигрышной комбинации, засчитывается ничья.

## Установка и сборка
Требования
- Компилятор C++ с поддержкой C++17 или выше.
- CMake для генерации файлов сборки.



## Запуск сервера и клиента
#### Сервер
Сервер ожидает подключения двух клиентов и запускает игровую сессию.
./server <PORT_NUMBER>
PORT_NUMBER — номер порта для подключения клиентов.
#### Клиент
Клиент подключается к серверу, указав IP-адрес и порт.
./client <SERVER_IP> <PORT_NUMBER>
SERVER_IP — IP-адрес сервера.
PORT_NUMBER — номер порта, на котором сервер принимает подключения.

## Структура проекта
- Server: обрабатывает подключения, контролирует ход игры, обновляет состояние доски и уведомляет игроков о результатах.
- Client: подключается к серверу, передает ходы пользователя и отображает ход игры.
## Возможности
- Поддержка TCP-соединений.
- Полная логика игры на стороне сервера.
- Уведомления клиентов о состоянии игры и результатах.
# Пример сессии
# Игрок 1

./client 127.0.0.1 12345

You are playing X

Waiting for another player to connect...

1|2|3

-+-+-

4|5|6

-+-+-

7|8|9

Your turn. Select position (1-9):

1

X|2|3

-+-+-

4|5|6

-+-+-

7|8|9

Waiting for the other player to move

X|O|3

-+-+-

4|5|6

-+-+-

7|8|9

Your turn. Select position (1-9):

4

X|O|3

-+-+-

X|5|6

-+-+-

7|8|9

Waiting for the other player to move

X|O|O

-+-+-

X|5|6

-+-+-

7|8|9

Your turn. Select position (1-9):

7

Player 1 win!

# Игрок 2

./client 127.0.0.1 12345

You are playing O

Waiting for another player to connect...

1|2|3

-+-+-

4|5|6

-+-+-

7|8|9

1

Not your move

X|2|3

-+-+-

4|5|6

-+-+-

7|8|9

Your turn. Select position (1-9):

2

X|O|3

-+-+-

4|5|6

-+-+-

7|8|9

Waiting for the other player to move

X|O|3

-+-+-

X|5|6

-+-+-

7|8|9

Your turn. Select position (1-9):

3

X|O|O

-+-+-

X|5|6

-+-+-

7|8|9

Waiting for the other player to move

Player 1 win!

