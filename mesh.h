#pragma once

#include "primitives.h"


#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <ios>
using std::ios_base;
using std::ios;

#include <set>
using std::set;

#include <vector>
using std::vector;

#include <limits>
using std::numeric_limits;

#include <cstring> // for memcpy()



void get_vertices_and_normals_from_triangles(vector<triangle> &t, vector<vertex_3> &fn, vector<vertex_3> &v, vector<vertex_3> &vn);



