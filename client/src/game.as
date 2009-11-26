package src
{
	import flash.display.MovieClip;
	import flash.system.Security;
	import flash.utils.ByteArray;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;

	public class game
	{
		public function game()
		{
			this.world = new game_world();
			
			this.chat_client = new chat_socket_stream();
			this.chat_client.add_event_handler(new keep_alive_event(), this.on_keep_alive);
			this.chat_client.add_event_handler(new update_player_position_event(), this.on_update_player_position);
			this.chat_client.add_event_handler(new create_player_event(), this.on_create_player);
			this.chat_client.add_event_handler(new update_player_event(), this.on_update_player);
			
			//this.realm_client = new flashx_network_client(new engine_protocol_realm_socket());
			//this.game_client = new flashx_network_client(new engine_protocol_game_socket());
			
			this.initialize();
		}
		
		public static function create(mc:MovieClip):game
		{
			var g:game = new game();
			
			core_application.initialize(mc);
			
			core_application.get_instance().add_component(g);
			
			return g;
		}
		
		public function initialize():void
		{
			//this.application.get_stage().addEventListener(KeyboardEvent.KEY_DOWN, this.on_key_down);
			//this.application.get_stage().addEventListener(KeyboardEvent.KEY_UP, this.on_key_up);
			
			//debug.log("[engine:input] Input detection started.");

			this.initialize_servers();
		}
		
		public function initialize_servers():void
		{
			var domain:String = "174.1.141.120";//this.application.get_domain();
			
			debug.log("[engine:chat] Attempting to connect (" + domain + ":" + 6110 + ").");
			
			Security.loadPolicyFile("xmlsocket://" + domain + ":" + "843");
			
			this.chat_client.connect(domain, 6110, connect_successful_handler, connect_failure_handler);
			
			var packet:network_packet = new network_packet();
			
			//this.chat_client.get_socket().send();
			
			//this.realm_client.connect(domain, 6111);
			//this.game_client.connect(domain, 6112);
			
			//var server:socket_stream = new socket_stream(
		}
		
		/*
		public function on_key_down(event:KeyboardEvent):void 
		{
			//debug.log("[engine:input] Key pressed: " + String.fromCharCode(event.charCode) + " (code: " + event.charCode + ")"); 
			
			this.keyboard.key_down(event);
		}
		
		public function on_key_up(event:KeyboardEvent):void 
		{
			this.keyboard.key_up(event);
		}*/
		
		public function on_keep_alive(event:keep_alive_event):void
		{
			debug.log("[engine:chat:event] Keep alive.");
		}
		
		public function on_create_player(event:create_player_event):void
		{
			debug.log("[engine:chat:event] Creating player... (#" + event.player.id + ")");
			
			if(event.control)
				this.player = event.player;
			
			this.world.add_player(event.player);
			
		}
		
		public function on_update_player(event:update_player_event):void
		{
			debug.log("[engine:chat:event] Updating player...");
		}
		
		public function on_update_player_position(event:update_player_position_event):void
		{
			var player:game_player = this.world.get_player(event.player_id);
			
			if(player != null)
			{
				debug.log("[engine:chat:event] Updating player position. (#" + player.id + ")");

				player.position = event.position;
			}
			else
			{
				debug.log("[engine:chat:event] Error: Trying to change position of undefined player.");
			}
		}
		
		public function update_server():void
		{
			//this.chat_client.update();
		}
		
		public function update_keyboard():void
		{
			var key_state:keyboard_state = core_application.get_keyboard().get_state();

			if(this.player != null)
			{
				this.player.rotation.x = 0;
				this.player.rotation.y = 0;
	
				if(key_state.is_key_down(key_code.w))
				{
					this.player.rotation.y = 1;
				}
				else if(key_state.is_key_down(key_code.d))
				{
					this.player.rotation.x = 1;
				}
				else if(key_state.is_key_down(key_code.s))
				{
					this.player.rotation.y = -1;
				}
				else if(key_state.is_key_down(key_code.a))
				{
					this.player.rotation.x = -1;
				}

				var message:update_player_rotation_event = new update_player_rotation_event();
				
				message.rotation = this.player.rotation;
	
				this.chat_client.send_message(message, 
				function():void
				{
					//debug.log("[engine:chat] Successfully sent message.");
				},
				function():void
				{
					//debug.log("[engine:chat] Failed to send message.");
				});
			}
		}
		
		public function connect_successful_handler():void
		{
			debug.log("[engine:chat] Successfully connected.");
			
			var message:login_event = new login_event();
			
			message.username = "xxxx";
			message.password = "xxxx";

			this.chat_client.send_message(message, 
			function():void
			{
				debug.log("[engine:chat] Successfully logged in.");
			},
			function():void
			{
				debug.log("[engine:chat] Failed to log in.");
			});
		}
		
		public function connect_failure_handler():void
		{
			debug.log("[engine:chat] Failed to connect.");
		}

		public function update(time:core_timestamp):void
		{
			if(core_application.get_keyboard().is_changed())
			{
				this.update_server();
				
				this.update_keyboard();
			}
			
			//if(this.player != null)
				//trace(this.player.position.x + "/" + this.player.position.y);
			
			this.world.update(time);
		}
		
		public function draw(device:graphics_device):void
		{
			this.world.draw(device);
		}
		
		private var chat_client:chat_socket_stream;
		//public var realm_client:network_client;
		//public var game_client:network_client;
		
		private var player:game_player = null;
		private var world:game_world;
	}
}

include "../../../flashx/src/include.as";


include "chat_socket_stream.as";
include "game_world.as";
include "game_object.as";
include "game_npc.as";
include "game_player.as";
include "game_monster.as";