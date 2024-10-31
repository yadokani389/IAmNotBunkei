#pragma once

#include <Siv3D.hpp>

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

  void sendStart(const size_t& level, const Array<Array<Array<size_t>>>& indexes, const Array<size_t>& categoryIndexes) {
    for (const auto& sessionID : sessionIds) {
      if (server.hasSession(sessionID)) {
        size_t sizeAll = 0;
        sizeAll += sizeof(size_t);
        Serializer<MemoryWriter> writer1;
        Serializer<MemoryWriter> writer2;
        writer1(indexes);
        writer2(categoryIndexes);
        sizeAll += writer1->getBlob().size();
        sizeAll += writer2->getBlob().size();
        Console << U"size: " << writer1->getBlob().size();
        Console << U"size: " << writer2->getBlob().size();
        server.send(1ull, sessionID);
        server.send(level, sessionID);
        server.send(writer1->getBlob().size(), sessionID);
        server.send(writer2->getBlob().size(), sessionID);
        server.send(writer1->getBlob().data(), writer1->getBlob().size(), sessionID);
        server.send(writer2->getBlob().data(), writer2->getBlob().size(), sessionID);

        Console << U"Notified to ID: " << sessionID << U"Start level at: " << level;
        Console << U"Category Indexes: " << categoryIndexes;
      }
    }
  }

  bool receiveStart(int& level, Array<Array<Array<size_t>>>& indexes, Array<size_t>& categoryIndexes) {
    size_t levelBuff;
    size_t sizeAll;
    size_t size1;
    size_t size2;
    Blob blob;
    if (!client.read(sizeAll))
      return false;
    Console << U"Start Received!";
    while (client.available() < sizeAll);

    if (!client.read(levelBuff))
      return false;
    Console << U"level: " << levelBuff;
    if (!client.read(size1))
      return false;
    Console << U"size1: " << size1;
    if (!client.read(size2))
      return false;
    Console << U"size2: " << size2;
    blob.resize(size1 + size2);
    if (!client.read(blob.data(), size1 + size2))
      return false;

    level = levelBuff;
    Deserializer<MemoryReader> reader1{blob.data(), size1};
    reader1(indexes);
    Deserializer<MemoryReader> reader2{blob.data() + size1, size2};
    reader2(categoryIndexes);

    Console << U"Start Received! Level at: " << level;
    Console << U"Category Indexes: " << categoryIndexes;
    return true;
  }

  void receiveScore(size_t& score, size_t& receivedAmount) {
    for (const auto& sessionID : sessionIds) {
      size_t scoreBuff;
      if (!server.read(scoreBuff, sessionID) || !(scoreBuff & (1 << 30)))
        continue;
      scoreBuff &= ~(1 << 30);
      Console << U"Client: " << sessionID << U" score: " << scoreBuff;
      score += scoreBuff;
      receivedAmount++;
    }
  }

  void sendScore(size_t& score) {
    if (client.isConnected()) {
      client.send(score | (1 << 30));
      Console << U"Send score to Sever! Score: " << score;
    } else {
      Console << U"No connections!";
    }
  }

  void sendPop() {
    if (isHost)
      for (const auto& sessionID : sessionIds)
        server.send(1ull, sessionID);
    else
      client.send(1ull);
  }

  size_t receivePop() {
    size_t pop = 0;
    if (isHost) {
      for (const auto& sessionID : sessionIds) {
        size_t popBuff;
        while (server.read(popBuff, sessionID))
          pop += popBuff;
      }
    } else {
      size_t popBuff;
      while (client.read(popBuff))
        pop += popBuff;
    }
    return pop;
  }

  void updateServer() {
    if (!server.hasSession())
      return;
    auto sessionIDs = server.getSessionIDs();
    for (const auto& sessionID : sessionIDs) {
      if (!sessionIds.includes(sessionID)) {
        sessionIds << sessionID;
        Console << U"New client! ID: " << sessionID;
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
