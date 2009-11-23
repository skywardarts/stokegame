import flash.utils.ByteArray;

class world_npc
{
	public var health:int;
	public var mana:int;
	public var name:String;
	public var mc:MovieClip;
	public var id:int;
	
	public function world_npc(mc:MovieClip)
	{
		this.health = 0;
		this.mana = 0;
		this.name = "None";
		
		this.mc = new MovieClip();
		//this.mc.x = 0; this.mc.y = 0;
		//this.mc.width = 64; this.mc.height = 64
		
		mc.addChild(this.mc);

		var txt:TextField = new TextField();
		txt.x = 0; txt.y = 0;
		txt.width = 64; txt.height = 20;
		txt.defaultTextFormat = new TextFormat("Verdana", 12, 0xFF0000, false, false, false);
		txt.selectable = false;
		txt.text = this.name;
		
		this.mc.addChild(txt);
		
		var bm:Bitmap = new Bitmap(new avatar(0, 0)); 
		bm.x = 0; bm.y = 20;
		bm.width = 34; bm.height = 34;
		
		
		this.mc.addChild(bm);
	}
	
	public function set x_position(x:Number):void 
	{
		this.mc.x = x;
	}

	public function get x_position():Number
	{
		return this.mc.x;
	}
	
	public function set y_position(y:Number):void 
	{
		this.mc.y = y * -1;
	}

	public function get y_position():Number
	{
		return this.mc.y;
	}
}