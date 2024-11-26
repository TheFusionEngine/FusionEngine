extends RigidBody3D

var dir = Vector3()
export var speed = 6

func _ready():
	set_process(true)
	
func _process(delta):

	if (Input.is_action_pressed("ui_up")):
		dir.y -= speed
	if (Input.is_action_pressed("ui_down")):
		dir.y += speed
	if (Input.is_action_pressed("ui_left")):
		dir.x -= speed
	if (Input.is_action_pressed("ui_right")):
		dir.x += speed
		
	dir = dir.normalized() * speed
	
	set_linear_velocity(Vector3(dir.x, 0, dir.y))
	dir = Vector3()
	