#include "main.h"
#include "mesh.h"

#include <opencv2/opencv.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world340.lib")

int main(int argc, char **argv)
{	
	Mat frame = imread("picture.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	transpose(frame, frame);
	flip(frame, frame, 1);

	for (int j = 0; j < frame.rows - 1; j++)
	{
		for (int i = 0; i < frame.cols - 1; i++)
		{
			unsigned char intensity0 = frame.at<unsigned char>(j, i);
			unsigned char intensity1 = frame.at<unsigned char>(j + 1, i);
			unsigned char intensity2 = frame.at<unsigned char>(j + 1, i + 1);
			unsigned char intensity3 = frame.at<unsigned char>(j, i + 1);

			unsigned char intensity4 = ((float)intensity0 + (float)intensity1 + (float) intensity2 + (float)intensity3) / 4.0;

			vertex_3 v0(j, i, intensity0 / 255.0 / 50);
			vertex_3 v1(j + 1, i, intensity1 / 255.0 / 50);
			vertex_3 v2(j + 1, i + 1, intensity2 / 255.0 / 50);
			vertex_3 v3(j, i + 1, intensity3 / 255.0 / 50);
			vertex_3 v4((j + j + 1) / 2.0 , (i + i + 1) / 2.0, intensity4 / 255.0 / 50);

			v0.x /= 1000; v0.y /= 1000;
			v1.x /= 1000; v1.y /= 1000;
			v2.x /= 1000; v2.y /= 1000;
			v3.x /= 1000; v3.y /= 1000;
			v4.x /= 1000; v4.y /= 1000;

			triangle t0;
			t0.vertex[0] = v0;
			t0.vertex[1] = v4;
			t0.vertex[2] = v3;
			t0.colour[0].x = intensity0 / 255.0;
			t0.colour[0].y = intensity0 / 255.0;
			t0.colour[0].z = intensity0 / 255.0;
			t0.colour[1].x = intensity4 / 255.0;
			t0.colour[1].y = intensity4 / 255.0;
			t0.colour[1].z = intensity4 / 255.0;
			t0.colour[2].x = intensity3 / 255.0;
			t0.colour[2].y = intensity3 / 255.0;
			t0.colour[2].z = intensity3 / 255.0;

			triangle t1;
			t1.vertex[0] = v0;
			t1.vertex[1] = v1;
			t1.vertex[2] = v4;
			t1.colour[0].x = intensity0 / 255.0;
			t1.colour[0].y = intensity0 / 255.0;
			t1.colour[0].z = intensity0 / 255.0;
			t1.colour[1].x = intensity1 / 255.0;
			t1.colour[1].y = intensity1 / 255.0;
			t1.colour[1].z = intensity1 / 255.0;
			t1.colour[2].x = intensity4 / 255.0;
			t1.colour[2].y = intensity4 / 255.0;
			t1.colour[2].z = intensity4 / 255.0;

			triangle t2;
			t2.vertex[0] = v1;
			t2.vertex[1] = v2;
			t2.vertex[2] = v4;
			t2.colour[0].x = intensity1 / 255.0;
			t2.colour[0].y = intensity1 / 255.0;
			t2.colour[0].z = intensity1 / 255.0;
			t2.colour[1].x = intensity2 / 255.0;
			t2.colour[1].y = intensity2 / 255.0;
			t2.colour[1].z = intensity2 / 255.0;
			t2.colour[2].x = intensity4 / 255.0;
			t2.colour[2].y = intensity4 / 255.0;
			t2.colour[2].z = intensity4 / 255.0;

			triangle t3;
			t3.vertex[0] = v3;
			t3.vertex[1] = v4;
			t3.vertex[2] = v2;
			t3.colour[0].x = intensity3 / 255.0;
			t3.colour[0].y = intensity3 / 255.0;
			t3.colour[0].z = intensity3 / 255.0;
			t3.colour[1].x = intensity4 / 255.0;
			t3.colour[1].y = intensity4 / 255.0;
			t3.colour[1].z = intensity4 / 255.0;
			t3.colour[2].x = intensity2 / 255.0;
			t3.colour[2].y = intensity2 / 255.0;
			t3.colour[2].z = intensity2 / 255.0;

			triangles.push_back(t0);
			triangles.push_back(t1);
			triangles.push_back(t2);
			triangles.push_back(t3);
		}
	}


	get_vertices_and_normals_from_triangles(triangles, face_normals, vertices, vertex_normals);	
	
	glutInit(&argc, argv);
	init_opengl(win_x, win_y);
	glutReshapeFunc(reshape_func);
	glutIdleFunc(idle_func);
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutPassiveMotionFunc(passive_motion_func);
	//glutIgnoreKeyRepeat(1);
	glutMainLoop();
	glutDestroyWindow(win_id);

	return 0;
}



void idle_func(void)
{
	glutPostRedisplay();
}

void init_opengl(const int &width, const int &height)
{
	win_x = width;
	win_y = height;

	if(win_x < 1)
		win_x = 1;

	if(win_y < 1)
		win_y = 1;

	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(win_x, win_y);
	win_id = glutCreateWindow("Binary Stereo Lithography file viewer");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(background_colour.x, background_colour.y, background_colour.z, 1);
	glClearDepth(1.0f);

	main_camera.Set(0, 0, camera_w, camera_fov, win_x, win_y, camera_near, camera_far);}

void reshape_func(int width, int height)
{
	win_x = width;
	win_y = height;

	if(win_x < 1)
		win_x = 1;

	if(win_y < 1)
		win_y = 1;

	glutSetWindow(win_id);
	glutReshapeWindow(win_x, win_y);
	glViewport(0, 0, win_x, win_y);

	main_camera.Set(main_camera.u, main_camera.v, main_camera.w, main_camera.fov, win_x, win_y, camera_near, camera_far);
}

// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
void render_string(int x, const int y, void *font, const string &text)
{
	for(size_t i = 0; i < text.length(); i++)
	{
		glRasterPos2i(x, y);
		glutBitmapCharacter(font, text[i]);
		x += glutBitmapWidth(font, text[i]) + 1;
	}
}
// End text drawing code.

void draw_objects(void)
{
	glEnable(GL_LIGHTING);

	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] =	{ 0.75f, 0.75f, 0.75f, 1.0f };
	GLfloat light0_pos[] = {0.0f, 0.0f, 10.0f, 1.0f };
//	GLfloat light1_pos[] = { -2.0f, -2.0f, -2.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glEnable(GL_LIGHT0);

	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	//glEnable(GL_LIGHT1);

	glPushMatrix();

	glTranslatef(camera_x_transform, camera_y_transform, 0);

	// Could probably stand to use a VBO here.
	if(true == draw_mesh)
	{
		glBegin(GL_TRIANGLES);

		for(size_t i = 0; i < triangles.size(); i++)
		{
			if(true == smooth_shading)
			{
				size_t v_index0 = triangles[i].vertex[0].index;
				size_t v_index1 = triangles[i].vertex[1].index;
				size_t v_index2 = triangles[i].vertex[2].index;

				glNormal3f(vertex_normals[v_index0].x, vertex_normals[v_index0].y, vertex_normals[v_index0].z);
				glVertex3f(vertices[v_index0].x , vertices[v_index0].y, vertices[v_index0].z);
				glNormal3f(vertex_normals[v_index1].x, vertex_normals[v_index1].y, vertex_normals[v_index1].z);
				glVertex3f(vertices[v_index1].x , vertices[v_index1].y, vertices[v_index1].z);
				glNormal3f(vertex_normals[v_index2].x, vertex_normals[v_index2].y, vertex_normals[v_index2].z);
				glVertex3f(vertices[v_index2].x , vertices[v_index2].y, vertices[v_index2].z);
			}
			else
			{
				glNormal3f(face_normals[i].x, face_normals[i].y, face_normals[i].z);

				//GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
				//GLfloat position[] = { 0.0, 0.0, 2.0, 1.0 };
				//GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
				//GLfloat mat_specular[] = { 1.0, 0.5, 0.0, 1.0 };
				//GLfloat mat_shininess[] = { 10.0 };

				//glEnable(GL_LIGHTING);
				//glEnable(GL_LIGHT0);

				//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
				//glLightfv(GL_LIGHT0, GL_POSITION, position);
				//	
				//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
				//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

				float v0_colour[] = { triangles[i].colour[0].x, triangles[i].colour[0].y, triangles[i].colour[0].z, 1.0f };
				glMaterialfv(GL_FRONT, GL_DIFFUSE, v0_colour);

				glVertex3f(triangles[i].vertex[0].x , triangles[i].vertex[0].y, triangles[i].vertex[0].z);

				float v1_colour[] = { triangles[i].colour[1].x, triangles[i].colour[1].y, triangles[i].colour[1].z, 1.0f };
				glMaterialfv(GL_FRONT, GL_DIFFUSE, v1_colour);

				glVertex3f(triangles[i].vertex[1].x , triangles[i].vertex[1].y, triangles[i].vertex[1].z);

				float v2_colour[] = { triangles[i].colour[2].x, triangles[i].colour[2].y, triangles[i].colour[2].z, 1.0f };
				glMaterialfv(GL_FRONT, GL_DIFFUSE, v2_colour);
				
				glVertex3f(triangles[i].vertex[2].x , triangles[i].vertex[2].y, triangles[i].vertex[2].z);
			}
		}

		glEnd();
	}

	glDisable(GL_LIGHTING);

	// If we do draw the axis at all, make sure not to draw its outline.
	if(true == draw_axis)
	{
		glLineWidth(1.0f);

		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);

		glColor3f(0.5, 0.5, 0.5);
		glVertex3f(0, 0, 0);
		glVertex3f(-1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -1);

		glEnd();
	}

	glPopMatrix();
}




void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Draw the model's components using OpenGL/GLUT primitives.
	draw_objects();

	if(true == draw_control_list)
	{
		// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
		// http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, win_x, 0, win_y);
		glScalef(1, -1, 1); // Neat. :)
		glTranslatef(0, -win_y, 0); // Neat. :)
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor3f(control_list_colour.x, control_list_colour.y, control_list_colour.z);

		size_t break_size = 22;
		size_t start = 20;
		ostringstream oss;

		render_string(10, start, GLUT_BITMAP_HELVETICA_18, string("Mouse controls:"));
		render_string(10, start + 1*break_size, GLUT_BITMAP_HELVETICA_18, string("  LMB + drag: Rotate camera"));
		render_string(10, start + 2*break_size, GLUT_BITMAP_HELVETICA_18, string("  RMB + drag: Zoom camera"));

		render_string(10, start + 4*break_size, GLUT_BITMAP_HELVETICA_18, string("Keyboard controls:"));
		render_string(10, start + 5*break_size, GLUT_BITMAP_HELVETICA_18, string("  q: Draw mesh"));
		render_string(10, start + 6*break_size, GLUT_BITMAP_HELVETICA_18, string("  w: Draw axis"));
		render_string(10, start + 7*break_size, GLUT_BITMAP_HELVETICA_18, string("  e: Draw text"));

		oss.clear();
		oss.str("");		
		oss << "  s: Smooth shading -- ";
		
		if(true == smooth_shading)
			oss << "ON";
		else
			oss << "OFF";

		render_string(10, start + 8*break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		render_string(10, start + 9*break_size, GLUT_BITMAP_HELVETICA_18, string("  u: Rotate camera +u"));
		render_string(10, start + 10*break_size, GLUT_BITMAP_HELVETICA_18, string("  i: Rotate camera -u"));
		render_string(10, start + 11*break_size, GLUT_BITMAP_HELVETICA_18, string("  o: Rotate camera +v"));
		render_string(10, start + 12*break_size, GLUT_BITMAP_HELVETICA_18, string("  p: Rotate camera -v"));


		
		vertex_3 eye = main_camera.eye;
		vertex_3 eye_norm = eye;
		eye_norm.normalize();

		oss.clear();
		oss.str("");		
		oss << "Camera position: " << eye.x << ' ' << eye.y << ' ' << eye.z;
		render_string(10, win_y - 2*break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		oss.clear();
		oss.str("");
		oss << "Camera position (normalized): " << eye_norm.x << ' ' << eye_norm.y << ' ' << eye_norm.z;
		render_string(10, win_y - break_size, GLUT_BITMAP_HELVETICA_18, oss.str());

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// End text drawing code.
	}

	glFlush();
	glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch(tolower(key))
	{
	case 'a':
		{
			output_to_mesh2();
			break;
		}
	case 'q':
		{
			draw_mesh = !draw_mesh;
			break;
		}
	case 'w':
		{
			draw_axis = !draw_axis;
			break;
		}
	case 'e':
		{
			draw_control_list = !draw_control_list;
			break;
		}
	case 's':
		{
			smooth_shading = !smooth_shading;
			break;
		}
	case 'u':
		{
			main_camera.u -= u_spacer;
			main_camera.Set();
			break;
		}
	case 'i':
		{
			main_camera.u += u_spacer;
			main_camera.Set();
			break;
		}
	case 'o':
		{
			main_camera.v -= v_spacer;
			main_camera.Set();
			break;
		}
	case 'p':
		{
			main_camera.v += v_spacer;
			main_camera.Set();
			break;
		}

	default:
		break;
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if(GLUT_LEFT_BUTTON == button)
	{
		if(GLUT_DOWN == state)
			lmb_down = true;
		else
			lmb_down = false;
	}
	else if(GLUT_MIDDLE_BUTTON == button)
	{
		if(GLUT_DOWN == state)
			mmb_down = true;
		else
			mmb_down = false;
	}
	else if(GLUT_RIGHT_BUTTON == button)
	{
		if(GLUT_DOWN == state)
			rmb_down = true;
		else
			rmb_down = false;
	}
}

void motion_func(int x, int y)
{
	int prev_mouse_x = mouse_x;
	int prev_mouse_y = mouse_y;

	mouse_x = x;
	mouse_y = y;

	int mouse_delta_x = mouse_x - prev_mouse_x;
	int mouse_delta_y = prev_mouse_y - mouse_y;

	if(true == lmb_down && (0 != mouse_delta_x || 0 != mouse_delta_y))
	{
		main_camera.u -= static_cast<float>(mouse_delta_y)*u_spacer;
		main_camera.v += static_cast<float>(mouse_delta_x)*v_spacer;
	}
	else if(true == rmb_down && (0 != mouse_delta_y))
	{
		main_camera.w -= static_cast<float>(mouse_delta_y)*w_spacer;

		if(main_camera.w < 1.1f)
			main_camera.w = 1.1f;
		else if(main_camera.w > 20.0f)
			main_camera.w = 20.0f;
	}

	main_camera.Set(); // Calculate new camera vectors.
}

void passive_motion_func(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}

void output_to_mesh2(void)
{
	ofstream out("out.mesh2.txt");

	out << "mesh2 {" << endl;
	out << "vertex_vectors {" << endl;	
	out << vertices.size() << "," << endl;

	for(size_t i = 0; i < vertices.size() - 1; i++)
		out << "<" << vertices[i].x << "," << vertices[i].y << "," << vertices[i].z << ">," << endl;

	out << "<" << vertices[vertices.size() - 1].x << "," << vertices[vertices.size() - 1].y << "," << vertices[vertices.size() - 1].z << ">" << endl;
	out << "}" << endl;

	out << "face_indices {" << endl;
	out << triangles.size() << "," << endl;
}


