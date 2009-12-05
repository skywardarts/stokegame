package stoke;

class WorldEvents
{
	public static inline var keep_alive:Int = 1;
	public static inline var user_login:Int = 2;
	public static inline var create_world:Int = 3;
	public static inline var create_chat:Int = 6;
}

class KeepAliveEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var status_id:Int;
	
	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{
		data.writeInt(this.status_id);
		
		var length = data.length;
		
		data.position = 0;
		data.writeInt(WorldEvents.keep_alive);
		data.writeInt(length);
	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		this.status_id = data.readInt();
	}
}


class LoginEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var username:String;
	public var password:String;
	
	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{
		data.writeInt(this.username.length);
		data.writeUTFBytes(this.username);
		data.writeInt(this.password.length);
		data.writeUTFBytes(this.password);
		
		var length = data.length;
		
		data.position = 0;
		data.writeInt(WorldEvents.user_login);
		data.writeInt(length);
	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		
	}
}

class CreateWorldEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var world_id:Int;
	public var auth_code:Int;
	
	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{

	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		this.world_id = data.readInt();
		this.auth_code = data.readInt();
	}
}

class CreateChatEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var chat_id:Int;

	public function new()
	{
		super();
	}
	
	public inline function pack(data:flash.utils.ByteArray):Void
	{

	}
	
	public inline function unpack(data:flash.utils.ByteArray):Void
	{
		this.chat_id = data.readInt();
	}
}


