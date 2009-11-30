class game_npc extends game_object
{
	public var id:int;
	public var name:String;

	public function game_npc()
	{
		this.id = 0;
		this.name = "Undefined";
	}
	
		
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

	public override function get changed():Boolean
	{
		if(this.model.changed)
			return true;
			
		return false;
	}
	
	public override function set changed(changed_:Boolean):void
	{
		this.model.changed = changed_;
	}
}