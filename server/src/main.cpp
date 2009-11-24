
#include "main.h"

void stoke::game::run()
{
    auto self = *this;

    nextgen::network::create_server<nextgen::network::http_client>(self->service, 80,
    [self](nextgen::network::http_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 80) accepted HTTP client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::http_message response) // bugfix(daemn) lambda wont PBV, so temp PVR
        {
            std::cout << "[stoke:game:run:server] Received data from HTTP client." << std::endl;

            std::cout << response->raw_header_list << std::endl;
            std::cout << response->content << std::endl;

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::xml_client>(self->service, 843,
    [self](nextgen::network::xml_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 843) accepted XML client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::xml_message response) // bugfix(daemn) lambda wont PBV, so temp PVR
        {

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::ngp_client>(self->service, 6110,
    [self](nextgen::network::ngp_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 6110) accepted NGP client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::ngp_message response) // bugfix(daemn) lambda wont PBV, so temp PVR
        {

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::ngp_client>(self->service, 6111,
    [self](nextgen::network::ngp_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 80) accepted NGP client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::ngp_message response) // bugfix(daemn) lambda wont PBV, so temp PVR
        {

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::ngp_client>(self->service, 6112,
    [self](nextgen::network::ngp_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 6112) accepted NGP client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [=](nextgen::network::ngp_message response)
        {

        },
        []()
        {

        });
    });

    nextgen::timer timer;
    timer.start();

    while(true)
    {
        try
        {
            if(timer.stop() > 1.0f)
            {
                timer.start();

                std::cout << "[stoke:game:run] Updating services..." << std::endl;
            }

            self->service.update();

            boost::this_thread::sleep(boost::posix_time::milliseconds(5));
        }
        catch(boost::exception& e)
        {
            std::cout << "[stoke:game:run] " << "Unexpected exception caught in " << BOOST_CURRENT_FUNCTION << std::endl << boost::current_exception_diagnostic_information();
        }
    }
}

int main()
{
    stoke::game::instance().run();
}
