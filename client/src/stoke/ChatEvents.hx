package stoke;

class ChatEvents
{
	public static inline var message:Int = 1;
}

class MessageEvent extends flaxe.Event, implements flaxe.SerializableBase
{
	public var player_id:Int;
	public var message_text:String;
	
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
		
		var message_text_length:Int = data.readInt();
		
		this.message_text = data.readUTFBytes(message_text_length);
	}
}


