#include "graphic_pipeline.h"
#include "macros.h"


GraphicPipeline::GraphicPipeline()
{
	LOG("Starting the Graphic Pipeline");		

}

GraphicPipeline::~GraphicPipeline()
{
	LOG("Stoping the Graphic Pipeline");
}

void GraphicPipeline::read_obj_file(const char* filename)
{
	_mesh.load_obj_file_dataV2(filename);
	_mesh.set_display(display);
}

void GraphicPipeline::load_texture(const char* filename)
{
	_mesh.texture.load_png_image(filename);
}

void GraphicPipeline::process_image()
{

	/* this function calcule all matrix multiplications, backface culling, flat shading
	 of mesh, and project into screen space */

	
	/* reset the number_of_triangles */
	_mesh.number_of_triangles_to_render = 0;

	vect3<float> target{ 0, 0, 300 };
	Mat4x4 camera_yaw_rotation = Mat4x4_MakeRotationY(Display::camera->yaw);
	Display::camera->direction = static_cast<vect3<float>>(
		Mat4x4_MultiplyVector<float>(
			camera_yaw_rotation,
			static_cast<vect4<float>>(target)
			)
		);

	target = Display::camera->position + Display::camera->direction;

	Mat4x4 view_matrix = Mat4x4_Look_At(
		Display::camera->position,
		target,
		vect3<float>{ 0, 1, 0 }
	);

	/* the struct mesh inside Mesh Class */
	mesh_t mesh = _mesh.mesh;

	/* Matrix to transformations */
	Mat4x4 scale =       Mat4x4_MakeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	Mat4x4 rotateX =     Mat4x4_MakeRotationX(mesh.rotation.x);
	Mat4x4 rotateY =     Mat4x4_MakeRotationY(mesh.rotation.y);
	Mat4x4 rotateZ =     Mat4x4_MakeRotationZ(mesh.rotation.z);
	Mat4x4 translation = Mat4x4_MakeTranslation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	Mat4x4 project_matrix = Mat4x4_MakePerspective(
		Display::camera->fov_y,
		Display::camera->aspect_y,
		Display::camera->znear,
		Display::camera->zfar
	);

	display->frustum->init_frustum_planes(
		Display::camera->fov_x,
		Display::camera->fov_y,
		Display::camera->znear,
		Display::camera->zfar
	);


	/* números de faces na malha */
	int n_faces = mesh.faces.size();
	for (int i = 0; i < n_faces; i++)
	{
		if (i < 2)continue;
		/* setando a face atual da coleção de faces da malha */
		face  mesh_face = mesh.faces[i];
		/* setando a cor selecionada pelo imgui */
		mesh_face.color = _mesh.get_color();
		/* vetor responsavel por armazenar os vertices da face */
		vect3<float> face_vertex[3];

		/* Populando os vertices da face */
		face_vertex[0] = mesh.vertices[mesh_face.a];
		face_vertex[1] = mesh.vertices[mesh_face.b];
		face_vertex[2] = mesh.vertices[mesh_face.c];
		vect4<float> transformed_vertices[3];
		for (int j = 0; j < 3; j++)
		{
			/* Espaço do modelo */
			vect4<float> transformed_vertex = static_cast<vect4<float>>(face_vertex[j]);
			
			/* Espaço do mundo */
			Mat4x4 world_matrix = Mat4x4_MakeIdentity();
			world_matrix = Mat4x4_MultiplyMatrix(scale, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateY, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateX, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateZ, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(_mesh.shearXY_matrix, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(_mesh.shearYX_matrix, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(translation, world_matrix);
			transformed_vertex = Mat4x4_MultiplyVector(world_matrix, transformed_vertex);
			
			/* Espaço da viewport*/
			transformed_vertex = Mat4x4_MultiplyVector(view_matrix, transformed_vertex);
			transformed_vertices[j] = transformed_vertex;
		}

		/*
		       _                 _ _          _   _             
			  (_)               | (_)        | | (_)            
		__   ___ ___ _   _  __ _| |_ ______ _| |_ _  ___  _ __  
		\ \ / / / __| | | |/ _` | | |_  / _` | __| |/ _ \| '_ \ 
		 \ V /| \__ \ |_| | (_| | | |/ / (_| | |_| | (_) | | | |
		  \_/ |_|___/\__,_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|
		
		*/

		/* Famoso backface culling*/
		vect3<float> normal_ray_camera = {};
		if (display->drawing_type == BACKFACE_TYPE::CULLING) {
			vect3<float> a = static_cast<vect3<float>>(transformed_vertices[0]);
			vect3<float> b = static_cast<vect3<float>>(transformed_vertices[1]);
			vect3<float> c = static_cast<vect3<float>>(transformed_vertices[2]);
			bool back_face = TriangleHelper::back_face_culling(a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z,	&normal_ray_camera);
			if (back_face) {
				continue;
			}
		}

		/* Sombreamento*/
		if (display->shading == SHADING_TYPE::FLAT) {			
			mesh_face.color = flat_shading(normal_ray_camera, mesh_face.color);
		}

		/* Clipping polygons*/
		triangle triangles_clipped[MAX_NUM_POLYGONS_TRIANGLES];
		int num_triangles_clipped = 0;

		polygon polygon = PolygonHelper::create_polygon_from_triangle(
			static_cast<vect3<float>>(transformed_vertices[0]),
			static_cast<vect3<float>>(transformed_vertices[1]),
			static_cast<vect3<float>>(transformed_vertices[2]),
			mesh_face.a_uv,
			mesh_face.b_uv,
			mesh_face.c_uv
		);
		
		PolygonHelper::clip_polygon(&polygon, display->frustum);		
		TriangleHelper::triangles_from_polygon(&polygon, triangles_clipped, &num_triangles_clipped);

		/* Espaço da tela*/
		for (int t = 0; t < num_triangles_clipped; t++)
		{

			triangle temp_triangle_clipped = triangles_clipped[t];
			vect4<float> projected_points[3];

			for (int j = 0; j < 3; j++)
			{
				projected_points[j] = Mat4x4_MultiplyProjectVector(project_matrix, temp_triangle_clipped.points[j]);
				projected_points[j].x *= (display->width / 2.0);
				projected_points[j].y *= (display->height / 2.0);

				/* invert the y values of mesh*/
				projected_points[j].y *= -1;

				/* Translate the projected points to the middle of the screen*/
				projected_points[j].x += (display->width / 2.0);
				projected_points[j].y += (display->height / 2.0);
			}



			triangle projected_triangle = {
				{
					{ projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
					{ projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
					{ projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w },
				},
				{
					{temp_triangle_clipped.textcoords[0].u,temp_triangle_clipped.textcoords[0].v},
					{temp_triangle_clipped.textcoords[1].u,temp_triangle_clipped.textcoords[1].v},
					{temp_triangle_clipped.textcoords[2].u,temp_triangle_clipped.textcoords[2].v}
				},
				mesh_face.color,
				0
			};

			if (_mesh.number_of_triangles_to_render < MAX_TRIANGLES_PER_MESH) {
				_mesh.triangles_to_render[_mesh.number_of_triangles_to_render] = projected_triangle;
				_mesh.number_of_triangles_to_render++;
			}
			else { break; }
		}
		
	}
}

void GraphicPipeline::process_image2()
{
	/* this function calcule all matrix multiplications, backface culling, flat shading
	 of mesh, and project into screen space */


	 /* reset the number_of_triangles */
	_mesh.number_of_triangles_to_render = 0;

	vect3<float> target{ 0, 0, 300 };
	Mat4x4 camera_yaw_rotation = Mat4x4_MakeRotationY(Display::camera->yaw);
	Display::camera->direction = static_cast<vect3<float>>(
		Mat4x4_MultiplyVector<float>(
			camera_yaw_rotation,
			static_cast<vect4<float>>(target)
			)
		);

	target = Display::camera->position + Display::camera->direction;

	Mat4x4 view_matrix = Mat4x4_Look_At(
		Display::camera->position,
		target,
		vect3<float>{ 0, 1, 0 }
	);

	/* the struct mesh inside Mesh Class */
	mesh_t mesh = _mesh.mesh;

	/* Matrix to transformations */
	Mat4x4 scale = Mat4x4_MakeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	Mat4x4 rotateX = Mat4x4_MakeRotationX(mesh.rotation.x);
	Mat4x4 rotateY = Mat4x4_MakeRotationY(mesh.rotation.y);
	Mat4x4 rotateZ = Mat4x4_MakeRotationZ(mesh.rotation.z);
	Mat4x4 translation = Mat4x4_MakeTranslation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	Mat4x4 project_matrix = Mat4x4_MakePerspective(
		Display::camera->fov_y,
		Display::camera->aspect_y,
		Display::camera->znear,
		Display::camera->zfar
	);

	display->frustum->init_frustum_planes(
		Display::camera->fov_x,
		Display::camera->fov_y,
		Display::camera->znear,
		Display::camera->zfar
	);


	/* números de faces na malha */
	int n_faces = mesh.faces.size();
	for (int i = 0; i < n_faces; i++)
	{
		if (i < 2)continue;
		/* setando a face atual da coleção de faces da malha */
		face  mesh_face = mesh.faces[i];
		/* setando a cor selecionada pelo imgui */
		mesh_face.color = _mesh.get_color();
		/* vetor responsavel por armazenar os vertices da face */
		vect3<float> face_vertex[3];

		/* Populando os vertices da face */
		face_vertex[0] = mesh.vertices[mesh_face.a];
		face_vertex[1] = mesh.vertices[mesh_face.b];
		face_vertex[2] = mesh.vertices[mesh_face.c];
		vect4<float> transformed_vertices[3];
		for (int j = 0; j < 3; j++)
		{
			/* Espaço do modelo */
			vect4<float> transformed_vertex = static_cast<vect4<float>>(face_vertex[j]);

			/* Espaço do mundo */
			Mat4x4 world_matrix = Mat4x4_MakeIdentity();
			world_matrix = Mat4x4_MultiplyMatrix(scale, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateY, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateX, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(rotateZ, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(_mesh.shearXY_matrix, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(_mesh.shearYX_matrix, world_matrix);
			world_matrix = Mat4x4_MultiplyMatrix(translation, world_matrix);
			transformed_vertex = Mat4x4_MultiplyVector(world_matrix, transformed_vertex);

			/* Espaço da viewport*/
			transformed_vertex = Mat4x4_MultiplyVector(view_matrix, transformed_vertex);
			transformed_vertices[j] = transformed_vertex;
		}

		/*
			   _                 _ _          _   _
			  (_)               | (_)        | | (_)
		__   ___ ___ _   _  __ _| |_ ______ _| |_ _  ___  _ __
		\ \ / / / __| | | |/ _` | | |_  / _` | __| |/ _ \| '_ \
		 \ V /| \__ \ |_| | (_| | | |/ / (_| | |_| | (_) | | | |
		  \_/ |_|___/\__,_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|

		*/

		/* Famoso backface culling*/
		vect3<float> normal_ray_camera = {};
		if (display->drawing_type == BACKFACE_TYPE::CULLING) {
			vect3<float> a = static_cast<vect3<float>>(transformed_vertices[0]);
			vect3<float> b = static_cast<vect3<float>>(transformed_vertices[1]);
			vect3<float> c = static_cast<vect3<float>>(transformed_vertices[2]);
			bool back_face = TriangleHelper::back_face_culling(a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z, &normal_ray_camera);
			if (back_face) {
				continue;
			}
		}

		/* Sombreamento*/
		if (display->shading == SHADING_TYPE::FLAT) {
			mesh_face.color = flat_shading(normal_ray_camera, mesh_face.color);
		}


		vect4<float> projected_points[3];

		for (int j = 0; j < 3; j++)
		{
			projected_points[j] = Mat4x4_MultiplyProjectVector(project_matrix, transformed_vertices[j]);
		}

		triangle triangles_clipped[MAX_NUM_POLYGONS_TRIANGLES];
		int num_triangles_clipped = 0;
		// criando o poligono a partir dos vertices da face no espaço-NDCS
		polygon4 polygon = PolygonHelper::create_polygon_from_triangle(
			projected_points[0],
			projected_points[1],
			projected_points[2],
			mesh_face.a_uv,
			mesh_face.b_uv,
			mesh_face.c_uv
		);

		PolygonHelper::ndcs_clip_polygon(&polygon);
		TriangleHelper::triangles_from_polygon(&polygon, triangles_clipped, &num_triangles_clipped);
		//LOG("Triangulos apos recorte " << num_triangles_clipped);

		/* Espaço da tela*/
		for (int t = 0; t < num_triangles_clipped; t++)
		{
			triangle temp_triangle_clipped = triangles_clipped[t];
			projected_points[0] = temp_triangle_clipped.points[0];
			projected_points[1] = temp_triangle_clipped.points[1];
			projected_points[2] = temp_triangle_clipped.points[2];
			

			for (int j = 0; j < 3; j++)
			{
				projected_points[j].x *= (display->width / 2.0);
				projected_points[j].y *= (display->height / 2.0);

				/* invert the y values of mesh*/
				projected_points[j].y *= -1;

				/* Translate the projected points to the middle of the screen*/
				projected_points[j].x += (display->width / 2.0);
				projected_points[j].y += (display->height / 2.0);
			}



			triangle projected_triangle = {
				{
					{ projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
					{ projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
					{ projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w },
				},
				{
					{temp_triangle_clipped.textcoords[0].u,temp_triangle_clipped.textcoords[0].v},
					{temp_triangle_clipped.textcoords[1].u,temp_triangle_clipped.textcoords[1].v},
					{temp_triangle_clipped.textcoords[2].u,temp_triangle_clipped.textcoords[2].v}
				},
				mesh_face.color,
				0
			};

			if (_mesh.number_of_triangles_to_render < MAX_TRIANGLES_PER_MESH) {
				_mesh.triangles_to_render[_mesh.number_of_triangles_to_render] = projected_triangle;
				_mesh.number_of_triangles_to_render++;
			}
			else { break; }
		}

	}
}

void GraphicPipeline::draw_pipeline()
{

	uint32_t color                    =	_mesh.get_color();
	int number_of_triangles_to_render = _mesh.number_of_triangles_to_render;
	triangle* triangles_to_render     =	_mesh.triangles_to_render;
	OSTexture texture                 =	_mesh.texture;

	/* the number of triangles in the array triangles_to_render */
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


void GraphicPipeline::set_display(Display* display)
{
	this->display = display;
}

void GraphicPipeline::set_drawing(Drawing* draw)
{
	this->draw = draw;
}

Mesh* GraphicPipeline::get_mesh()
{
	return &_mesh;
}

uint32_t GraphicPipeline::flat_shading(vect3<float> normal, uint32_t old_color)
{
	float light_percentage = -vdotvect(normal, vect3<float>{ 0, 0, 1 });
	return light_apply_intensity(old_color, light_percentage);
}

void GraphicPipeline::painters_algorithm()
{
	/*
	
		calculate the depth of triangle	 
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0f;


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

		int num_triangles = number_of_triangles_to_render;
		TriangleHelper::quick_sort_avg_depth(triangles_to_render, 0, num_triangles);
	
	*/
}
