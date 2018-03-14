// #include "Socket.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "T3.hpp"
using namespace std;

class Server
{
  int sockfd, newsockfd, port_no, bindfd, listenfd, bytes_sent, bytes_recvd;
  char buffer[512], cli_ip[16], sname[64], cname[64];
  char *ptr_buff, *ptr_port;
  const char *ptr_cli_ip;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t serv_size, cli_size;
  int inp_true = 0, count = 0, inp, ni, x, y, toss;
  char serv_choice, cli_choice, nc;
  char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;

public:
  Server()
  {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
      Error("Server side listening Socket could not be created!");
    // init serv_addr with 0
    memset(&serv_addr, 0, sizeof(serv_addr));
    port_no = CONFIG.PORT;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    //binding socket
    bindfd = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bindfd == -1)
      Error("Failed to bind!");

    //listening for incoming connections
    cout << "Enter your Name : ";
    cin >> sname;
    cout << "Server created!" << endl
         << "Waiting for a Player..." << endl;

    listenfd = listen(sockfd, 5);
    if (listenfd == -1)
      Error("Failed to listen!");

    serv_size = sizeof(serv_addr);
    cli_size = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_size);

    if (newsockfd == -1)
      Error("Failed to accept from client!");

    ptr_cli_ip = inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, cli_size);
    cout << "Server received connections from " << cli_ip << endl;

    memset(&cname, 0, sizeof(cname));
    do
    {
      static int flag = 0;
      bytes_recvd = recv(newsockfd, &cname, sizeof(cname), 0);
      if (bytes_recvd == -1 && flag == 0)
      {
        memset(&cname, 0, sizeof(cname));
        cout << "Could not ACQUIRE Player Information!" << endl
             << "Trying again..." << endl;
        continue;
      }
      else
      {
        flag = 1;
        bytes_sent = send(newsockfd, &sname, sizeof(sname), 0);
        if (bytes_sent == -1)
          cout << "Could not SEND Player Data!"
               << "Trying Again..." << endl;
        else
          cout << cname << " has joined the game." << endl;
      }
    } while (bytes_recvd == -1 || bytes_sent == -1);

    cout << "Creating Game. Please wait..." << endl;
    sleep(2);
    cout << endl
         << "Game created!" << endl
         << endl
         << "Doing a toss...";
    srand(time(NULL));
    toss = rand() % 2;
    sleep(1);
    sprintf(&toss_buffer, "%d", toss);
    bytes_sent = send(newsockfd, &toss_buffer, sizeof(toss_buffer), 0);
    if (bytes_sent == -1)
      Error("TOSS BUFFER not sent!");
    if (toss == 0)
    {
      cout << endl
           << "You WON the toss!" << endl;
      do
      {
        cout << sname << " Enter Your Choice (X or O): ";
        cin >> serv_choice;
        if (serv_choice == 'X' || serv_choice == 'x')
        {
          serv_choice = 'X';
          cli_choice = 'O';
          inp_true = 1;
          cout << endl
               << cname << " gets O." << endl
               << endl
               << "Lets Play!" << endl
               << endl;
        }
        else if (serv_choice == 'O' || serv_choice == 'o' || serv_choice == '0')
        {
          serv_choice = 'O';
          cli_choice = 'X';
          inp_true = 1;
          cout << endl
               << cname << " gets X." << endl
               << endl
               << "Lets Play!" << endl
               << endl;
        }
        else
        {
          cout << "\nInvalid Choice! Please Choose Again..." << endl;
          inp_true == 0;
        }
      } while (inp_true == 0);

      memset(&choice_buffer, 0, sizeof(choice_buffer));
      choice_buffer[0] = serv_choice;
      choice_buffer[1] = cli_choice;

      bytes_sent = send(newsockfd, &choice_buffer, sizeof(choice_buffer), 0);
      if (bytes_sent == -1)
        Error("CHOICE BUFFER could not be sent!");
    }
    else
    {
      cout << endl
           << cname << " WON the toss." << endl;
      cout << cname << " is choosing. Please wait..." << endl
           << endl;

      memset(&choice_buffer, 0, sizeof(choice_buffer));
      bytes_recvd = recv(newsockfd, &choice_buffer, sizeof(choice_buffer), 0);
      if (bytes_recvd == -1)
        Error("CHOICE BUFFER not received!");

      else
      {
        serv_choice = choice_buffer[0];
        cli_choice = choice_buffer[1];
        cout << sname << " has chosen " << serv_choice << endl
             << endl
             << "You will play with " << cli_choice << endl;
        cout << endl
             << "Lets Play!" << endl
             << endl;
      }
    }

    if (serv_choice == 'X')
    {
      inp = 1;
      cout << "You  will play first." << endl
           << endl;
    }
    else
    {
      inp = 2;
      cout << cname << " will play first." << endl
           << endl;
    }

    cout << endl
         << "Starting Game..." << endl;
    Game.display();

    while (count < 9)
    {
      memset(&co_ordinates_buffer, 0, sizeof(co_ordinates_buffer));

      if (inp % 2 != 0)
      {
        cout << endl
             << "Your turn. Enter co-ordinates separated by a space : ";
        cin >> x >> y;
        ni = Game.input(serv_choice, x, y);
        if (ni == 0)
        {
          inp++;
          sprintf(&co_ordinates_buffer[0], "%d", x);
          sprintf(&co_ordinates_buffer[1], "%d", y);
          cout << endl
               << "Updating Matrix..." << endl;
          bytes_sent = send(newsockfd, &co_ordinates_buffer, sizeof(co_ordinates_buffer), 0);
          if (bytes_sent == -1)
            Error("CO-ORDINATES BUFFER not sent!");
        }
      }
      else
      {
        cout << endl
             << cname << "'s turn. Please wait..." << endl;
        bytes_recvd = recv(newsockfd, &co_ordinates_buffer, sizeof(co_ordinates_buffer), 0);
        if (bytes_recvd == -1)
          Error("CO-ORDINATES BUFFER not recieved!");
        x = co_ordinates_buffer[0] - '0';
        y = co_ordinates_buffer[1] - '0';
        ni = Game.input(cli_choice, x, y);
        if (ni == 0)
        {
          inp++;
          cout << endl
               << cname << " has played. Updating Matrix..." << endl;
        }
      }

      count++;
      sleep(2);
      Game.display();

      if (count >= 5)
      {
        nc = Game.check();
        if (nc == 'f')
          continue;
        else if (serv_choice == nc)
        {
          cout << endl
               << "Congrats! You have won!!!" << endl
               << cname << " lost." << endl;
          break;
        }
        else if (cli_choice == nc)
        {
          cout << endl
               << "You loose." << endl
               << cname << " has won." << endl;
          break;
        }
      }
    }

    if (nc == 'f')
      cout << endl
           << "Game ends in a draw." << endl;

    cout << endl
         << "Thank You for playing Tic-tac-Toe" << endl;
  }
  virtual ~Server()
  {
    close(newsockfd);
    close(sockfd);
  }
};
