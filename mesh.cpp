#include "mesh.h"

void get_vertices_and_normals_from_triangles(vector<triangle> &t, vector<vertex_3> &fn, vector<vertex_3> &v, vector<vertex_3> &vn)
{
	fn.clear();
	v.clear();
	vn.clear();
	
	if(0 == t.size())
		return;

	cout << "Triangles: " << t.size() << endl;
	
    cout << "Welding vertices" << endl;
 
    // Insert unique vertices into set.
    set<vertex_3> vertex_set;
 
    for(vector<triangle>::const_iterator i = t.begin(); i != t.end(); i++)
    {
        vertex_set.insert(i->vertex[0]);
        vertex_set.insert(i->vertex[1]);
        vertex_set.insert(i->vertex[2]);
    }
 
    cout << "Vertices: " << vertex_set.size() << endl;

    cout << "Generating vertex indices" << endl;
 
    // Add indices to the vertices.
    for(set<vertex_3>::const_iterator i = vertex_set.begin(); i != vertex_set.end(); i++)
    {
        size_t index = v.size();
        v.push_back(*i);
        v[index].index = index;
    }
 
    vertex_set.clear();

	// Re-insert modifies vertices into set.
    for(vector<vertex_3>::const_iterator i = v.begin(); i != v.end(); i++)
        vertex_set.insert(*i);
 
    cout << "Assigning vertex indices to triangles" << endl;
   
    // Find the three vertices for each triangle, by index.
    set<vertex_3>::iterator find_iter;
 
    for(vector<triangle>::iterator i = t.begin(); i != t.end(); i++)
    {
        find_iter = vertex_set.find(i->vertex[0]);
        i->vertex[0].index = find_iter->index;
 
        find_iter = vertex_set.find(i->vertex[1]);
        i->vertex[1].index = find_iter->index;
 
        find_iter = vertex_set.find(i->vertex[2]);
        i->vertex[2].index = find_iter->index;
    }

	vertex_set.clear();

	cout << "Calculating normals" << endl;
	fn.resize(t.size());
	vn.resize(v.size());

	for(size_t i = 0; i < t.size(); i++)
	{
		vertex_3 v0 = t[i].vertex[1] - t[i].vertex[0];
		vertex_3 v1 = t[i].vertex[2] - t[i].vertex[0];
		fn[i] = v0.cross(v1);
		fn[i].normalize();

		vn[t[i].vertex[0].index] = vn[t[i].vertex[0].index] + fn[i];
		vn[t[i].vertex[1].index] = vn[t[i].vertex[1].index] + fn[i];
		vn[t[i].vertex[2].index] = vn[t[i].vertex[2].index] + fn[i];
	}

	for(size_t i = 0; i < vn.size(); i++)
		vn[i].normalize();
}
