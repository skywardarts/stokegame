
class Main
{
	public function new() {}
	
    public function run():Void
    {
		trace("[stokegame] Initializing client...");
		
		var mc:flash.display.MovieClip = flash.Lib.current;
		
		Client.create(mc);

		trace("[stokegame] Initialized client.");
    }
    
    public static function main():Void { new Main().run(); }
}

