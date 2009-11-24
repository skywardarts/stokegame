#include "main.h"

void stoke::game::run()
{
    auto self = *this;

    nextgen::network::create_server<nextgen::network::http_client>(self->service, 80,
    [=](nextgen::network::http_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 80) accepted HTTP client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::http_message request) // bugfix(daemn) lambda wont PBV, so temp PVR
        {
            std::cout << "[stoke:game:run:server] Received data from HTTP client." << std::endl;

            std::cout << request->raw_header_list << std::endl;
            std::cout << request->content << std::endl;

            nextgen::network::http_message response;

            response->version = "1.1";
            response->status_code = 200;
            response->header_list["Connection"] = "close";
            response->content = "hi";

            client2.send(response);
            client2.disconnect();

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::xml_client>(self->service, 843,
    [=](nextgen::network::xml_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 843) accepted XML client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::xml_message request)
        {
            std::cout << "[stoke:game:run:server] Received data from XML client." << std::endl;

            std::cout << request->content << std::endl;

            if(request->content == "<policy-file-request/>")
            {
                nextgen::network::xml_message response;

                response->content = "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"6110-6112\" /></cross-domain-policy>\0";

                client2.send(response);
            }

            client2.disconnect();
        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::ngp_client>(self->service, 6110,
    [=](nextgen::network::ngp_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 6110) accepted NGP client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::ngp_message response)
        {
            nextgen::engine::world_player player();


        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::ngp_client>(self->service, 6111,
    [=](nextgen::network::ngp_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 6111) accepted NGP client." << std::endl;

        auto self2 = self; // bugfix(daemn) nested lambdas have no stack
        auto client2 = client; // bugfix(daemn) nested lambdas have no stack

        client.receive(
        [self2, client2](nextgen::network::ngp_message response)
        {

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::ngp_client>(self->service, 6112,
    [=](nextgen::network::ngp_client client)
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
