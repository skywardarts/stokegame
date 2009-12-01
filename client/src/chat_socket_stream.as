interface socket_event
{
	function pack():ByteArray;
	function unpack(ba:ByteArray):void;
	function clone():socket_event;
	function get id():int;
	//public static const id:int;
	/*
	public function pack():ByteArray
	{
		throw new core_abstract_method_error();
	}
	
	public function unpack(ba:ByteArray):void
	{
		throw new core_abstract_method_error();
	}
	
	public function copy():socket_event
	{
		throw new core_abstract_method_error();
	}
	
	public function get id():int
	{
		throw new core_abstract_method_error();
	}*/
}

class keep_alive_event implements socket_event
{
	//public static const id:int = 1;
	
	public static const open_status:int = 1;
	public static const close_status:int = 2;
	
	public var status_id:int;
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
		
		ba.writeInt(this.status_id);
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		this.status_id = ba.readInt();
	}
	
	public function clone():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 1;
	}
}


class login_event implements socket_event
{
	public var username:String;
	public var password:String;
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
		
		ba.writeInt(this.username.length);
		ba.writeUTFBytes(this.username);
		ba.writeInt(this.password.length);
		ba.writeUTFBytes(this.password);
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		
	}
	
	public function clone():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 2;
	}
}

class create_player_event implements socket_event
{
	public var player:game_player;
	public var control:Boolean;
	
	public function create_player_event()
	{
		this.player = new game_player();
		
	}
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
		
		ba.writeInt(this.player.id);
		ba.writeInt(this.player.name.length);
		ba.writeUTFBytes(this.player.name);
		ba.writeInt(this.player.position.x);
		ba.writeInt(this.player.position.y);
		ba.writeInt(this.player.rotation.x);
		ba.writeInt(this.player.rotation.y);
		ba.writeBoolean(this.control);
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		//var event:socket_event = new update_player_position_event();
trace("unpacking create player event");

		this.player.id = ba.readInt();
		trace(this.player.id);
		var player_name_length:int = ba.readInt();
		trace(player_name_length);
		this.player.name = ba.readUTFBytes(player_name_length);
		trace(this.player.name);
		this.player.position.x = ba.readInt();
		trace(this.player.position.x);
		this.player.position.y = ba.readInt();
		trace(this.player.position.y);
		this.player.rotation.x = ba.readInt();
		trace(this.player.rotation.x);
		this.player.rotation.y = ba.readInt();
		trace(this.player.rotation.y);
		this.control = ba.readBoolean();
		
		//return event;
	}
	
	public function clone():socket_event
	{
		return new create_player_event();
	}

	public function get id():int
	{
		return 3;
	}
}


class update_player_event implements socket_event
{
	public var player_id:int;
	public var data:ByteArray;
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
		
		ba.writeInt(this.player_id);
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		this.player_id = ba.readInt();
		
		ba.readBytes(this.data, ba.position);
	}
	
	public function clone():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 4;
	}
}


class remove_player_event implements socket_event
{
	public var player:game_player;
	public var data:ByteArray;
	
	public function remove_player_event()
	{
		this.player = new game_player();
	}
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
		
		ba.writeInt(this.player.id);
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		this.player.id = ba.readInt();
	}
	
	public function clone():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 5;
	}
}

class update_player_position_event implements socket_event
{
	public var player_id:int;
	public var position:math_vector2;
	
	public function update_player_position_event()
	{
		this.position = new math_vector2();
	}
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
		
		ba.writeInt(this.player_id);
		ba.writeInt(this.position.x);
		ba.writeInt(this.position.y);
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		//var event:socket_event = new update_player_position_event();
		
		this.player_id = ba.readInt();
		this.position.x = ba.readInt();
		this.position.y = ba.readInt();

		//return event;
	}
	
	public function clone():socket_event
	{
		return new update_player_position_event();
	}

	public function get id():int
	{
		return 41;
	}
}


class update_player_rotation_event implements socket_event
{
	public var rotation:math_vector2;
	
	public function update_player_rotation_event()
	{
		this.rotation = new math_vector2();
	}
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
trace("rot: " + this.rotation.x + " / " + this.rotation.y);
		ba.writeInt(this.rotation.x);
		ba.writeInt(this.rotation.y);
		//ba.writeInt(math_helper.random_int(0, 9999));
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		
	}
	
	public function clone():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 42;
	}
}



class chat_socket_stream
{

	//public static const channel_list:uint = 0x0B;
	//public static const chat_event:uint = 0x0F;
	//public static const ad_info:uint = 0x15;
	//public static const ping:uint = 0x25;
	//public static const File_Time_Info:uint = 0x33;
	//public static const Logon_Response:uint = 0x3a;
	//public static const Realm_Logon_Response:uint = 0x3e;
	//public static const Query_Realms_Response:uint = 0x40;
	//public static const News_Info:uint = 0x46;
	//public static const Extra_Work_Info:uint = 0x4a;
	//public static const Required_ExtraWork_Info:uint = 0x4c;
	//public static const Connection_Response:uint = 0x50;
	//public static const Auth_Response:uint = 0x51;
	//public static const Invalid:uint = 0x83;
	
	private var socket:network_socket;
	
	private var event_handler_list:Array;
	private var message_handler_list:Array;
	
	private var data:ByteArray;
	
	public function chat_socket_stream()
	{
		this.event_handler_list = new Array();
		this.message_handler_list = new Array();
		this.data = new ByteArray();
		
		this.add_message_handler(new keep_alive_event());
		this.add_message_handler(new login_event());
		this.add_message_handler(new create_player_event());
		this.add_message_handler(new remove_player_event());
		this.add_message_handler(new update_player_position_event());
		this.add_message_handler(new update_player_rotation_event());
	}
	
	public function connect(host:String, port:int, successful:Function, failure:Function):void
	{
		var self:chat_socket_stream = this;
		
		this.socket = new network_socket(host, port);
		
		this.socket.add_event_handler(network_socket.receive_successful_event, function(ba:ByteArray):void
		{
			var ba_length:int = ba.length;
			
			self.data.writeBytes(ba);

			self.data.position = self.data.position - ba_length;

			var processing:Boolean = true;
			
			while(processing)
			{
				
				
				if(self.data.bytesAvailable >= 8)
				{
					var message_id:int = self.data.readInt();
					var message_length:int = self.data.readInt();
					var message_data:ByteArray = new ByteArray();
					
					trace("Incoming event #" + message_id);
					trace("message_length: " + message_length);
					trace("bytes_avail: " + self.data.bytesAvailable);

					if(self.data.bytesAvailable > 0 && message_length > 0 && self.data.bytesAvailable >= message_length)
					{
						self.data.readBytes(message_data, 0, message_length);
						
						//self.message_handler_list[message_id]
						
						var event:socket_event = self.message_handler_list[message_id].clone();
						
						message_data.position = 0;
						
						event.unpack(message_data);
						
						if(self.event_handler_list[message_id] != null)
						{
							trace("Firing off event #" + message_id);
							self.event_handler_list[message_id](event);
						}
					}
					else
					{
						
						//self.data.position -= 8;
						
						//self.data.writeInt(message_id);
						//self.data.writeInt(message_length);

						processing = false;
					}
				}
				else
				{
					processing = false;
				}
	
				//for each(var callback:Function in this.callbacks[packet.message_id])
					//callback(packet);
				
				//i += message_length;
			}
		});
		
		this.socket.connect(
		function():void
		{
			successful();
		},
		function():void
		{
			failure();
		});
	}
	/*
	public function process_message(ba:ByteArray)
	{
		var message_id:int = ba.readInt();
		var message_length:int = ba.readInt();
		var message_data:ByteArray;
		
		ba.readBytes(message_data, message_length);
		
		this.message_handler_list[message_id].unpack(message_data);
		
		var event:socket_event = this.message_handler_list[message_id].copy();
		
		this.event_handler_list[event.id](event);
		
	}*/
	
	public function send_message(message:socket_event, successful:Function, failure:Function):void
	{
		var ba:ByteArray = new ByteArray();
		
		var message_ba:ByteArray = message.pack();
		
		ba.writeInt(message.id);
		ba.writeInt(message_ba.length);
		ba.writeBytes(message_ba);
		
		this.socket.send(ba, 
		function():void
		{
			successful();
		},
		function():void
		{
			failure();
		});
	}
	
	public function add_message_handler(event:socket_event):void
	{
		this.message_handler_list[event.id] = event;
	}
	
	public function add_event_handler(event:socket_event, handler:Function):void
	{
		this.event_handler_list[event.id] = handler;
	}
	
	public function remove_event_handler(event_id:int):void
	{
		this.event_handler_list[event_id] = null;
	}
	
	public function update():void
	{
		//this.socket.receive();
	}
	
}

