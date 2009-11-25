package src
{
	import flash.display.MovieClip;
	import flash.system.Security;
	import flash.utils.ByteArray;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;

	
	public class device
	{
		public var application:core_application;
		
		public var chat_client:chat_socket_stream;
		//public var realm_client:network_client;
		//public var game_client:network_client;
		
		public var keyboard:keyboard_device;
		
		public var player:game_player;
		public var world:game_world;
		
		public function device(mc:MovieClip)
		{
			this.application = new core_application(mc);
			
			this.keyboard = new keyboard_device(mc);
			
			this.chat_client = new chat_socket_stream();
			//this.realm_client = new flashx_network_client(new engine_protocol_realm_socket());
			//this.game_client = new flashx_network_client(new engine_protocol_game_socket());
			
			
			this.chat_client.add_event_handler(new keep_alive_event(), this.on_keep_alive);
			this.chat_client.add_event_handler(new update_player_position_event(), this.on_update_player_position);
			this.chat_client.add_event_handler(new create_player_event(), this.on_create_player);
			this.chat_client.add_event_handler(new update_player_event(), this.on_update_player);
			
			this.initialize();
		}
		
		public function initialize():void
		{
			//this.application.get_stage().addEventListener(KeyboardEvent.KEY_DOWN, this.on_key_down);
			//this.application.get_stage().addEventListener(KeyboardEvent.KEY_UP, this.on_key_up);
			
			//debug.log("[engine:input] Input detection started.");

			this.application.get_root().addEventListener(Event.ENTER_FRAME, this.update);
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
			debug.log("[engine:chat:event] Creating player...");
			
			var player:game_player = new game_player(this.application.get_root());

			player.id = event.player_id;
			player.name = event.player_name;
			player.position.x = event.x_position;
			player.position.y = event.y_position;
			
			if(event.control)
				this.player = player;
			
			this.world.player_list[player.id] = player;
			
		}
		
		public function on_update_player(event:update_player_event):void
		{
			debug.log("[engine:chat:event] Updating player...");
		}
		
		public function on_update_player_position(event:update_player_position_event):void
		{
			var player:game_player = this.world.player_list[event.player_id];
			
			if(player != null)
			{
				debug.log("[engine:chat:event] Updating player position.");

				player.position.x = event.x_position;
				player.position.y = event.y_position;
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
			var key_state:keyboard_state = this.keyboard.get_state();

			var message:update_player_direction_event = new update_player_direction_event();

			if(key_state.is_key_down(key_code.w))
			{
				message.direction_id = update_player_direction_event.direction_up;
			}
			else if(key_state.is_key_down(key_code.d))
			{
				message.direction_id = update_player_direction_event.direction_right;
			}
			else if(key_state.is_key_down(key_code.s))
			{
				message.direction_id = update_player_direction_event.direction_down;
			}
			else if(key_state.is_key_down(key_code.a))
			{
				message.direction_id = update_player_direction_event.direction_left;
			}
			else
			{
				message.direction_id = update_player_direction_event.direction_none;
			}

			this.chat_client.send_message(message, 
			function():void
			{
				debug.log("[engine:chat] Successfully sent message.");
			},
			function():void
			{
				debug.log("[engine:chat] Failed to send message.");
			});
		}
		
		public function connect_successful_handler():void
		{
			var self:device = this;
			
			debug.log("[engine:chat] Successfully connected.");
			
			this.world = new game_world();
			
			var message:login_event = new login_event();
			
			message.username = "xxxx";
			message.password = "xxxx";

			self.chat_client.send_message(message, 
			function():void
			{
				debug.log("[engine:chat] Successfully logged in.");
			},
			function():void
			{
				debug.log("[engine:chat] Failed to log in.");
			});


/*
			var msg:String = "";
			
			this.chat_client.send(chat_socket_stream.enter_chat_response + msg.length + msg, 
			function() 
			{
				self.connected = true;
			},
			function()
			{
				self.connected = false;
			});*/


		}
		
		public function connect_failure_handler():void
		{
			debug.log("[engine:chat] Failed to connect.");
		}
		
		public function connect():void
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
		
		public function update(e:Event):void
		{
			if(this.keyboard.is_changed())
			{
				this.update_server();
				
				this.update_keyboard();
			}
		}
		
	}
}

include "../../../flashx/src/include.as";


include "chat_socket_stream.as";
include "game_world.as";
include "game_object.as";
include "game_npc.as";
include "game_player.as";
include "game_monster.as";