extends Sprite2D

var scalex = 0
var scaley = 0

func _ready():
	set_process(true)
	
func _process(delta):
	if(scalex > 3):
		scalex = 0
		scaley = 0
		
	set_scale(Vector2(scalex, scaley))
	scalex += 0.04
	scaley += 0.04
	
	