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
            public: typedef math::vector<vector_type> rotation_type;
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
                rotation_type rotation;
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


template <typename integer_type>
void read_int(integer_type& result, const unsigned char* bits, bool little_endian = false)
{
    result = 0;

    uint32_t l = sizeof(result);

    if(little_endian)
        for(uint32_t i = l; i >= 0; --i)
            result = (result << 8) + bits[i];
    else
        for(uint32_t i = 0; i < l; ++i)
            result = (result << 8) + bits[i];
}

uint32_t read_int32(const char* bits, bool little_endian = false)
{
    uint32_t result;

    read_int<uint32_t>(result, (const unsigned char*)bits, little_endian);

    return result;
}

uint32_t read_int32(std::string const& bits, uint32_t offset = 0, bool little_endian = false)
{
    return read_int32(bits.substr(offset, sizeof(uint32_t)).c_str(), little_endian);
}

void write_int32(std::string& bits, uint32_t item)
{
    char bytes[4];
    bytes[0] = item & 0xFF;
    bytes[1] = (item >> 8) & 0xFF;
    bytes[2] = (item >> 16) & 0xFF;
    bytes[3] = (item >> 24) & 0xFF;

    bits += std::string(bytes, 4);
}

        namespace realm_message
        {
            const uint32_t keep_alive = 1;
            const uint32_t user_login = 2;
            const uint32_t create_player = 3;
            const uint32_t update_player = 4;
            const uint32_t remove_player = 5;
            const uint32_t create_object = 6;
            const uint32_t load_asset = 7;
            const uint32_t update_player_position = 41;
            const uint32_t update_player_rotation = 42;

        }

/**
player connects
server looks at player x/y, sends nearby players
server looks at player x/y, sends nearby objects (building)
server looks at player x/y, sends nearby npcs
client requests resource
server sends resource -tile- (resource id, width/height, resource data)
client puts object together with resource id at x/y
*/
        struct game_message
        {
            static const uint32_t keep_alive;
        };

        struct chat_message
        {

        };



        class game_world
        {
            public: typedef network::service network_service_type;
            public: typedef array<game_player> player_list_type;
            public: typedef math::vector<int> vector_type;
            public: typedef vector_type position_type;
            public: typedef vector_type rotation_type;

            public: void initialize()
            {
                auto self = *this;


                network::create_server<network::ngp_client>(self->network_service, 6110,
                [=](network::ngp_client client)
                {
                    std::cout << "[nextgen:engine:game_world] Server (port 6110) accepted NGP client." << std::endl;

std::cout << "10" << std::endl;

                    game_player player(client);

                    client.receive(
                    [=](network::ngp_message request)
                    {
                        std::cout << "[nextgen:engine:game_world] Received message from NGP client." << std::endl;

                        std::cout << "message_id: " << request->id << std::endl;
                        std::cout << "message_length: " << request->length << std::endl;

                        //player_list_type::iterator i = std::find_if(self->player_list.begin(), self->player_list.end(), [=](game_player player) -> bool
                        //{
                        //    return player->client == client;
                        //});

                        //game_player player = (*i);

                        switch(request->id)
                        {
                            case realm_message::keep_alive:
                            {


                                break;
                            }
                            case realm_message::user_login:
                            {
                                std::cout << "[nextgen:engine:game_world] User has logged in." << std::endl;

                                player->id = random(10000, 99999);
                                player->name = "Undefined";
                                player->position = position_type(1620, -4690);
                                player->rotation = rotation_type::zero();


                                {
                                    network::ngp_message old_message;

                                    old_message->id = realm_message::create_player;
                                    old_message->data << player->id << player->name.length() << player->name << player->position.x() << player->position.y() << player->rotation.x() << player->rotation.y() << false;

                                    for(player_list_type::iterator i = self->player_list.begin(), l = self->player_list.end(); i != l; ++i)
                                    {
                                        game_player old_player = (*i);

                                        // show the old players for the new player
                                        {
                                            network::ngp_message message;

                                            message->id = realm_message::create_player;
                                            message->data << old_player->id << old_player->name.length() << old_player->name << old_player->position.x() << old_player->position.y() << old_player->rotation.x() << old_player->rotation.y() << false;

                                            client.send(message);
                                        }

                                        // show the new player for old players
                                        {

                                            old_player->client.send(old_message);
                                        }
                                    }
                                }

                                self->player_list.push_back(player);

                                // show the new player to self
                                {
                                    network::ngp_message message;

                                    message->id = realm_message::create_player;
                                    message->data << player->id << player->name.length() << player->name << player->position.x() << player->position.y() << player->rotation.x() << player->rotation.y() << true;

                                    client.send(message);
                                }

                                break;
                            }
                            case realm_message::create_player:
                            {

                                break;
                            }
                            case realm_message::update_player_rotation:
                            {

                                int step = 10;

                                int x, y;

                                request->data >> x >> y;

                                std::cout << "player rot: " << x << " / " << y << std::endl;

                                //if(player->rotation.x() != x && player->rotation.y() != y)
                                //{
                                    player->rotation.x(x);
                                    player->rotation.y(y);


                                    //if(player->rotation->is_changed())
                                    //{
                                        if(player->rotation.x() == 1)
                                            player->position.x(player->position.x() + step);
                                        else if(player->rotation.x() == -1)
                                            player->position.x(player->position.x() - step);
                                        else if(player->rotation.y() == 1)
                                            player->position.y(player->position.y() + step);
                                        else if(player->rotation.y() == -1)
                                            player->position.y(player->position.y() - step);


                                   //}
    std::cout << "player pos: " << player->position.x() << " / " << player->position.y() << std::endl;

                                    {
                                        if(player->position.is_changed())
                                        {
                                            network::ngp_message message;

                                            message->id = realm_message::update_player_position;
                                            message->data << player->id << player->position.x() << player->position.y();

                                            for(player_list_type::iterator i = self->player_list.begin(), l = self->player_list.end(); i != l; ++i)
                                            {
                                                game_player player = (*i);

                                                player->client.send(message);
                                            }
                                        }
                                    }
                                //}
/*
                                {
                                    network::ngp_message message;

                                    message->id = realm_message::update_player_position;
                                    message->data << player->id << player->position.x() << player->position.y();

                                    client.send(message);
                                }*/


                                break;
                            }
                        }
                    },
                    [=]()
                    {
                        std::cout << "Removing player #" << player->id << std::endl;

                        auto i = std::find_if(self->player_list.begin(), self->player_list.end(), [=](game_player p) -> bool
                        {
                            return p == player;
                        });

                        if(i != self->player_list.end())
                        {
                            self->player_list.erase(i);

                            network::ngp_message message;

                            message->id = realm_message::remove_player;
                            message->data << player->id;

                            for(player_list_type::iterator i = self->player_list.begin(), l = self->player_list.end(); i != l; ++i)
                            {
                                game_player player = (*i);

                                std::cout << "Telling player #" << player->id << std::endl;

                                player->client.send(message);
                            }
                        }
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
/*
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
    });*/

    nextgen::network::create_server<nextgen::network::xml_client>(self->network_service, 843,
    [=](nextgen::network::xml_client client)
    {
        std::cout << "[stoke:game:run:server] Server (port 843) accepted XML client." << std::endl;

        client.receive(
        [=](nextgen::network::xml_message request)
        {
            std::cout << "[stoke:game:run:server] Received data from XML client (length: " << request->data.length() << ")" << std::endl;

            std::cout << request->data << std::endl;

            if(request->data.find("<policy-file-request/>") != nextgen::string::npos)
            {
                nextgen::network::xml_message response;

                response->data = "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"6110-6112\" /></cross-domain-policy>\0";

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
