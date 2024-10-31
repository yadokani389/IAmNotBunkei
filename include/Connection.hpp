#pragma once

#include <Siv3D.hpp>

struct Message {
  size_t id;
  size_t element;
};

struct Server {
  bool isHost = false;

  // for server
  TCPServer server;
  Array<TCPSessionID> sessionIds;

  // for client
  TCPClient client;

  void startServer(uint16 port) {
    isHost = true;
    server.startAcceptMulti(port);
    Console << U"Server Established!";

    // 自分の IP アドレス一覧を取得
    Array<IPv4Address> addresses = Network::EnumerateIPv4Addresses();

    // すべての IP アドレスを表示
    for (const auto& address : addresses) {
      Console << U"Local IP Address: {}"_fmt(address);
    }
  }

  bool connectToServer(const IPv4Address& serverAddress, uint16 port) {
    isHost = false;
    client.connect(serverAddress, port);
    if (client.isConnected()) {
      Console << U"Connected to Server!";
      return true;
    } else {
      Console << U"Failed to Connect to Server!";
      return false;
    }
  }

  void sendStart(const int& level) {
    Message message;
    message.id = 1;
    message.element = level;

    for (const auto& sessionID : sessionIds) {
      if (server.hasSession(sessionID)) {
        server.send(message, sessionID);
        Console << U"Notified to ID: " << sessionID << U"Start level at: " << level;
      }
    }
  }

  bool receiveStart(int& level) {
    Message message;
    if (client.available() >= sizeof(Message)) {
      if (client.read(message)) {
        if (message.id == 1) {
          level = message.element;
          Console << U"Start Received! Level at: " << level;
          return true;
        }
      }
    }
    return false;
  }

  void receiveScore(ssize_t& score, size_t& receivedAmount) {
    for (const auto& sessionID : sessionIds) {
      while (server.available(sessionID) >= sizeof(Message)) {
        Message message;
        if (server.read(message, sessionID) && message.id == 2) {
          Console << U"Client: " << sessionID << U" score: " << message.element;
          score += message.element;
          receivedAmount++;
        }
      }
    }
  }

  void sendScore(ssize_t& score) {
    Message message;
    message.id = 2;
    message.element = score;

    if (client.isConnected()) {
      client.send(message);
      Console << U"Send score to Sever! Score: " << score;
    } else {
      Console << U"No connections!";
    }
  }

  void updateServer() {
    if (server.hasSession()) {
      auto sessionIDs = server.getSessionIDs();
      for (const auto& sessionID : sessionIDs) {
        if (!sessionIds.includes(sessionID)) {
          sessionIds << sessionID;
          Console << U"New client! ID: " << sessionID;
        }
      }
    }
  }

  void updateClient() {
    if (!client.isConnected() && !client.hasError()) {
      Console << U"Disconnected!";
    }
  }

  void update() {
    if (isHost)
      updateServer();
    else
      updateClient();
  }
};
