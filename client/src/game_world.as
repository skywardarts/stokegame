class game_world
{
	public var player_list:Array;
	public var scene:graphics_scene;
	
	public function game_world()
	{
		this.player_list = new Array();
		this.scene = new graphics_scene();
		
	}
	
	public function get_player(id:int):game_player
	{
		return this.player_list[id];
	}
	
	public function add_player(player:game_player):void
	{
		this.player_list[player.id] = player;
			
		this.scene.add_model(player.model);
	}
	
	public function update(time:core_timestamp):void
	{
		for each(var player:game_player in this.player_list)
			player.update(time);
		
/*
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
		
		this.mc.addChild(bm);*/
		
		
	}
	
	public function draw(device:graphics_device):void
	{
		this.scene.draw(device);
	}
}