import flash.utils.ByteArray;

class game_npc extends game_object
{
	public var id:int;
	public var name:String;
	public var model:graphics_model_2d;

	public function game_npc()
	{
		this.id = 0;
		this.name = "Undefined";
		
		this.model = new graphics_model_2d();
	}
	
	public function update(time:core_timestamp):void
	{
		//if(this.changed)
		//{
			/*var step:Number = time.elapsed_real_time / 10;

			if(this.model.rotation.x == 1)
			{
				this.model.position.x += step;
			}
			else if(this.model.rotation.x == -1)
			{
				this.model.position.x -= step;
			}
			else if(this.model.rotation.y == 1)
			{
				this.model.position.y += step;
			}
			else if(this.model.rotation.y == -1)
			{
				this.model.position.y -= step;
			}*/
			
			//this.changed = false;
		//}
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

	public function get position():math_vector2
	{
		return this.model.position;
	}
	
	public function set position(position_:math_vector2):void
	{
		this.model.position = position_;
	}
	
	public function get rotation():math_vector2
	{
		return this.model.rotation;
	}
	
	
	
}