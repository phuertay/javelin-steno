//---------------------------------------------------------------------------

#pragma once

//---------------------------------------------------------------------------

enum class ConnectionId {
  ANY,
  BLE,
  USB,
  USB_PAIR,
};

enum class PairConnectionId {
  ANY,
  BLE,
  CABLE,
};

class Connection {
public:
  static bool IsConnected(ConnectionId connectionId);
  static ConnectionId GetActiveConnection();
  static void SetPreferredConnection(ConnectionId first, ConnectionId second,
                                     ConnectionId third);
  static bool IsPairConnected(PairConnectionId pairConnectionId);

  // This is based on the currently active connection.
  static bool IsHostSleeping();

  static void PrintInfo();

  static ConnectionId preferredConnections[];
};

//---------------------------------------------------------------------------
