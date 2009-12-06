package stoke;

import stoke.WorldEvents;

class WorldSession
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

								var object = new flaxe.game.Object();
								
								object.id = event_data.readInt();
								object.position.x = data.readInt();
								object.position.y = data.readInt();
								object.rotation.x = data.readInt();
								object.rotation.y = data.readInt();
								
								// find model
								var model_id = data.readInt();
								
								self.add_object(object);

								self.create_object_event.call_with([object]);
								
							case WorldEvents.update_object:
								trace("update_object");
								
							case WorldEvents.remove_object:
								trace("remove_object");
							
							case WorldEvents.create_player: 
								trace("create_player");

								var player = new flaxe.game.Player();
								
								player.id = data.readInt();
								
								var player_name_length:Int = data.readInt();
								
								player.name = data.readUTFBytes(player_name_length);
								player.position.x = data.readInt();
								player.position.y = data.readInt();
								player.rotation.x = data.readInt();
								player.rotation.y = data.readInt();
								player.controlled = data.readBoolean();
								
								self.add_player(player);
								
								self.create_player_event.call_with([player]);
							
							case WorldEvents.update_player:
								trace("update_player");
								
							case WorldEvents.remove_player:
								trace("remove_player");
								
								//var event = self.remove_player_event;
								
								//event.unpack(event_data);
								
								//var player = self.remove_player(self.find_player(event.player_id));
								
								//event.call_with([player]);
							
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
	
	public function update(time:flaxe.core.Timestamp):Void
	{
		for(player
	}
}
