import flash.utils.ByteArray;

class game_object
{
	public var model:graphics_model_2d;
	public var position:math_vector2;
	public var rotation:math_vector2;
	
	public function game_object()
	{
		this.model = new graphics_model_2d();
		this.position = new math_vector2();
		this.rotation = new math_vector2();
	}
	
	public function update(time:core_timestamp):void
	{
		this.model.position.x = this.position.x;
		this.model.position.y = this.position.y;
	}
	
	public function get changed():Boolean
	{
		if(this.model.changed)
			return true;
			
		return false;
	}
	
	public function set changed(changed_:Boolean):void
	{
		this.model.changed = changed_;
	}
}