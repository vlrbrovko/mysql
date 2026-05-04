CREATE DATABASE IF NOT EXISTS network_logs;
USE network_logs;

CREATE TABLE IF NOT EXISTS messages (
   id INT AUTO_INCREMENT PRIMARY KEY,          -- Унікальний ідентифікатор запису
   client_ip VARCHAR(45) NOT NULL,             -- IP-адреса відправника (IPv4/IPv6)
   client_port INT NOT NULL,                   -- Порт відправника
   message_text TEXT NOT NULL,                 -- Вміст повідомлення
   received_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- Автоматична мітка часу
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE USER IF NOT EXISTS 'lera_admin'@'localhost' IDENTIFIED BY 'secure_password';
GRANT ALL PRIVILEGES ON network_logs.* TO 'lera_admin'@'localhost';
FLUSH PRIVILEGES;

-- Приклад запиту для перевірки результатів:
-- SELECT * FROM messages ORDER BY received_at DESC;
