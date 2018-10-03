#include "network.h"
#include "session.h"

ssim::network::network() :
    session_id_(0)
{
}

void ssim::network::init(uint16_t port /*= 9301*/, int therad_num /*= 1*/)
{
    ioc_ = std::make_shared<asio::io_context>(therad_num);
    acc_ = std::make_shared<tcp::acceptor>(*ioc_, tcp::endpoint(tcp::v4(), port));
    socket_ = std::make_shared<tcp::socket>(*ioc_);
}

void ssim::network::run()
{
    try {
        do_accept();
        ioc_->run();
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }
}

void ssim::network::do_accept()
{
    acc_->async_accept(*socket_,
        [this](std::error_code ec)
    {
        if (!ec) {
            std::make_shared<session>(create_session_id(), std::move(*socket_), this)->start();
        }

        do_accept();
    });
}

uint64_t ssim::network::create_session_id()
{
    uint64_t session_id;
    do {
        session_id = session_id_.load();
    } while (session_id_.compare_exchange_weak(session_id, session_id + 1));
    return session_id;
}

SSIM_API ssim::network_interface * ssim::create_network_interface()
{
    return new network();
}

SSIM_API void ssim::destory_network_interface(ssim::network_interface * network)
{
    delete network;
}
