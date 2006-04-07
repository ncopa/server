/* Copyright (C) 2003 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef STOP_REQ_HPP
#define STOP_REQ_HPP

#include "SignalData.hpp"

class StopReq 
{
  /**
   * Reciver(s)
   */
  friend class Ndbcntr;

  /**
   * Sender
   */
  friend class MgmtSrvr;

public:
  STATIC_CONST( SignalLength = 9 + NdbNodeBitmask::Size);
  
public:
  Uint32 senderRef;
  Uint32 senderData;
  
  Uint32 requestInfo;
  Uint32 singleuser;          // Indicates whether or not to enter 
                              // single user mode.
                              // Only in conjunction with system stop
  Uint32 singleUserApi;       // allowed api in singleuser

  Int32 apiTimeout;           // Timeout before api transactions are refused
  Int32 transactionTimeout;   // Timeout before transactions are aborted
  Int32 readOperationTimeout; // Timeout before read operations are aborted
  Int32 operationTimeout;     // Timeout before all operations are aborted

  Uint32 nodes[NdbNodeBitmask::Size];

  static void setSystemStop(Uint32 & requestInfo, bool value);
  static void setPerformRestart(Uint32 & requestInfo, bool value);
  static void setNoStart(Uint32 & requestInfo, bool value);
  static void setInitialStart(Uint32 & requestInfo, bool value);
  /**
   * Don't perform "graceful" shutdown/restart...
   */
  static void setStopAbort(Uint32 & requestInfo, bool value);
  static void setStopNodes(Uint32 & requestInfo, bool value);

  static bool getSystemStop(const Uint32 & requestInfo);
  static bool getPerformRestart(const Uint32 & requestInfo);
  static bool getNoStart(const Uint32 & requestInfo);
  static bool getInitialStart(const Uint32 & requestInfo);
  static bool getStopAbort(const Uint32 & requestInfo);
  static bool getStopNodes(const Uint32 & requestInfo);
};

struct StopConf
{
  STATIC_CONST( SignalLength = 2 );
  Uint32 senderData;
  union {
    Uint32 nodeState;
    Uint32 nodeId;
  };
};

class StopRef 
{
  /**
   * Reciver(s)
   */
  friend class MgmtSrvr;
  
  /**
   * Sender
   */
  friend class Ndbcntr;

public:
  STATIC_CONST( SignalLength = 3 );
  
  enum ErrorCode {
    OK = 0,
    NodeShutdownInProgress = 1,
    SystemShutdownInProgress = 2,
    NodeShutdownWouldCauseSystemCrash = 3,
    TransactionAbortFailed = 4,
    UnsupportedNodeShutdown = 5,
    MultiNodeShutdownNotMaster = 6
  };
  
public:
  Uint32 senderData;
  Uint32 errorCode;
  Uint32 masterNodeId;
};

inline
bool
StopReq::getSystemStop(const Uint32 & requestInfo)
{
  return requestInfo & 1;
}

inline
bool
StopReq::getPerformRestart(const Uint32 & requestInfo)
{
  return requestInfo & 2;
}

inline
bool
StopReq::getNoStart(const Uint32 & requestInfo)
{
  return requestInfo & 4;
}

inline
bool
StopReq::getInitialStart(const Uint32 & requestInfo)
{
  return requestInfo & 8;
}

inline
bool
StopReq::getStopAbort(const Uint32 & requestInfo)
{
  return requestInfo & 32;
}

inline
bool
StopReq::getStopNodes(const Uint32 & requestInfo)
{
  return requestInfo & 64;
}


inline
void
StopReq::setSystemStop(Uint32 & requestInfo, bool value)
{
  if(value)
    requestInfo |= 1;
  else
    requestInfo &= ~1;
}

inline
void 
StopReq::setPerformRestart(Uint32 & requestInfo, bool value)
{
  if(value)
    requestInfo |= 2;
  else
    requestInfo &= ~2;
}

inline
void 
StopReq::setNoStart(Uint32 & requestInfo, bool value)
{
  if(value)
    requestInfo |= 4;
  else
    requestInfo &= ~4;
}

inline
void
StopReq::setInitialStart(Uint32 & requestInfo, bool value)
{
  if(value)
    requestInfo |= 8;
  else
    requestInfo &= ~8;
}

inline
void
StopReq::setStopAbort(Uint32 & requestInfo, bool value)
{
  if(value)
    requestInfo |= 32;
  else
    requestInfo &= ~32;
}

inline
void
StopReq::setStopNodes(Uint32 & requestInfo, bool value)
{
  if(value)
    requestInfo |= 64;
  else
    requestInfo &= ~64;
}

#endif

