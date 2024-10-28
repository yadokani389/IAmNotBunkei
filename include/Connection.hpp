#pragma once

#include <Siv3D.hpp>

struct StartMessage {
  int ID = 1;
  int level;
};

struct ScoreMessage {
  int ID = 2;
  size_t score;
};

struct Server {
  bool isHost = false;

  // for server
  TCPServer m_Server;
  Array<TCPSessionID> m_SessionIDs;

  // for client
  TCPClient m_Client;

  void StartServer(uint16 port) {
    isHost = true;
    m_Server.startAcceptMulti(port);
    Console << U"Server Established!";

    // 自分の IP アドレス一覧を取得
    Array<IPv4Address> addresses = Network::EnumerateIPv4Addresses();

    // すべての IP アドレスを表示
    for (const auto& address : addresses) {
      Console << U"Local IP Address: {}"_fmt(address);
    }
  }

  bool ConnectToServer(const IPv4Address& serverAddress, uint16 port) {
    isHost = false;

    if (m_Client.connect(serverAddress, port)) {
      Console << U"Connected to Server!";
      return true;
    } else {
      Console << U"Failed to Connect to Server!";
      return false;
    }
  }

  void SendStart(const int& level) {
    StartMessage message;
    message.level = level;

    for (const auto& sessionID : m_SessionIDs) {
      if (m_Server.hasSession(sessionID)) {
        m_Server.send(message, sessionID);
        Console << U"Notified to ID: " << sessionID << U"Start level at: " << level;
      }
    }
  }

  bool ReceiveStart(int& level) {
    StartMessage message;
    if (m_Client.available() >= sizeof(StartMessage)) {
      if (m_Client.read(message)) {
        if (message.ID == 1) {
          level = message.level;
          Console << U"Start Received! Level at: " << level;
          return true;
        }
      }
    }
    return false;
  }

  void SendScore(size_t& score) {
    ScoreMessage message;
    message.score = score;

    if (m_Client.isConnected()) {
      m_Client.send(message);
      Console << U"Send score to Sever! Score: " << score;
    } else {
      Console << U"No connections!";
    }
  }

  void ReceiveScore(size_t& score, size_t& receivedAmount) {
    for (const auto& sessionID : m_SessionIDs) {
      while (m_Server.available(sessionID) >= sizeof(ScoreMessage)) {
        ScoreMessage message;
        if (m_Server.read(message, sessionID) && message.ID == 2) {
          Console << U"Client: " << sessionID << U" score: " << message.score;
          score += message.score;
          receivedAmount++;
        }
      }
    }
  }

  void UpdateServer() {
    if (m_Server.hasSession()) {
      auto sessionIDs = m_Server.getSessionIDs();
      for (const auto& sessionID : sessionIDs) {
        if (!m_SessionIDs.includes(sessionID)) {
          m_SessionIDs << sessionID;
          Console << U"New client! ID: " << sessionID;
        }
      }
    }
  }

  void UpdateClient() {
    if (!m_Client.isConnected() && !m_Client.hasError()) {
      Console << U"Disconnected!";
    }
  }

  void Update() {
    if (isHost)
      UpdateServer();
    else
      UpdateClient();
  }
};