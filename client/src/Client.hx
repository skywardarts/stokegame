package;

class Client
{
	public function new()
	{
		this.world = new flashx.game.World();
		
		this.chat_client = new flashx.network.SocketStream();
		this.chat_client.add_event_handler(new flashx.network.events.KeepAliveEvent(), this.on_keep_alive);
		this.chat_client.add_event_handler(new flashx.network.events.UpdatePlayerPositionEvent(), this.on_update_player_position);
		this.chat_client.add_event_handler(new flashx.network.events.CreatePlayerEvent(), this.on_create_player);
		this.chat_client.add_event_handler(new flashx.network.events.RemovePlayerEvent(), this.on_remove_player);
		
		this.initialize();
	}
	 
	public static function create(mc:flash.display.MovieClip):Client
	{
		var c = new Client();
		
		c.application = flashx.Application.get_instance();
		
		c.application.initialize(mc);
		c.application.add_component(c);
		
		return c;
	}
	
	public function initialize():Void
	{
		//trace("[engine:input] Input detection started.");

		this.initialize_servers();
	}
	
	public function initialize_servers():Void
	{
		flash.system.Security.allowDomain("*");
		flash.system.Security.allowInsecureDomain("*"); 
		
		var domain:String = "stokegames.com";//this.application.get_domain();
		
		trace("[stoke:realm_server] Attempting to connect (" + domain + ":" + 6110 + ").");
		
		flash.system.Security.loadPolicyFile("xmlsocket://" + domain + ":" + "843");
		
		this.chat_client.connect(domain, 6110, connect_successful_handler, connect_failure_handler);
	}
	
	public function on_keep_alive(event:flashx.network.events.KeepAliveEvent):Void
	{
		trace("[stoke:realm_server] Keep alive.");
	}
	
	public function on_create_player(event:flashx.network.events.CreatePlayerEvent):Void
	{
		trace("[stoke:game_server] Creating player... (#" + event.player.id + ")");
		
		if(event.control)
		{
			this.player = event.player;
			this.world.scene.camera.model = this.player.model;
		}
		
		this.world.add_player(event.player);
		
	}
	
	public function on_remove_player(event:flashx.network.events.RemovePlayerEvent):Void
	{
		trace("[stoke:game_server] Removing player... (#" + event.player.id + ")");
		
		this.world.remove_player(event.player);
		
	}
	
	public function on_update_player_position(event:flashx.network.events.UpdatePlayerPositionEvent):Void
	{
		var player:flashx.game.Player = this.world.get_player(event.player_id);
		
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
	}
	
	public inline function update_server():Void
	{
		//this.chat_client.update();
	}
	
	public function update_keyboard():Void
	{
		var key_state:flashx.input.keyboard.State = this.application.keyboard.get_state();

		if(this.player != null)
		{
			if(key_state.is_key_down(flashx.input.keyboard.Code.up) || key_state.is_key_down(flashx.input.keyboard.Code.w)
				&& !(key_state.is_key_down(flashx.input.keyboard.Code.down) || key_state.is_key_down(flashx.input.keyboard.Code.s)))
			{
				this.player.rotation.x = 0;
				this.player.rotation.y = 1;
				trace("ZZZ" + this.player.rotation.x + "/" + this.player.rotation.x);
			}
			else if(key_state.is_key_down(flashx.input.keyboard.Code.down) || key_state.is_key_down(flashx.input.keyboard.Code.s)
				&& !(key_state.is_key_down(flashx.input.keyboard.Code.up) || key_state.is_key_down(flashx.input.keyboard.Code.w)))
			{
				this.player.rotation.x = 0;
				this.player.rotation.y = -1;
			}
			
			if(key_state.is_key_down(flashx.input.keyboard.Code.left) || key_state.is_key_down(flashx.input.keyboard.Code.a)
				&& !(key_state.is_key_down(flashx.input.keyboard.Code.right) || key_state.is_key_down(flashx.input.keyboard.Code.d)))
			{
				this.player.rotation.x = -1;
				this.player.rotation.y = 0;
			}
			else if(key_state.is_key_down(flashx.input.keyboard.Code.right) || key_state.is_key_down(flashx.input.keyboard.Code.d)
				&& !(key_state.is_key_down(flashx.input.keyboard.Code.left) || key_state.is_key_down(flashx.input.keyboard.Code.a)))
			{
				this.player.rotation.x = 1;
				this.player.rotation.y = 0;
			}
			
			if(this.player.rotation.changed)
			{
				var message:flashx.network.events.UpdatePlayerRotationEvent = new flashx.network.events.UpdatePlayerRotationEvent();
				
				message.rotation.x = this.player.rotation.x;
				message.rotation.y = this.player.rotation.y;
				
				trace(this.player.rotation.x + "/" + this.player.rotation.x);
				trace(message.rotation.x + "/" + message.rotation.y);
	
				this.chat_client.send_message(message, 
				function():Void
				{
					//trace("[engine:chat] Successfully sent message.");
				},
				function():Void
				{
					//trace("[engine:chat] Failed to send message.");
				});
			}
		}
	}
	
	public function connect_successful_handler():Void
	{
		trace("[stoke:realm_server] Successfully connected.");
		
		var message:flashx.network.events.LoginEvent = new flashx.network.events.LoginEvent();
		
		message.username = "xxxx";
		message.password = "xxxx";

		this.chat_client.send_message(message, 
		function():Void
		{
			trace("[stoke:realm_server] Successfully logged in.");
		},
		function():Void
		{
			trace("[stoke:realm_server] Failed to log in.");
		});
	}
	
	public function connect_failure_handler():Void
	{
		trace("[stoke:realm_server] Failed to connect.");
	}

	public function update(time:flashx.core.Timestamp):Void
	{
		if(this.application.keyboard.is_changed())
		{
			this.update_server();
			
			this.update_keyboard();
		}
		
		if(this.player != null)
		{
			//trace("play pos: " + this.player.position.x + "/" + this.player.position.y);
		}
			
		
		this.world.update(time);
	}
	
	public function draw(device:flashx.graphics.Device):Void
	{
		this.world.draw(device);
	}
	
	var chat_client:flashx.network.SocketStream;
	//public var realm_client:network_client;
	//public var game_client:network_client;
	
	var player:flashx.game.Player;
	var world:flashx.game.World;
	public var application:flashx.Application;
}
