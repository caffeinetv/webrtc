#include "caffeine.h"

#include <iostream>

#include "api/peerconnectioninterface.h"

class PCO : public webrtc::PeerConnectionObserver
{
    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override
    {
        std::cout << "OnSignalingChange " << new_state << std::endl;
    }

    void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override
    {
        std::cout << "OnDataChannel " << data_channel->id() << std::endl;
    }

    void OnRenegotiationNeeded() override
    {
        std::cout << "OnRenegotiationNeeded" << std::endl;
    }

    void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override
    {
        std::cout << "OnIceConnectionChange " << new_state << std::endl;
    }

    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override
    {
        std::cout << "OnIceGatheringChange " << new_state << std::endl;
    }

    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override
    {
        std::cout << "OnIceCandidate ";
        if (candidate) {
            std::cout << candidate->server_url();
        }
        std::cout << std::endl;
    }
};


extern "C" bool TestStub() {
    PCO pco;
    std::cerr << "This is only a stub.\n";
    return true;
}
