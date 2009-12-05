package stoke;

import stoke.WorldEvents;

class WorldServer
{
	public var create_player_event:CreatePlayerEvent;
	public var remove_player_event:RemovePlayerEvent;
	public var create_object_event:CreateObjectEvent;
	
	public var connection:flaxe.network.Socket;
	
	public var host:String;
	public var port:Int;
	
	public function new(host:String, port:Int)
	{
		this.host = host;
		this.port = port;
		
		this.create_player_event = new CreatePlayerEvent();
		this.remove_player_event = new RemovePlayerEvent();
		this.create_object_event = new CreateObjectEvent();
		
		this.connection = new flaxe.network.Socket(host, port);
	}
	
	public function connect(successful:Void->Void, failure:Void->Void):Void
	{
		var self = this;
		
		// todo(daemn) check connected
		
		this.connection.receive_successful_event.add(function(data:flash.utils.ByteArray)
		{
			var processing = true;
			
			while(processing)
			{
				if(data.bytesAvailable > 0)
				{
					var event_id = data.readInt();
					var event_length = data.readInt();
					var event_data = new flash.utils.ByteArray();
					
					var available:Int = data.bytesAvailable;
					
					trace("Incoming event #" + event_id);
					trace("message_length: " + event_length);
					trace("bytes_avail: " + data.bytesAvailable);
					
					if(available > 0 && event_length > 0 && available >= event_length)
					{
						data.readBytes(event_data, 0, event_length);

						event_data.position = 0;
					
						trace("Firing off event #" + event_id);
						
						switch(event_id)
						{
							case WorldEvents.create_object:
								trace("create_object");

								var event = self.create_object_event;

								event.unpack(event_data);

								var object = new flaxe.game.Object();
								
								object.id = event.object_id;
								object.position.x = event.object_position_x;
								object.position.y = event.object_position_y;
								object.rotation.x = event.object_rotation_x;
								object.rotation.y = event.object_rotation_y;
								
								self.add_object(object);

								event.call_with([object]);
								
							case WorldEvents.update_object:
								trace("update_object");
								
							case WorldEvents.remove_object:
								trace("remove_object");
							
							case WorldEvents.create_player: 
								trace("create_player");

								var event = self.create_player_event;

								event.unpack(event_data);
								
								var player = new flaxe.game.Player();
								
								player.id = event.player_id;
								player.name = event.player_name;
								player.position.x = event.player_position_x;
								player.position.y = event.player_position_y;
								player.rotation.x = event.player_rotation_x;
								player.rotation.y = event.player_rotation_y;
								player.controlled = event.control;
								
								self.add_player(player);
								
								event.call_with([player]);
							
							case WorldEvents.update_player:
								trace("update_player");
								
							case WorldEvents.remove_player:
								trace("remove_player");
								
								var event = self.remove_player_event;
								
								event.unpack(event_data);
								
								var player = self.remove_player(self.find_player(event.player_id));
								
								event.call_with([player]);
							
							default:
								trace("default");
						}
					}
					else
					{
						trace("BOOOOOOOOOOOOM");
						
						data.position = 0;
						data.writeInt(event_id);
						data.writeInt(event_length);
						
						processing = false;
					}
					

				}
				else
				{
					processing = false;
				}
			}
		});

		this.connection.connect(
		function():Void
		{
			successful();
		},
		function():Void
		{
			failure();
		});
	}
	
	public function send(event:Dynamic, successful:Void->Void, failure:Void->Void):Void
	{
		var data = new flash.utils.ByteArray();
		
		event.pack(data);
		
		this.connection.send(data, 
		function():Void
		{
			successful();
		},
		function():Void
		{
			failure();
		});
	}
}
