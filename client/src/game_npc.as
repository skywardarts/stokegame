import flash.utils.ByteArray;

class game_npc extends game_object
{
	public var id:int;
	public var name:String;
	public var position:math_vector2;
	public var rotation:math_vector2;
	
	public var mc:MovieClip;
	
	public function game_npc(mc:MovieClip)
	{
		this.name = "Undefined";
		
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
	
	public function update():void
	{
		//if(this.position.changed)
		this.mc.x = this.position.x;
		this.mc.y = this.position.y * -1;
		
		//if(this.rotation.changed)
	}
	
}