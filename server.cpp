#include <iostream>
#include <string>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <mysql/mysql.h>

class Database {
private:
    MYSQL* conn;
public:
    Database() {
        conn = mysql_init(NULL);
    }

    bool connect(const char* host, const char* user, const char* pass, const char* db) {
        return mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0);
    }

    void insertMessage(std::string ip, int port, std::string text) {
        std::string query = "INSERT INTO messages (client_ip, client_port, message_text) VALUES ('"
                            + ip + "', " + std::to_string(port) + ", '" + text + "')";
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "MySQL Error: " << mysql_error(conn) << std::endl;
        }
    }

    ~Database() {
        mysql_close(conn);
    }
};

void processClientRequest(std::string ip, int port, std::string message, Database* db) {
    std::cout << "[THREAD] Обробка даних від " << ip << ":" << port << " успішна." << std::endl;
    db->insertMessage(ip, port, message);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];

    Database db;
    if (!db.connect("localhost", "lera_admin", "secure_password", "network_logs")) {
        std::cerr << "Критична помилка: Не вдалося підключитися до MySQL." << std::endl;
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(12345);

    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    std::cout << "--- UDP Сервер Валерії Бровко активний (Port: 12345) ---" << std::endl;

    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&cliaddr, &len);
        if (n < 0) continue;
        buffer[n] = '\0';

        std::string clientIP = inet_ntoa(cliaddr.sin_addr);
        int clientPort = ntohs(cliaddr.sin_port);
        std::string msgText(buffer);

        std::thread(processClientRequest, clientIP, clientPort, msgText, &db).detach();
    }

    close(sockfd);
    return 0;
}
