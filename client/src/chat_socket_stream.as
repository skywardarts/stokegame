﻿interface socket_event
{
	function pack():ByteArray;
	function unpack(ba:ByteArray):void;
	function copy():socket_event;
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
	
	public function copy():socket_event
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
	
	public function copy():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 2;
	}
}

class update_player_position_event implements socket_event
{
	public var player_id:int;
	public var position:math_vector2;
	
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
	
	public function copy():socket_event
	{
		var e:update_player_position_event = new update_player_position_event();
		
		e.player_id = this.player_id;
		e.position.x = this.position.x;
		e.position.y = this.position.y;
		
		return e;
	}

	public function get id():int
	{
		return 3;
	}
}

class update_player_direction_event implements socket_event
{
	public var direction:math_vector2;
	
	public function update_player_direction_event()
	{
		this.direction = new math_vector2();
	}
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();

		ba.writeInt(this.direction.x);
		ba.writeInt(this.direction.y);
		//ba.writeInt(math_helper.random_int(0, 9999));
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		
	}
	
	public function copy():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 7;
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
	
	public function copy():socket_event
	{
		return this;
	}
	
	public function get id():int
	{
		return 6;
	}
}


class create_player_event implements socket_event
{
	//public static const id:int = 3;
	
	public var player:game_player;
	public var position:math_vector2; 
	public var rotation:math_vector2;
	public var control:Boolean;
	
	public function pack():ByteArray
	{
		var ba:ByteArray = new ByteArray();
		
		ba.writeInt(this.player.id);
		ba.writeInt(this.player.name.length);
		ba.writeUTFBytes(this.player.name);
		ba.writeInt(this.position.x);
		ba.writeInt(this.position.y);
		ba.writeInt(this.rotation.x);
		ba.writeInt(this.rotation.y);
		ba.writeBoolean(this.control);
		
		return ba;
	}
	
	public function unpack(ba:ByteArray):void
	{
		//var event:socket_event = new update_player_position_event();
		
		this.player.id = ba.readInt();
		var player_name_length:int = ba.readInt();
		this.player.name = ba.readUTFBytes(player_name_length);
		this.position.x = ba.readInt();
		this.position.y = ba.readInt();
		this.rotation.x = ba.readInt();
		this.rotation.y = ba.readInt();
		this.control = ba.readBoolean();
		
		//return event;
	}
	
	public function copy():socket_event
	{
		var e:create_player_event = new create_player_event();
		
		e.player = this.player;
		e.position = this.position;
		e.rotation = this.rotation;
		e.control = this.control;
		
		return e;
	}

	public function get id():int
	{
		return 5;
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
		this.add_message_handler(new update_player_position_event());
		this.add_message_handler(new update_player_direction_event());
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

					if(self.data.bytesAvailable >= message_length)
					{
						self.data.readBytes(message_data, 0, message_length);
						
						//self.message_handler_list[message_id]
						
						var event:socket_event = self.message_handler_list[message_id].copy();
						
						event.unpack(message_data);
						
						if(self.event_handler_list[message_id] != null)
							self.event_handler_list[message_id](event);
					}
					else
					{
						
						self.data.position -= 8;
						
						self.data.writeInt(message_id);
						self.data.writeInt(message_length);

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

