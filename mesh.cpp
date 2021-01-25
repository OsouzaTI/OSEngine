#include "mesh.h"
#include "matrix.h"
#include "osmath.h"
#include <iostream>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::load_obj_file_data(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");

	}
	else {
		while (1) {
			//buffer
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0) {

				vect3<float> v;
				fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
				mesh.vertices.push_back(v);

			}

			if (strcmp(lineHeader, "f") == 0) {

				int vi[3];
				int ti[3];
				int ni[3];

				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d",
					&vi[0], &ti[0], &ni[0],
					&vi[1], &ti[1], &ni[1],
					&vi[2], &ti[2], &ni[2]
				);

				face f = {
					vi[0],
					vi[1],
					vi[2],
					0xFFFF0000
				};

				mesh.faces.push_back(f);

			}

		}
	}
	this->loaded = true;
	std::cout << "Mesh Loaded" << std::endl;
}
int angle = 0.0f;
void Mesh::update_mesh()
{
	if (angle > 360) angle = 0;
	angle += 1;

	mesh.scale.x = 5;
	mesh.scale.y = 5;
	mesh.scale.z = 5;
	
	mesh.translation.x = 15.0f;
	mesh.translation.y = 15.0f;
	mesh.translation.z = 30.0f;

	Mat4x4 scale = Mat4x4_MakeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	Mat4x4 rotateY = Mat4x4_MakeRotationY(mesh.rotation.y);
	Mat4x4 rotateX = Mat4x4_MakeRotationX(mesh.rotation.x);
	Mat4x4 rotateZ = Mat4x4_MakeRotationZ(mesh.rotation.z);
	Mat4x4 translation = Mat4x4_MakeTranslation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	Mat4x4 project_matrix = Mat4x4_MakePerspective(
		RAD(this->display->camera.fov),
		this->display->camera.aspect,
		this->display->camera.znear,
		this->display->camera.zfar
	);

	//number of faces of array mesh.faces
	int n_faces = mesh.faces.size();
	for (int i = 0; i < n_faces; i++)
	{
		// get the face index of array faces
		face  mesh_face = mesh.faces[i];
		// create a vec3 for the 3 vectors of face
		vect3<float> face_vertex[3];

		//Populando os vertices da face

		face_vertex[0] = mesh.vertices[mesh_face.a - 1];
		face_vertex[1] = mesh.vertices[mesh_face.b - 1];
		face_vertex[2] = mesh.vertices[mesh_face.c - 1];
		vect4<float> transformed_vertices[3];
		for (int j = 0; j < 3; j++)
		{
			vect4<float> transformed_vertex = vec4_from_vec3(face_vertex[j]);
			Mat4x4 world_matrix = Mat4x4_MakeIdentity();
			world_matrix = Mat4x4_MultiplyMatrix(scale, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateY, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateX, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateZ, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(translation, world_matrix);
			transformed_vertex = Mat4x4_MultiplyVector(world_matrix, transformed_vertex);
			transformed_vertices[j] = transformed_vertex;
		}

		vect4<float> projected_points[3];
		for (int j = 0; j < 3; j++)
		{
			projected_points[j] = Mat4x4_MultiplyProjectVector(project_matrix, transformed_vertices[j]);
			projected_points[j].x *= (display->width / 2.0);
			projected_points[j].y *= (display->height / 2.0);
		}

		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0f;

		triangle projected_triangle = {
			{
				{ projected_points[0].x, projected_points[0].y },
				{ projected_points[1].x, projected_points[1].y },
				{ projected_points[2].x, projected_points[2].y },
			},
			mesh_face.color,
			avg_depth
		};

		triangles_to_render.push_back(projected_triangle);
	}
}

void Mesh::draw_mesh(Drawing &draw)
{
	// the number of triangles in the array triangles_to_render
	int n_triangles = triangles_to_render.size();
	for (int i = 0; i < n_triangles; i++)
	{
		triangle triangle = triangles_to_render[i];
		draw.draw_triangle(
			triangle.points[0].x,
			triangle.points[0].y,
			triangle.points[1].x,
			triangle.points[1].y,
			triangle.points[2].x,
			triangle.points[2].y,
			0xFFFFFF00
		);
	}

	triangles_to_render.clear();
}

void Mesh::set_display(Display* display)
{
	this->display = display;
}

void Mesh::set_rotate_mesh_x(float angle_x)
{
	this->mesh.rotation.x = angle_x;
}

void Mesh::set_rotate_mesh_y(float angle_y)
{
	this->mesh.rotation.y = angle_y;
}

void Mesh::set_rotate_mesh_z(float angle_z)
{
	this->mesh.rotation.z = angle_z;
}
