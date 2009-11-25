import flash.utils.ByteArray;

class game_npc extends game_object
{
	public var id:int;
	public var name:String;
	public var position:math_vector2;
	public var rotation:math_vector2;
	public var model:graphics_model_2d;

	public function game_npc()
	{
		this.id = 0;
		this.name = "Undefined";
		
	}
	
	public function update():void
	{
		//if(this.position.changed)
		//this.mc.x = this.position.x;
		//this.mc.y = this.position.y * -1;
		
		//if(this.rotation.changed)
	}
	/*
	public function draw(device:graphics_device):void
	{
		if(this.changed)
		{
			var bm:Bitmap = new Bitmap(new avatar(0, 0)); 
			bm.x = this.position.x; bm.y = this.position.y * -1;
			bm.width = 34; bm.height = 34;
			
			device.addChild(bm);
		}
	}*/
	
}