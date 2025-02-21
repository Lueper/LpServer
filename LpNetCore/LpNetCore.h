#pragma once

#pragma comment (lib, "LpUtility.lib")

#include <iostream>
#include <mutex>
#include <thread>

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/system.hpp"

#include "LpNetEnum.h"

#include "LpIOContext.h"
#include "LpSession.h"
#include "LpSessionPool.h"
#include "LpAcceptor.h"
#include "LpPacket.h"
#include "LpPacketHandler.h"

#include "LpUtility.h"

using namespace boost;