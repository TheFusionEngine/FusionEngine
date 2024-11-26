extends Sprite2D

var rot = 0.1

func _ready():
	print("active")
	set_process(true)
	
func _process(delta):
	
	rotate(rot)