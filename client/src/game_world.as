class game_world
{
	public var player_list:Array;
	public var scene:graphics_scene;
	public var map:game_object;
	
	public function game_world()
	{
		this.player_list = new Array();
		this.scene = new graphics_scene();
		this.map = new game_object();
		this.map.model.display = new kanto(0, 0); // todo(daemn) temp
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
	
	public function remove_player(player:game_player):void
	{
		player = this.player_list[player.id];
		
		this.scene.remove_model(player.model);
		
		delete this.player_list[player.id];
	}
	
	public function update(time:core_timestamp):void
	{
		
		
		for each(var player:game_player in this.player_list)
			player.update(time);
			
		this.scene.update(time);
		
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
		var p:Point = new Point((this.scene.camera.position.x - (device.width / 2)) * -1, (this.scene.camera.position.y - device.height / 2) * -1 * -1);
		trace("new cam pos: " + p);
		device.display.screen.copyPixels(this.map.model.display, this.map.model.display.rect, p);
		
		this.scene.draw(device);
	}
}