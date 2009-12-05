
typedef Point = {
	var x : Int;
	var y : Int;
}
typedef Point3D = {> Point,
	var z : Int;
}



class O1
{
	public function new()
	{
		
	}
}

class O2
{
	public function new()
	{
		
	}
}

typedef O4 = {
    function get1() : String;
    function get2() : String;
}

typedef O5 = {
    function get3() : String;
    function get4() : String;
}


interface GraphicsObjectBase
{
	function Render():Void;
	var x:Int;
}

class GraphicsBasicObject implements GraphicsObjectBase
{
	public var x:Int;
	public function new()
	{
		
	}
	
	public function Render():Void
	{
		trace("RENDER");
	}
}


interface PhysicsObjectBase
{
	function Collide():Void;
	var x:Int;
}

class PhysicsBasicObject implements PhysicsObjectBase
{
	public var x:Int;
	public function new()
	{
		
	}
	
	public function Collide():Void
	{
		trace("COLLIDE");
	}
}

//typedef Jarad = {>O1, >O2}

class GameObject implements GraphicsObjectBase, implements PhysicsObjectBase
{
	public var x:Int;
	public var graphics:GraphicsObjectBase;
	public var physics:PhysicsObjectBase;
	
	
	public function new(graphics:GraphicsObjectBase, physics:PhysicsObjectBase)
	{
		this.graphics = graphics;
		this.physics = physics;
		
	}
	
	public function Render():Void
	{
		this.graphics.Render();
	}
	
	public function Collide():Void
	{
		this.physics.Collide();
	}
	
	public function toString():String
	{
		return this.to_string();
	}
	
	public function to_string():String
	{
		return "[flaxe:object]";
	}
}


class Main
{
	public function new() {}
	
    public function run():Void
    {
		trace("[stokegame] Initializing client...");
		
		var o = new GameObject(new GraphicsBasicObject(), new PhysicsBasicObject());
		o.Render();
		o.Collide();
		trace(o);
		
		var mc:flash.display.MovieClip = flash.Lib.current;
		
		//var application = new flaxe.Application(mc);
		
		//var client = new stoke.Client(application);
		
		trace("[stokegame] Initialized client.");
    }
    
    public static function main():Void { new Main().run(); }
}

