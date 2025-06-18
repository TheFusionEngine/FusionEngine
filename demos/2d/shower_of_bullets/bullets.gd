
extends Node2D

# This demo is an example of controling a high number of 2D objects with logic and collision without using scene nodes.
# This technique is a lot more efficient than using instancing and nodes, but requires more programming and is less visual

const BULLET_COUNT = 500
const SPEED_MIN = 20
const SPEED_MAX = 50

var bullets=[]
var shape

class Bullet:
	var pos = Vector2()
	var speed = 1.0
	var body = RID()
	

func _draw():

	var t = preload("res://bullet.png")
	var tofs = -t.get_size()*0.5
	for b in bullets:
		draw_texture(t,b.pos+tofs)	
		
	
func _process(delta):
	var width = get_viewport_rect().size.x*2.0
	var mat = Transform2D()
	for b in bullets:
		b.pos.x-=b.speed*delta
		if (b.pos.x < -30):
			b.pos.x+=width
		mat.o=b.pos		
		
		PhysicsServer2D.body_set_state(b.body,PhysicsServer2D.BODY_STATE_TRANSFORM,mat)
		
	update()
		
			
func _ready():

	shape = PhysicsServer2D.shape_create(PhysicsServer2D.SHAPE_CIRCLE)
	PhysicsServer2D.shape_set_data(shape,8) #radius

	for i in range(BULLET_COUNT):
		var b = Bullet.new()
		b.speed=rand_range(SPEED_MIN,SPEED_MAX)
		b.body = PhysicsServer2D.body_create(PhysicsServer2D.BODY_MODE_KINEMATIC)
		PhysicsServer2D.body_set_space(b.body,get_world_2d().get_space())
		PhysicsServer2D.body_add_shape(b.body,shape)
		
		b.pos = Vector2( get_viewport_rect().size * Vector2(randf()*2.0,randf()) ) #twice as long
		b.pos.x += get_viewport_rect().size.x # start outside
		var mat = Transform2D()
		mat.o=b.pos
		PhysicsServer2D.body_set_state(b.body,PhysicsServer2D.BODY_STATE_TRANSFORM,mat)
		
		bullets.append(b)
		
		
	set_process(true)
		
	
func _exit_tree():
	for b in bullets:
		PhysicsServer2D.free_rid(b.body)
	
	PhysicsServer2D.free_rid(shape)
	# Initalization here
	bullets.clear()
	
	pass


