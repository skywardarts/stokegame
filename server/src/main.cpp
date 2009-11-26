#include "main.h"

namespace nextgen
{
    namespace engine
    {
        class game_object
        {

        };

        class game_npc : public game_object
        {
            public: typedef uint32_t id_type;
            public: typedef string name_type;
            public: typedef int vector_type;
            public: typedef math::vector<vector_type> position_type;
            public: typedef math::vector<vector_type> direction_type;
        };

        class game_monster : public game_npc
        {

        };

        class game_player : public game_npc
        {
            public: typedef network::ngp_client client_type;

            private: struct variables
            {
                variables(client_type client) : id(0), name("Undefined"), client(client)
                {

                }

                ~variables()
                {

                }

                id_type id;
                name_type name;
                position_type position;
                direction_type direction;
                client_type client;
            };

            NEXTGEN_SHARED_DATA(game_player, variables);
        };

struct testing_message_base
{
    int id;
    int length;
};

struct testing_message
{
    int player_id;
    int name_length;
    int position_x;
    int position_y;
    int rotation_x;
    int rotation_y;
    bool something;
};

        class game_world
        {
            public: typedef network::service network_service_type;
            public: typedef array<game_player> player_list_type;
            public: typedef math::vector<int> vector_type;
            public: typedef vector_type position_type;
            public: typedef vector_type direction_type;

            public: void initialize()
            {
                auto self = *this;

                network::create_server<network::ngp_client>(self->network_service, 6110,
                [=](network::ngp_client client)
                {
                    std::cout << "[nextgen:engine:game_world] Server (port 6110) accepted NGP client." << std::endl;

                    game_player new_player(client);

                    new_player->id = random(10000, 99999);
                    new_player->name = "Undefined";
                    new_player->position = position_type::zero();
                    new_player->direction = direction_type::up();


                    for(player_list_type::iterator i = self->player_list.begin(), l = self->player_list.end(); i != l; ++i)
                    {
                        game_player old_player = (*i);
/*
                        // show the new player for old players
                        {
                            network::ngp_message message;

                            std::stringstream content;

                            testing_message t = {new_player->id, new_player->name.length(), new_player->name.c_str(), new_player->position->x, new_player->position->y, new_player->direction->x, new_player->direction->y, false};


                            content << t;//new_player->id << new_player->name.length() << new_player->name << new_player->position->x << new_player->position->y << new_player->direction->x << new_player->direction->y << false;

                            std::stringstream content2;

                            content2 << 5 << content.str().length() << content.str();

                            message->content = content2.str();

                            std::cout << "sending: " << message->content << std::endl;

                            old_player->client.send(message);
                        }

                        // show the old players for the new player
                        {
                            network::ngp_message message;

                            std::stringstream content;

                            content << old_player->id << old_player->name.length() << old_player->name << old_player->position->x << old_player->position->y << old_player->direction->x << old_player->direction->y << false;

                            std::stringstream content2;

                            content2 << 5 << content.str().length() << content.str();

                            message->content = content2.str();

                            std::cout << "sending: " << message->content << std::endl;

                            new_player->client.send(message);
                        }*/
                    }

                    self->player_list.push_back(new_player);

                    // show the new player to self
                    {
                        network::ngp_message message;

                        //testing_message t = {new_player->id, new_player->name.length(), new_player->name.c_str(), new_player->position->x, new_player->position->y, new_player->direction->x, new_player->direction->y, true};
std::ostream data_stream(&message->stream.get_buffer());


                        data_stream << 5 << sizeof(testing_message) + new_player->name.length() << new_player->id << new_player->name.length() << new_player->name.c_str() << new_player->position->x << new_player->position->y << new_player->direction->x << new_player->direction->y << true;

std::cout << sizeof(testing_message) + new_player->name.length();

                        std::cout << "sending: " << &message->stream.get_buffer() << std::endl;

                        new_player->client.send(message);
                    }

                    client.receive(
                    [=](network::ngp_message request)
                    {
                        std::cout << "[nextgen:engine:game_world] Received data from NGP client." << std::endl;

                        std::cout << request->content << std::endl;

                        std::stringstream ss(request->content);

                        int message_id;
                        int message_length;
                        string message_data;

                        ss >> message_id;
                        ss >> message_length;
                        ss >> message_data;

                        std::cout << "message_id: " << message_id << std::endl;
                        std::cout << "message_length: " << message_length << std::endl;
                        std::cout << "message_data: " << message_data << std::endl;

                        player_list_type::iterator i = std::find_if(self->player_list.begin(), self->player_list.end(), [=](game_player player) -> bool
                        {
                            return player->client == client;
                        });

                        game_player player = (*i);


                    },
                    []()
                    {

                    });
                });
            }

            private: struct variables
            {
                variables(network_service_type network_service) : network_service(network_service)
                {

                }

                ~variables()
                {

                }

                network_service_type network_service;
                player_list_type player_list;
            };

            NEXTGEN_SHARED_DATA(game_world, variables,
            {
                this->initialize();
            });
        };

    }
}



namespace stoke
{
	class game : public nextgen::singleton<game>
	{
	    public: typedef nextgen::network::service network_service_type;
	    public: typedef nextgen::engine::game_world game_world_type;

		public: void initialize()
		{
		    auto self = *this;

        }

		public: void run();

        private: struct variables
        {
            variables() : world(network_service)
            {

            }

            ~variables()
            {

            }

            network_service_type network_service;
            game_world_type world;
        };

        NEXTGEN_SHARED_DATA(game, variables);
	};

	class game_message
	{

	};
}

void stoke::game::run()
{
    auto self = *this;

    nextgen::network::create_server<nextgen::network::http_client>(self->network_service, 80,
    [=](nextgen::network::http_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 80) accepted HTTP client." << std::endl;

        client.receive(
        [=](nextgen::network::http_message request)
        {
            std::cout << "[stoke:game:run:server] Received data from HTTP client." << std::endl;

            std::cout << request->raw_header_list << std::endl;
            std::cout << request->content << std::endl;

            nextgen::network::http_message response;

            response->version = "1.1";
            response->status_code = 200;
            response->header_list["Connection"] = "close";
            response->content = "hi";

            client.send(response);
            client.disconnect();

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::xml_client>(self->network_service, 843,
    [=](nextgen::network::xml_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 843) accepted XML client." << std::endl;

        client.receive(
        [=](nextgen::network::xml_message request)
        {
            std::cout << "[stoke:game:run:server] Received data from XML client (length: " << request->content.length() << ")" << std::endl;

            std::cout << request->content << std::endl;

            if(request->content.find("<policy-file-request/>") != nextgen::string::npos)
            {
                nextgen::network::xml_message response;

                response->content = "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"6110-6112\" /></cross-domain-policy>\0";

                client.send(response);
            }

            client.disconnect();
        },
        []()
        {

        });
    });


    nextgen::network::create_server<nextgen::network::ngp_client>(self->network_service, 6111,
    [=](nextgen::network::ngp_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 6111) accepted NGP client." << std::endl;

        client.receive(
        [=](nextgen::network::ngp_message response)
        {

        },
        []()
        {

        });
    });

    nextgen::network::create_server<nextgen::network::ngp_client>(self->network_service, 6112,
    [=](nextgen::network::ngp_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 6112) accepted NGP client." << std::endl;

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
            if(timer.stop() > 1)
            {
                timer.start();

                std::cout << "[stoke:game:run] Updating services..." << std::endl;
            }

            self->network_service.update();

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
