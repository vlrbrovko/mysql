#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
   int sockfd;
   struct sockaddr_in servaddr;
   std::string userInput;

   if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
       perror("Socket creation failed");
       return 1;
   }

   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(12345);
   servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   std::cout << "--- Клієнтський термінал (Валерія Бровко) ---" << std::endl;
   std::cout << "Введіть текст повідомлення (або 'exit' для завершення):" << std::endl;

   while (true) {
       std::cout << "> ";
       std::getline(std::cin, userInput);

       if (userInput == "exit") break;

       sendto(sockfd, userInput.c_str(), userInput.length(), 0,
              (const struct sockaddr*)&servaddr, sizeof(servaddr));
   }

   close(sockfd);
   return 0;
}
