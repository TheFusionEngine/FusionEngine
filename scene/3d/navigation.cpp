#include "navigation.h"

void Navigation::_navmesh_link(int p_id) {

	ERR_FAIL_COND(!navmesh_map.has(p_id));
	NavMesh &nm=navmesh_map[p_id];
	ERR_FAIL_COND(nm.linked);

	print_line("LINK");

	DVector<Vector3> vertices=nm.navmesh->get_vertices();
	int len = vertices.size();
	if (len==0)
		return;

	DVector<Vector3>::Read r=vertices.read();

	for(int i=0;i<nm.navmesh->get_polygon_count();i++) {

		//build

		List<Polygon>::Element *P=nm.polygons.push_back(Polygon());
		Polygon &p=P->get();
		p.owner=&nm;

		Vector<int> poly = nm.navmesh->get_polygon(i);
		int plen=poly.size();
		const int *indices=poly.ptr();
		bool valid=true;
		p.edges.resize(plen);

		Vector3 center;

		for(int j=0;j<plen;j++) {

			int idx = indices[j];
			if (idx<0 || idx>=len) {
				valid=false;
				break;
			}

			Polygon::Edge e;
			Vector3 ep=nm.xform.xform(r[idx]);
			center+=ep;
			e.point=_get_point(ep);
			p.edges[j]=e;
		}

		if (!valid) {
			nm.polygons.pop_back();
			ERR_CONTINUE(!valid);
			continue;
		}

		p.center=center/plen;

		//connect

		for(int j=0;j<plen;j++) {

			int next = (j+1)%plen;
			EdgeKey ek(p.edges[j].point,p.edges[next].point);

			Map<EdgeKey,Connection>::Element *C=connections.find(ek);
			if (!C) {

				Connection c;
				c.A=&p;
				c.A_edge=j;
				c.B=NULL;
				c.B_edge=-1;
				connections[ek]=c;
			} else {

				if (C->get().B!=NULL) {
					print_line(String()+_get_vertex(ek.a)+" -> "+_get_vertex(ek.b));
				}
				ERR_CONTINUE(C->get().B!=NULL); //wut

				C->get().B=&p;
				C->get().B_edge=j;
				C->get().A->edges[C->get().A_edge].C=&p;
				C->get().A->edges[C->get().A_edge].C_edge=j;;
				p.edges[j].C=C->get().A;
				p.edges[j].C_edge=C->get().A_edge;
				//connection successful.
			}
		}
	}

	nm.linked=true;

}


void Navigation::_navmesh_unlink(int p_id) {

	ERR_FAIL_COND(!navmesh_map.has(p_id));
	NavMesh &nm=navmesh_map[p_id];
	ERR_FAIL_COND(!nm.linked);

	print_line("UNLINK");

	for (List<Polygon>::Element *E=nm.polygons.front();E;E=E->next()) {


		Polygon &p=E->get();

		int ec = p.edges.size();
		Polygon::Edge *edges=p.edges.ptr();

		for(int i=0;i<ec;i++) {
			int next = (i+1)%ec;

			EdgeKey ek(edges[i].point,edges[next].point);
			Map<EdgeKey,Connection>::Element *C=connections.find(ek);
			ERR_CONTINUE(!C);
			if (C->get().B) {
				//disconnect

				C->get().B->edges[C->get().B_edge].C=NULL;
				C->get().B->edges[C->get().B_edge].C_edge=-1;
				C->get().A->edges[C->get().A_edge].C=NULL;
				C->get().A->edges[C->get().A_edge].C_edge=-1;

				if (C->get().A==&E->get()) {

					C->get().A=C->get().B;
					C->get().A_edge=C->get().B_edge;
				}
				C->get().B=NULL;
				C->get().B_edge=-1;

			} else {
				connections.erase(C);
				//erase
			}
		}
	}

	nm.polygons.clear();

	nm.linked=false;


}


int Navigation::navmesh_create(const Ref<NavigationMesh>& p_mesh, const Transform3D& p_xform, Object *p_owner) {

	int id = last_id++;
	NavMesh nm;
	nm.linked=false;
	nm.navmesh=p_mesh;
	nm.xform=p_xform;
	nm.owner=p_owner;
	navmesh_map[id]=nm;

	_navmesh_link(id);

	return id;
}

void Navigation::navmesh_set_transform(int p_id, const Transform3D& p_xform){

	ERR_FAIL_COND(!navmesh_map.has(p_id));
	NavMesh &nm=navmesh_map[p_id];
	if (nm.xform==p_xform)
		return; //bleh
	_navmesh_unlink(p_id);
	nm.xform=p_xform;
	_navmesh_link(p_id);



}
void Navigation::navmesh_remove(int p_id){

	ERR_FAIL_COND(!navmesh_map.has(p_id));
	_navmesh_unlink(p_id);
	navmesh_map.erase(p_id);

}

void Navigation::_clip_path(Vector<Vector3>& path, Polygon *from_poly, const Vector3& p_to_point, Polygon* p_to_poly) {

	Vector3 from = path[path.size()-1];

	if (from.distance_to(p_to_point)<CMP_EPSILON)
		return;
	Plane cut_plane;
	cut_plane.normal = (from-p_to_point).cross(up);
	if (cut_plane.normal==Vector3())
		return;
	cut_plane.normal.normalize();
	cut_plane.d = cut_plane.normal.dot(from);


	while(from_poly!=p_to_poly) {

		int pe = from_poly->prev_edge;
		Vector3 a = _get_vertex(from_poly->edges[pe].point);
		Vector3 b = _get_vertex(from_poly->edges[(pe+1)%from_poly->edges.size()].point);

		from_poly=from_poly->edges[pe].C;
		ERR_FAIL_COND(!from_poly);

		if (a.distance_to(b)>CMP_EPSILON) {

			Vector3 inters;
			if (cut_plane.intersects_segment(a,b,&inters)) {
				if (inters.distance_to(p_to_point)>CMP_EPSILON && inters.distance_to(path[path.size()-1])>CMP_EPSILON) {
					path.push_back(inters);
				}
			}
		}
	}
}

Vector<Vector3> Navigation::get_simple_path(const Vector3& p_start, const Vector3& p_end, bool p_optimize) {


	Polygon *begin_poly=NULL;
	Polygon *end_poly=NULL;
	Vector3 begin_point;
	Vector3 end_point;
	float begin_d=1e20;
	float end_d=1e20;


	for (Map<int,NavMesh>::Element*E=navmesh_map.front();E;E=E->next()) {

		if (!E->get().linked)
			continue;
		for(List<Polygon>::Element *F=E->get().polygons.front();F;F=F->next()) {

			Polygon &p=F->get();
			for(int i=2;i<p.edges.size();i++) {

				Face3 f(_get_vertex(p.edges[0].point),_get_vertex(p.edges[i-1].point),_get_vertex(p.edges[i].point));
				Vector3 spoint = f.get_closest_point_to(p_start);
				float dpoint = spoint.distance_to(p_start);
				if (dpoint<begin_d) {
					begin_d=dpoint;
					begin_poly=&p;
					begin_point=spoint;
				}

				spoint = f.get_closest_point_to(p_end);
				dpoint = spoint.distance_to(p_end);
				if (dpoint<end_d) {
					end_d=dpoint;
					end_poly=&p;
					end_point=spoint;
				}
			}

			p.prev_edge=-1;
		}
	}

	if (!begin_poly || !end_poly) {

		//print_line("No Path Path");
		return Vector<Vector3>(); //no path
	}

	if (begin_poly==end_poly) {

		Vector<Vector3> path;
		path.resize(2);
		path[0]=begin_point;
		path[1]=end_point;
		//print_line("Direct Path");
		return path;
	}


	bool found_route=false;

	List<Polygon*> open_list;

	for(int i=0;i<begin_poly->edges.size();i++) {

		if (begin_poly->edges[i].C) {

			begin_poly->edges[i].C->prev_edge=begin_poly->edges[i].C_edge;
			begin_poly->edges[i].C->distance=begin_poly->center.distance_to(begin_poly->edges[i].C->center);
			open_list.push_back(begin_poly->edges[i].C);

			if (begin_poly->edges[i].C==end_poly) {
				found_route=true;
			}
		}
	}


	while(!found_route) {

		if (open_list.size()==0) {
		//	print_line("NOU OPEN LIST");
			break;
		}
		//check open list

		List<Polygon*>::Element *least_cost_poly=NULL;
		float least_cost=1e30;

		//this could be faster (cache previous results)
		for (List<Polygon*>::Element *E=open_list.front();E;E=E->next()) {

			Polygon *p=E->get();


			float cost=p->distance;
			cost+=p->center.distance_to(end_point);

			if (cost<least_cost) {

				least_cost_poly=E;
				least_cost=cost;
			}
		}


		Polygon *p=least_cost_poly->get();
		//open the neighbours for search

		for(int i=0;i<p->edges.size();i++) {


			Polygon::Edge &e=p->edges[i];

			if (!e.C)
				continue;

			float distance = p->center.distance_to(e.C->center) + p->distance;

			if (e.C->prev_edge!=-1) {
				//oh this was visited already, can we win the cost?

				if (e.C->distance>distance) {

					e.C->prev_edge=e.C_edge;
					e.C->distance=distance;
				}
			} else {
				//add to open neighbours

				e.C->prev_edge=e.C_edge;
				e.C->distance=distance;
				open_list.push_back(e.C);

				if (e.C==end_poly) {
					//oh my reached end! stop algorithm
					found_route=true;
					break;

				}

			}
		}

		if (found_route)
			break;

		open_list.erase(least_cost_poly);
	}

	if (found_route) {

		Vector<Vector3> path;

		if (p_optimize) {
			//string pulling

			Polygon *apex_poly=end_poly;
			Vector3 apex_point=end_point;
			Vector3 portal_left=apex_point;
			Vector3 portal_right=apex_point;
			Polygon *left_poly=end_poly;
			Polygon *right_poly=end_poly;
			Polygon *p=end_poly;
			path.push_back(end_point);

			while(p) {

				Vector3 left;
				Vector3 right;

#define CLOCK_TANGENT(m_a,m_b,m_c) ( ((m_a)-(m_c)).cross((m_a)-(m_b)) )

				if (p==begin_poly) {
					left=begin_point;
					right=begin_point;
				} else {
					int prev = p->prev_edge;
					int prev_n = (p->prev_edge+1)%p->edges.size();
					left = _get_vertex(p->edges[prev].point);
					right = _get_vertex(p->edges[prev_n].point);

					if (CLOCK_TANGENT(apex_point,left,(left+right)*0.5).dot(up) < 0){
						SWAP(left,right);
					}
				}

				bool skip=false;


				if (CLOCK_TANGENT(apex_point,portal_left,left).dot(up) >= 0){
					//process
					if (portal_left==apex_point || CLOCK_TANGENT(apex_point,left,portal_right).dot(up) > 0) {
						left_poly=p;
						portal_left=left;
					} else {

						_clip_path(path,apex_poly,portal_right,right_poly);

						apex_point=portal_right;						
						p=right_poly;
						left_poly=p;
						apex_poly=p;
						portal_left=apex_point;
						portal_right=apex_point;
						path.push_back(apex_point);
						skip=true;
					}
				}

				if (!skip && CLOCK_TANGENT(apex_point,portal_right,right).dot(up) <= 0){
					//process
					if (portal_right==apex_point || CLOCK_TANGENT(apex_point,right,portal_left).dot(up) < 0) {
						right_poly=p;
						portal_right=right;
					} else {

						_clip_path(path,apex_poly,portal_left,left_poly);

						apex_point=portal_left;
						p=left_poly;
						right_poly=p;
						apex_poly=p;
						portal_right=apex_point;
						portal_left=apex_point;
						path.push_back(apex_point);
					}
				}

				if (p!=begin_poly)
					p=p->edges[p->prev_edge].C;
				else
					p=NULL;

			}

			if (path[path.size()-1]!=begin_point)
				path.push_back(begin_point);

			path.invert();




		} else {
			//midpoints
			Polygon *p=end_poly;

			path.push_back(end_point);
			while(true) {
				int prev = p->prev_edge;
				int prev_n = (p->prev_edge+1)%p->edges.size();
				Vector3 point = (_get_vertex(p->edges[prev].point) + _get_vertex(p->edges[prev_n].point))*0.5;
				path.push_back(point);
				p = p->edges[prev].C;
				if (p==begin_poly)
					break;
			}

			path.push_back(begin_point);


			path.invert();;
		}

		return path;
	}


	return Vector<Vector3>();

}

Vector3 Navigation::get_closest_point_to_segment(const Vector3& p_from,const Vector3& p_to) {


	bool use_collision=false;
	Vector3 closest_point;
	float closest_point_d=1e20;
	NavMesh *closest_navmesh=NULL;

	for (Map<int,NavMesh>::Element*E=navmesh_map.front();E;E=E->next()) {

		if (!E->get().linked)
			continue;
		for(List<Polygon>::Element *F=E->get().polygons.front();F;F=F->next()) {

			Polygon &p=F->get();
			for(int i=2;i<p.edges.size();i++) {

				Face3 f(_get_vertex(p.edges[0].point),_get_vertex(p.edges[i-1].point),_get_vertex(p.edges[i].point));
				Vector3 inters;
				if (f.intersects_segment(p_from,p_to,&inters)) {

					if (!use_collision) {
						closest_point=inters;
						use_collision=true;
						closest_point_d=p_from.distance_to(inters);
						closest_navmesh=p.owner;
					} else if (closest_point_d > inters.distance_to(p_from)){

						closest_point=inters;
						closest_point_d=p_from.distance_to(inters);
						closest_navmesh=p.owner;
					}
				}
			}

			if (!use_collision) {

				for(int i=0;i<p.edges.size();i++) {

					Vector3 a,b;

					Geometry::get_closest_points_between_segments(p_from,p_to,_get_vertex(p.edges[i].point),_get_vertex(p.edges[(i+1)%p.edges.size()].point),a,b);

					float d = a.distance_to(b);
					if (d<closest_point_d) {

						closest_point_d=d;
						closest_point=b;
						closest_navmesh=p.owner;
					}

				}
			}
		}
	}

	if (closest_navmesh && closest_navmesh->owner) {
		//print_line("navmesh is: "+closest_navmesh->owner->cast_to<Node>()->get_name());
	}

	return closest_point;
}

Vector3 Navigation::get_closest_point(const Vector3& p_point) {

	Vector3 closest_point;
	float closest_point_d=1e20;

	for (Map<int,NavMesh>::Element*E=navmesh_map.front();E;E=E->next()) {

		if (!E->get().linked)
			continue;
		for(List<Polygon>::Element *F=E->get().polygons.front();F;F=F->next()) {

			Polygon &p=F->get();
			for(int i=2;i<p.edges.size();i++) {

				Face3 f(_get_vertex(p.edges[0].point),_get_vertex(p.edges[i-1].point),_get_vertex(p.edges[i].point));
				Vector3 inters = f.get_closest_point_to(p_point);
				float d = inters.distance_to(p_point);
				if (d<closest_point_d) {
					closest_point=inters;
					closest_point_d=d;
				}
			}
		}
	}

	return closest_point;

}

Vector3 Navigation::get_closest_point_normal(const Vector3& p_point){

	Vector3 closest_point;
	Vector3 closest_normal;
	float closest_point_d=1e20;

	for (Map<int,NavMesh>::Element*E=navmesh_map.front();E;E=E->next()) {

		if (!E->get().linked)
			continue;
		for(List<Polygon>::Element *F=E->get().polygons.front();F;F=F->next()) {

			Polygon &p=F->get();
			for(int i=2;i<p.edges.size();i++) {

				Face3 f(_get_vertex(p.edges[0].point),_get_vertex(p.edges[i-1].point),_get_vertex(p.edges[i].point));
				Vector3 inters = f.get_closest_point_to(p_point);
				float d = inters.distance_to(p_point);
				if (d<closest_point_d) {
					closest_point=inters;
					closest_point_d=d;
					closest_normal=f.get_plane().normal;
				}
			}
		}
	}

	return closest_normal;

}


void Navigation::set_up_vector(const Vector3& p_up) {


	up=p_up;
}

Vector3 Navigation::get_up_vector() const{

	return up;
}


void Navigation::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("navmesh_create","mesh:NavigationMesh","xform","owner"),&Navigation::navmesh_create,DEFVAL(Variant()));
	ObjectTypeDB::bind_method(_MD("navmesh_set_transform","id","xform"),&Navigation::navmesh_set_transform);
	ObjectTypeDB::bind_method(_MD("navmesh_remove","id"),&Navigation::navmesh_remove);

	ObjectTypeDB::bind_method(_MD("get_simple_path","start","end","optimize"),&Navigation::get_simple_path,DEFVAL(true));
	ObjectTypeDB::bind_method(_MD("get_closest_point_to_segment","start","end"),&Navigation::get_closest_point_to_segment);
	ObjectTypeDB::bind_method(_MD("get_closest_point","to_point"),&Navigation::get_closest_point);
	ObjectTypeDB::bind_method(_MD("get_closest_point_normal","to_point"),&Navigation::get_closest_point_normal);

	ObjectTypeDB::bind_method(_MD("set_up_vector","up"),&Navigation::set_up_vector);
	ObjectTypeDB::bind_method(_MD("get_up_vector"),&Navigation::get_up_vector);

	ADD_PROPERTY( PropertyInfo(Variant::VECTOR3,"up_vector"),_SCS("set_up_vector"),_SCS("get_up_vector"));
}

Navigation::Navigation() {

	ERR_FAIL_COND( sizeof(Point)!=8 );
	cell_size=0.01; //one centimeter
	last_id=1;
	up=Vector3(0,1,0);
}


