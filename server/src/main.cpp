
#include "main.h"

void stoke::game::run()
{
    auto self = *this;

    // http server
    nextgen::network::server<nextgen::network::tcp_socket> s1(self->service, 80);

    // policy server
    nextgen::network::server<nextgen::network::tcp_socket> s2(self->service, 843);

    // realm server
    nextgen::network::server<nextgen::network::tcp_socket> s3(self->service, 6110);

    // game server
    nextgen::network::server<nextgen::network::tcp_socket> s4(self->service, 6111);

    // chat server
    nextgen::network::server<nextgen::network::tcp_socket> s5(self->service, 6112);


    s1.accept(
    [self](nextgen::network::tcp_socket socket)
    {
        nextgen::network::http_client client(socket);

        client.receive(
        [self, client](nextgen::network::http_message response)
        {


        },
        []()
        {

        });
    });

    s2.accept(
    [self](nextgen::network::tcp_socket socket)
    {
        nextgen::network::xml_client client(socket);

        client.receive(
        [self, client](nextgen::network::xml_message response)
        {


        },
        []()
        {

        });
    });

    s3.accept(
    [self](nextgen::network::tcp_socket socket)
    {
        nextgen::network::ngp_client client(socket);

        client.receive(
        [self, client](nextgen::network::ngp_message response)
        {


        },
        []()
        {

        });
    });

    s4.accept(
    [self](nextgen::network::tcp_socket socket)
    {
        nextgen::network::ngp_client client(socket);

        client.receive(
        [self, client](nextgen::network::ngp_message response)
        {


        },
        []()
        {

        });
    });

    s5.accept(
    [self](nextgen::network::tcp_socket socket)
    {
        nextgen::network::ngp_client client(socket);

        client.receive(
        [self, client](nextgen::network::ngp_message response)
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
    social_satan::main::instance().run();
}
