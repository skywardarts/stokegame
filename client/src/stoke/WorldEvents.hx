package stoke;

class WorldEvents
{
	public static inline var authenticate_session:Int = 1;
	public static inline var create_object:Int = 6;
	public static inline var update_object:Int = 7;
	public static inline var remove_object:Int = 8;
	public static inline var create_player:Int = 9;
	public static inline var update_player:Int = 10;
	public static inline var remove_player:Int = 11;
	public static inline var chat_message:Int = 12;
	public static inline var load_asset:Int = 13;
}

class AuthenticateSessionEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var auth_code:Int;
	public var status:Int;
	
	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{
		data.writeInt(this.auth_code);
	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		this.status = data.readInt();
	}
}


class CreatePlayerEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var player_id:Int;
	public var player_name:String;
	public var player_position_x:Int;
	public var player_position_y:Int;
	public var player_rotation_x:Int;
	public var player_rotation_y:Int;
	public var control:Bool;
	
	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{

	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		this.player_id = data.readInt();

		var player_name_length:Int = data.readInt();
		
		this.player_name = data.readUTFBytes(player_name_length);
		this.player_position_x = data.readInt();
		this.player_position_y = data.readInt();
		this.player_rotation_x = data.readInt();
		this.player_rotation_y = data.readInt();
		this.control = data.readBoolean();
	}
}

class RemovePlayerEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var player_id:Int;

	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{

	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		this.player_id = data.readInt();
	}
}

class CreateObjectEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var object_id:Int;
	public var object_position_x:Int;
	public var object_position_y:Int;
	public var object_rotation_x:Int;
	public var object_rotation_y:Int;
	public var model_id:Int;
	
	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{

	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		this.object_id = data.readInt();
		this.object_position_x = data.readInt();
		this.object_position_y = data.readInt();
		this.object_rotation_x = data.readInt();
		this.object_rotation_y = data.readInt();
		this.model_id = data.readInt();
	}
}


