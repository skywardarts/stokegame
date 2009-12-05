package stoke;

import stoke.RealmEvents;

class RealmServer
{
	public var keep_alive_event:KeepAliveEvent;
	public var create_world_event:CreateWorldEvent;
	
	public var connection:flaxe.network.Socket;
	
	public var host:String;
	public var port:Int;
	
	public function new(host:String, port:Int)
	{
		this.host = host;
		this.port = port;
		
		this.keep_alive_event = new KeepAliveEvent();
		this.create_world_event = new CreateWorldEvent();
		
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
							case WorldEvents.keep_alive:
								trace("keep_alive");

								self.keep_alive_event.unpack(event_data);
								self.keep_alive_event.call();
								
							case WorldEvents.user_login:
								trace("user_login");
								
							case WorldEvents.create_world:
								trace("create_world");

								self.create_world_event.unpack(event_data);
								self.create_world_event.call();
								
							case WorldEvents.update_world:
								trace("update_world");
								
							case WorldEvents.remove_world:
								trace("remove_world");
								
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
