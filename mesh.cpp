#include "mesh.h"
#include "math.h"
#include <iostream>
#include "polygon.h"
#include "triangle.h"

Mesh::Mesh()
{

	// alocate the static memory of triangles
	triangles_to_render = (triangle*)malloc(sizeof(triangle) * MAX_TRIANGLES_PER_MESH);
	if (!triangles_to_render) {
		std::cout << "Not possible allocate " << MAX_TRIANGLES_PER_MESH << " triangles in memory\n";
	}
	else {
		LOG("Sucessful allocated memory for mesh");
	}

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

	LOG("Mesh started");

}

Mesh::~Mesh()
{	
	if(texture.png_texture != NULL)
		upng_free(texture.png_texture);
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

	cube_faces[0] = { 1, 2, 3, {0, 1}, {0, 0}, {1, 0}, 0xFFFFFFFF };
	cube_faces[1] = { 1, 3, 4, {0, 1}, {1, 0}, {1, 1}, 0xFFFFFFFF };

	cube_faces[2] = { 4, 3, 5, {0, 1}, {0, 0},	{1, 0},0xFFFFFFFF };
	cube_faces[3] = { 4, 5, 6, {0, 1}, {1, 0},	{1, 1},0xFFFFFFFF };

	cube_faces[4] = { 6, 5, 7, {0, 1}, {0, 0},	{1, 0},0xFFFFFFFF };
	cube_faces[5] = { 6, 7, 8, {0, 1}, {1, 0},	{1, 1},0xFFFFFFFF };

	cube_faces[6] = { 8, 7, 2, {0, 1}, {0, 0},	{1, 0},0xFFFFFFFF };
	cube_faces[7] = { 8, 2, 1, {0, 1}, {1, 0},	{1, 1},0xFFFFFFFF };

	cube_faces[8] = { 2, 7, 5, {0, 1}, {0, 0},	{1, 0},0xFFFFFFFF };
	cube_faces[9] = { 2, 5, 3, {0, 1}, {1, 0},	{1, 1},0xFFFFFFFF };

	cube_faces[10] = { 6, 8, 1, {0, 1}, {0, 0},	{1, 0}, 0xFFFFFFFF };
	cube_faces[11] = { 6, 1, 4, {0, 1}, {1, 0},	{1, 1}, 0xFFFFFFFF };

	for (int i = 0; i < 8; i++) {
		vect3<float> cube_vertex = cube_vertices[i];
		mesh.vertices.push_back(cube_vertex);
	}
	for (int i = 0; i < 12; i++) {
		face cube_face = cube_faces[i];
		mesh.faces.push_back(cube_face);
	}

}

void Mesh::load_obj_file_data(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");

	}
	else {
		std::vector<UVtext> textcoordinates;
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

			// Texture coodinates information
			if (strcmp(lineHeader, "vt") == 0) {
				UVtext textcoord;
				fscanf(file, "%f %f", &textcoord.u, &textcoord.v);
				textcoordinates.push_back(textcoord);
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
					vi[0] - 1,
					vi[1] - 1,
					vi[2] - 1,
					textcoordinates[ti[0]-1], // a UV
					textcoordinates[ti[1]-1], // b UV
					textcoordinates[ti[2]-1], // c UV
					0xFFFFFFFF
				};

				mesh.faces.push_back(f);

			}

		}
		textcoordinates.clear();
	}
	this->loaded = true;
	std::cout << "Mesh Loaded" << std::endl;
}

// versao alternativa
void Mesh::load_obj_file_dataV2(const char* filename) {
	FILE* file;
	file = fopen(filename, "r");
	char line[1024];

	std::vector<UVtext> texcoords;

	while (fgets(line, 1024, file)) {
		// Vertex information
		if (strncmp(line, "v ", 2) == 0) {
			vect3<float> vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			mesh.vertices.push_back(vertex);
		}
		// Texture coordinate information
		if (strncmp(line, "vt ", 3) == 0) {
			UVtext texcoord;
			sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
			texcoords.push_back(texcoord);
		}
		// Face information
		if (strncmp(line, "f ", 2) == 0) {
			int vertex_indices[3];
			int texture_indices[3];
			int normal_indices[3];
			sscanf(
				line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vertex_indices[0], &texture_indices[0], &normal_indices[0],
				&vertex_indices[1], &texture_indices[1], &normal_indices[1],
				&vertex_indices[2], &texture_indices[2], &normal_indices[2]
			);
			
			face face = {
				vertex_indices[0] - 1,
				vertex_indices[1] - 1,
				vertex_indices[2] - 1,
				texcoords[texture_indices[0] - 1],
				texcoords[texture_indices[1] - 1],
				texcoords[texture_indices[2] - 1],
				0xFFFFFFFF
			};
			mesh.faces.push_back(face);
		}
	}
}

void Mesh::draw_mesh()
{
	// the number of triangles in the array triangles_to_render
	int n_triangles = number_of_triangles_to_render;
	for (int i = 0; i < n_triangles; i++)
	{
		triangle* triangle = &triangles_to_render[i];
		if (display->draw_mode == DRAW_MODE::WIREFRAME) {
			draw->draw_triangle(
				triangle->points[0].x,
				triangle->points[0].y,
				triangle->points[1].x,
				triangle->points[1].y,
				triangle->points[2].x,
				triangle->points[2].y,
				color
			);
		}
		
		if (display->draw_mode == DRAW_MODE::FILLED) {
			draw->draw_zbuffer_fill_triangle(
				triangle->points[0].x,
				triangle->points[0].y,
				triangle->points[0].z,
				triangle->points[0].w,
				triangle->points[1].x,
				triangle->points[1].y,
				triangle->points[1].z,
				triangle->points[1].w,
				triangle->points[2].x,
				triangle->points[2].y,
				triangle->points[2].z,
				triangle->points[2].w,
				triangle->color
			);
		}

		if (display->draw_mode == DRAW_MODE::TEXTURED) {
			draw->draw_textured_triangle(
				triangle->points[0].x, triangle->points[0].y,
				triangle->points[0].z, triangle->points[0].w,
				triangle->textcoords[0].u, triangle->textcoords[0].v,

				triangle->points[1].x, triangle->points[1].y,
				triangle->points[1].z, triangle->points[1].w,
				triangle->textcoords[1].u, triangle->textcoords[1].v,

				triangle->points[2].x, triangle->points[2].y,
				triangle->points[2].z, triangle->points[2].w,
				triangle->textcoords[2].u, triangle->textcoords[2].v,
				&texture
			);
		}

		if (display->draw_mode == DRAW_MODE::TEXTURED_WIREFRAME) {
			// texture

			draw->draw_textured_triangle(
				triangle->points[0].x, triangle->points[0].y,
				triangle->points[0].z, triangle->points[0].w,
				triangle->textcoords[0].u, triangle->textcoords[0].v,

				triangle->points[1].x, triangle->points[1].y,
				triangle->points[1].z, triangle->points[1].w,
				triangle->textcoords[1].u, triangle->textcoords[1].v,

				triangle->points[2].x, triangle->points[2].y,
				triangle->points[2].z, triangle->points[2].w,
				triangle->textcoords[2].u, triangle->textcoords[2].v,
				&texture
			);

			// wireframe
			draw->draw_triangle(
				triangle->points[0].x,
				triangle->points[0].y,
				triangle->points[1].x,
				triangle->points[1].y,
				triangle->points[2].x,
				triangle->points[2].y,
				color
			);
		}

	}

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
