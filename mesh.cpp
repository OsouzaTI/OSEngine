#include "mesh.h"
#include "osmath.h"
#include <iostream>

Mesh::Mesh()
{
	mesh.scale.x = 1;
	mesh.scale.y = 1;
	mesh.scale.z = 1;

	mesh.rotation.x = 0;
	mesh.rotation.y = 0;
	mesh.rotation.z = 0;

	shearXY_matrix = Mat4x4_MakeIdentity();
	shearYX_matrix = Mat4x4_MakeIdentity();

	mesh.translation.x = 0.0f;
	mesh.translation.y = 0.0f;
	mesh.translation.z = 5.0f;
}

Mesh::~Mesh()
{
}

void Mesh::load_cube_object() {

	cube_vertices[0] = { -1, -1, -1 };
	cube_vertices[1] = { -1,  1, -1 };
	cube_vertices[2] = { 1,  1, -1 };
	cube_vertices[3] = { 1, -1, -1 };
	cube_vertices[4] = { 1,  1,  1 };
	cube_vertices[5] = { 1, -1,  1 };
	cube_vertices[6] = { -1,  1,  1 };
	cube_vertices[7] = { -1, -1,  1 };

	cube_faces[0] = { 1, 2, 3, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFFFF };
	cube_faces[1] = { 1, 3, 4, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFFFF };

	cube_faces[2] = { 4, 3, 5, {0, 0}, {0, 1},	{1, 1},0xFFFFFFFF };
	cube_faces[3] = { 4, 5, 6, {0, 0}, {1, 1},	{1, 0},0xFFFFFFFF };

	cube_faces[4] = { 6, 5, 7, {0, 0}, {0, 1},	{1, 1},0xFFFFFFFF };
	cube_faces[5] = { 6, 7, 8, {0, 0}, {1, 1},	{1, 0},0xFFFFFFFF };

	cube_faces[6] = { 8, 7, 2, {0, 0}, {0, 1},	{1, 1},0xFFFFFFFF };
	cube_faces[7] = { 8, 2, 1, {0, 0}, {1, 1},	{1, 0},0xFFFFFFFF };

	cube_faces[8] = { 2, 7, 5, {0, 0}, {0, 1},	{1, 1},0xFFFFFFFF };
	cube_faces[9] = { 2, 5, 3, {0, 0}, {1, 1},	{1, 0},0xFFFFFFFF };

	cube_faces[10] = { 6, 8, 1, {0, 0}, {0, 1},	{1, 1}, 0xFFFFFFFF };
	cube_faces[11] = { 6, 1, 4, {0, 0}, {1, 1},	{1, 0}, 0xFFFFFFFF };

	for (int i = 0; i < 8; i++) {
		vect3<float> cube_vertex = cube_vertices[i];
		mesh.vertices.push_back(cube_vertex);
	}
	for (int i = 0; i < 12; i++) {
		face cube_face = cube_faces[i];
		mesh.faces.push_back(cube_face);
	}

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
					0xFFFFFFFF
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

	Mat4x4 scale = Mat4x4_MakeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	Mat4x4 rotateX = Mat4x4_MakeRotationX(mesh.rotation.x);
	Mat4x4 rotateY = Mat4x4_MakeRotationY(mesh.rotation.y);
	Mat4x4 rotateZ = Mat4x4_MakeRotationZ(mesh.rotation.z);
	Mat4x4 translation = Mat4x4_MakeTranslation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	Mat4x4 project_matrix = Mat4x4_MakePerspective(
		RAD(Display::camera->fov),
		Display::camera->aspect,
		Display::camera->znear,
		Display::camera->zfar
	);

	//number of faces of array mesh.faces
	int n_faces = mesh.faces.size();
	for (int i = 0; i < n_faces; i++)
	{
		// get the face index of array faces
		face  mesh_face = mesh.faces[i];
		// set the same color ImGUI
		mesh_face.color = color;
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
			world_matrix = Mat4x4_MultiplyMatrix(shearXY_matrix, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(shearYX_matrix, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(translation, world_matrix);
			transformed_vertex = Mat4x4_MultiplyVector(world_matrix, transformed_vertex);
			transformed_vertices[j] = transformed_vertex;
		}

		// Famoso backface
		vect3<float> vector_a = vec3_from_vec4(transformed_vertices[0]);
		vect3<float> vector_b = vec3_from_vec4(transformed_vertices[1]);
		vect3<float> vector_c = vec3_from_vec4(transformed_vertices[2]);

		vect3<float> vector_ab = vsubvect(vector_b, vector_a);
		vect3<float> vector_ac = vsubvect(vector_c, vector_a);
		vnormalize(&vector_ab);
		vnormalize(&vector_ac);

		vect3<float> normal = vcrossvect(vector_ab, vector_ac);
		vnormalize(&normal);

		vect3<float> camera_ray = vsubvect(Display::camera->point, vector_a);

		float dot_normal_camera = vdotvect(normal, camera_ray);

		if (display->drawing_type == BACKFACE_TYPE::CULLING) {
			if (dot_normal_camera < 0) {
				continue;
			}
		}		

		vect4<float> projected_points[3];
		for (int j = 0; j < 3; j++)
		{
			projected_points[j] = Mat4x4_MultiplyProjectVector(project_matrix, transformed_vertices[j]);
			projected_points[j].x *= (display->width / 2.0);
			projected_points[j].y *= (display->height / 2.0);

			// invert the y values of mesh
			projected_points[j].y *= -1;

			// Translate the projected points to the middle of the screen
			projected_points[j].x += (display->width / 2.0);
			projected_points[j].y += (display->height / 2.0);
		}

		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0f;
		
		if (display->shading == SHADING_TYPE::FLAT) {
			float light_percentage = -vdotvect(normal, vect3<float>{ 0, 0, 1 });
			mesh_face.color = light_apply_intensity(mesh_face.color, light_percentage);
		}
		
		triangle projected_triangle = {
			{
				{ projected_points[0].x, projected_points[0].y },
				{ projected_points[1].x, projected_points[1].y },
				{ projected_points[2].x, projected_points[2].y },
			},
			{
				{mesh_face.a_uv.u,mesh_face.a_uv.v},
				{mesh_face.b_uv.u,mesh_face.b_uv.v},
				{mesh_face.c_uv.u,mesh_face.c_uv.v}
			},
			mesh_face.color,
			avg_depth
		};

		triangles_to_render.push_back(projected_triangle);
	}

	//Sort the triangles to render by their avg_depth
	int num_triangles = triangles_to_render.size();
	for (int i = 0; i < num_triangles; i++) {
		for (int j = i; j < num_triangles; j++) {
			if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
				// Swap the triangles positions in the array
				triangle temp = triangles_to_render[i];
				triangles_to_render[i] = triangles_to_render[j];
				triangles_to_render[j] = temp;
			}
		}
	}

}

void Mesh::draw_mesh()
{
	// the number of triangles in the array triangles_to_render
	int n_triangles = triangles_to_render.size();
	for (int i = 0; i < n_triangles; i++)
	{
		triangle triangle = triangles_to_render[i];
		if (display->draw_mode == DRAW_MODE::WIREFRAME) {
			draw->draw_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				color
			);
		}
		
		if (display->draw_mode == DRAW_MODE::FILLED) {
			draw->fill_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				triangle.color
			);
		}

		if (display->draw_mode == DRAW_MODE::TEXTURED) {
			draw->draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.textcoords[0].u, triangle.textcoords[0].v,
				triangle.points[1].x, triangle.points[1].y,
				triangle.textcoords[1].u, triangle.textcoords[1].v,
				triangle.points[2].x, triangle.points[2].y,
				triangle.textcoords[2].u, triangle.textcoords[2].v,
				mesh_texture
			);
		}

		if (display->draw_mode == DRAW_MODE::TEXTURED_WIREFRAME) {
			// texture

			draw->draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.textcoords[0].u, triangle.textcoords[0].v,
				triangle.points[1].x, triangle.points[1].y,
				triangle.textcoords[1].u, triangle.textcoords[1].v,
				triangle.points[2].x, triangle.points[2].y,
				triangle.textcoords[2].u, triangle.textcoords[2].v,
				mesh_texture
			);

			// wireframe
			draw->draw_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				color
			);
		}

	}

	triangles_to_render.clear();
}

void Mesh::set_display(Display* display)
{
	this->display = display;
}

void Mesh::set_color(uint32_t color)
{
	this->color = color;
}

void Mesh::set_drawing(Drawing* drawing)
{
	draw = drawing;
}

uint32_t Mesh::get_color()
{
	return color;
}

void Mesh::set_translation(vect3<float> translation)
{
	mesh.translation.x = translation.x;
	mesh.translation.y = translation.y;
	mesh.translation.z = translation.z;
}

void Mesh::set_scale(vect3<float> scale)
{
	mesh.scale.x = scale.x;
	mesh.scale.y = scale.y;
	mesh.scale.z = scale.z;
}

void Mesh::set_rotate(vect3<float> rotation)
{
	mesh.rotation.x = rotation.x;
	mesh.rotation.y = rotation.y;
	mesh.rotation.z = rotation.z;
}

void Mesh::set_shearXY(float angle)
{
	shearXY_matrix = Mat4x4_MakeDistorcionXY(angle);
}

void Mesh::set_shearYX(float angle)
{
	shearYX_matrix = Mat4x4_MakeDistorcionYX(angle);
}

vect3<float>* Mesh::get_translation()
{
	return &mesh.translation;
}

vect3<float>* Mesh::get_scale()
{
	return &mesh.scale;
}

vect3<float>* Mesh::get_rotate()
{
	return &mesh.rotation;
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
