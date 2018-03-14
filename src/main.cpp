#include <iostream>
// global configrations
#include "T3.hpp"
#include "Client.hpp"
#include "Server.hpp"



using namespace std;


// Two players client vs server 
//            P1   vs   P2



int main(){
  char choice;
  std::cout << "Invite a player or join a game ( I / J) : ";
  std::cin >> choice;

  if( choice== 'I' || choice == 'i'){
    // Be a server
    cout<<"Server started on ...\n";
    Server server;

  } else if (choice == 'J' || choice == 'j' ){
    // Be a client
    char serv[128];
    cout<< "Enter server address : ";
    scanf("%s",serv);
    Log(serv);


    Client client(serv);

  } else std::cout <<"Invalid choice! try again\n";

  return 0;
}
