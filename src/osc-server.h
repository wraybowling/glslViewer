#pragma once

#include "../include/oscpack/osc/OscReceivedElements.h"
#include "../include/oscpack/osc/OscPacketListener.h"
#include "../include/oscpack/ip/UdpSocket.h"

#include <iostream>
#include <cstring>

class MyPacketListener : public osc::OscPacketListener {
  protected:
    virtual void ProcessMessage(
        const osc::ReceivedMessage& m,
        const IpEndpointName& remoteEndpoint );
};