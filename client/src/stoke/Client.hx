package stoke;

import stoke.RealmEvents;
import stoke.WorldEvents;
import stoke.ChatEvents;

class Client
{
	public var realm_server:stoke.RealmServer;
	public var world_server:stoke.WorldServer;
	public var chat_server:stoke.ChatServer;
	
	public var application:flaxe.Application;
	public var graphics_service:flaxe.graphics.Service;
	public var input_service:flaxe.input.Service;
	public var network_service:flaxe.network.Service;
	
	public var target_player:flaxe.game.Player;
	
	public var graphics_scene:flaxe.graphics.Scene<flaxe.graphics.Camera2>;
	public var physics_scene:flaxe.physics.Scene;
	
	public function new(application:flaxe.Application, graphics_service:flaxe.graphics.Service, input_service:flaxe.input.Service, network_service:flaxe.network.Service)
	{
		var self = this;
		
		this.application = application;
		this.graphics_service = graphics_service;
		this.input_service = input_service;
		this.network_service = network_service;
		
		this.application.load_event.add(this.initialize, []);
	}
	
	public function initialize():Void
	{
		var self = this;
		
		this.application.begin_update_event.add(this.update);
		
		// add information to the screen (fps, memory, latency, etc)
		var info = new flaxe.InformationComponent(this.application.layer, this.application.layer.stage.stageWidth - 185, 25, 200, 100);

		this.application.begin_update_event.add(info.start);
		this.application.end_update_event.add(info.stop);
		
		// subscribe to input events
		this.input_service.change_event.add(this.update_input);
		
		this.graphics_scene = new flaxe.graphics.Scene<flaxe.graphics.Camera2>();
		this.physics_scene = new flaxe.graphics.Scene();
		
		// attempt to initiate connection to realm
		this.realm_server = new stoke.RealmServer(this.application.domain, 6110);
		
		this.realm_server.connect(
		function():Void
		{
			trace("[stoke:realm_server] Successfully connected.");
			
			var e = new stoke.LoginEvent();
			
			e.username = "xxxx";
			e.password = "xxxx";

			self.realm_server.send(e, 
			function():Void
			{
				trace("[stoke:realm_server] Successfully logged in.");
				
				self.initialize_realm();
			},
			function():Void
			{
				trace("[stoke:realm_server] Failed to log in.");
			});
		},
		function():Void
		{
			trace("[stoke:realm_server] Failed to connect.");
		});
	}

	public function on_keep_alive():Void
	{
		trace("[stoke:realm_server] Keep alive.");
	}
	
	public function on_create_chat(event:stoke.CreateChatEvent)
	{
		trace("[stoke:realm_server] Creating chat... (#" + event.chat_id + ")");
		
		this.chat_server = new stoke.ChatServer(this.application.domain, 6111);
	}
	
	public function on_create_world(event:stoke.CreateWorldEvent)
	{
		trace("[stoke:realm_server] Creating world... (#" + event.world_id + ")");
		
		var self = this;
		
		// todo(daemn) handle multiple worlds, for instances and transitioning perhaps
		
		this.world_server = new stoke.WorldServer(this.application.domain, 6112);
		
		this.world_server.connect(
		function():Void
		{
			trace("[stoke:realm_server] Successfully connected.");
			
			var e = new stoke.AuthenticateSessionEvent();
			
			e.auth_code = event.auth_code;

			self.world_server.send(e, 
			function():Void
			{
				trace("[stoke:realm_server] Successfully authenticated world session.");

				self.initialize_world();
			},
			function():Void
			{
				trace("[stoke:realm_server] Failed to authenticate world session.");
			});
		},
		function():Void
		{
			trace("[stoke:realm_server] Failed to connect.");
		});
	}
	
	/*
	public function on_update_player_position(event:flaxe.game.events.UpdatePlayerPositionEvent):Void
	{
		var player:flaxe.game.Player = this.world.get_player(event.player_id);
		
		if(player != null)
		{
			trace("[stoke:game_server] Updating player position. (#" + player.id + ")");

			player.position.x = event.position.x;
			player.position.y = event.position.y;
		}
		else
		{
			trace("[stoke:game_server] Error: Trying to change position of undefined player.");
		}
	}*/
	
	public function initialize_realm():Void
	{
		this.realm_server.create_world_event.add(this.on_create_world);
	}
	
	public function initialize_world():Void
	{	
		this.world = new flaxe.game.World();
		
		this.application.begin_update_event.add(this.world.update);
		this.graphics_service.render_event.add(this.world.draw);
		
		this.world_server.create_player_event.add(this.on_create_player);
		this.world_server.remove_player_event.add(this.on_remove_player);
		this.world_server.create_object_event.add(this.on_create_object);
	}
	
	public function update_input():Void
	{
		var key_state:flaxe.input.KeyState = this.input_service.get_key_state();

		if(key_state.is_key_down(flaxe.input.KeyCode.Up) || key_state.is_key_down(flaxe.input.KeyCode.W)
			&& !(key_state.is_key_down(flaxe.input.KeyCode.Down) || key_state.is_key_down(flaxe.input.KeyCode.S)))
		{
			this.player.rotation.x = 0;
			this.player.rotation.y = 1;
		}
		else if(key_state.is_key_down(flaxe.input.KeyCode.Down) || key_state.is_key_down(flaxe.input.KeyCode.S)
			&& !(key_state.is_key_down(flaxe.input.KeyCode.Up) || key_state.is_key_down(flaxe.input.KeyCode.W)))
		{
			this.player.rotation.x = 0;
			this.player.rotation.y = -1;
		}
		
		if(key_state.is_key_down(flaxe.input.KeyCode.Left) || key_state.is_key_down(flaxe.input.KeyCode.A)
			&& !(key_state.is_key_down(flaxe.input.KeyCode.Right) || key_state.is_key_down(flaxe.input.KeyCode.D)))
		{
			this.player.rotation.x = -1;
			this.player.rotation.y = 0;
		}
		else if(key_state.is_key_down(flaxe.input.KeyCode.Right) || key_state.is_key_down(flaxe.input.KeyCode.D)
			&& !(key_state.is_key_down(flaxe.input.KeyCode.Left) || key_state.is_key_down(flaxe.input.KeyCode.A)))
		{
			this.player.rotation.x = 1;
			this.player.rotation.y = 0;
		}
		
		if(this.player.rotation.changed)
		{
			/*
			var message:flaxe.game.events.UpdatePlayerRotationEvent = new flaxe.game.events.UpdatePlayerRotationEvent();
			
			message.rotation.x = this.player.rotation.x;
			message.rotation.y = this.player.rotation.y;
			
			trace(this.player.rotation.x + "/" + this.player.rotation.x);
			trace(message.rotation.x + "/" + message.rotation.y);

			this.game_server.send(message, 
			function():Void
			{
				//trace("[engine:chat] Successfully sent message.");
			},
			function():Void
			{
				//trace("[engine:chat] Failed to send message.");
			});*/
		}
	}
	
	public function on_create_player(player:flaxe.game.Player):Void
	{
		trace("[stoke:world_server] Created player... (#" + player.id + ")");
		

		if(player.controlled)
		{
			this.target_player = player;
			this.scene.camera.target = player;
		}
		

	}
	
	public function on_create_object(object:flaxe.game.Object):Void
	{
		trace("[stoke:world_server] Created object... (#" + object.id + ")");
		

	}
	
	public function on_remove_player(player:flaxe.game.Player):Void
	{
		trace("[stoke:world_server] Removing player... (#" + player.id + ")");
		
		
	}
	
}
