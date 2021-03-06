// TODO : mesh.h?Լ? ????
#include "mesh.h"
#include "array.h"
#include "string.h"
#include <stdlib.h>
#include "color.h"

mesh_t g_mesh =
{
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0 ,0, 0 },
    .scale = { 1.0f, 1.0f, 1.0f },
    .translation = { 0, 0, 0},
};

mesh_t g_mesh2 =
{
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0,0,0},
    .scale = { 1.0f, 1.0f, 1.0f },
    .translation = { 0, 0, 0},
};

vec3_t g_cube_vertices[N_CUBE_VERTICES] = 
{
    {.x = -1, .y = -1, .z = -1 },   // 1
    {.x = -1, .y =  1, .z = -1 },   // 2
    {.x =  1, .y =  1, .z = -1 },   // 3
    {.x =  1, .y = -1, .z = -1 },   // 4
    {.x =  1, .y =  1, .z =  1 },   // 5
    {.x =  1, .y = -1, .z =  1 },   // 6
    {.x = -1, .y =  1, .z =  1 },   // 7
    {.x = -1, .y = -1, .z =  1 }    // 8
};

face_t g_cube_faces[N_CUBE_FACES] = 
{
    // front
    {.a = 1, .b = 2, .c = 3, .color = White },
    {.a = 1, .b = 3, .c = 4, .color = White },
    // right               
    {.a = 4, .b = 3, .c = 5, .color = White },
    {.a = 4, .b = 5, .c = 6, .color = White },
    // back                           
    {.a = 6, .b = 5, .c = 7, .color = White },
    {.a = 6, .b = 7, .c = 8, .color = White },
    // left                           
    {.a = 8, .b = 7, .c = 2, .color = White },
    {.a = 8, .b = 2, .c = 1, .color = White },
    // top                            
    {.a = 2, .b = 7, .c = 5, .color = White },
    {.a = 2, .b = 5, .c = 3, .color = White },
    // bottom                         
    {.a = 6, .b = 8, .c = 1, .color = White },
    {.a = 6, .b = 1, .c = 4, .color = White }
};

void load_cube_mesh_data(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; i++)
    {
        vec3_t cube_vertex = g_cube_vertices[i];
        array_push(g_mesh2.vertices, cube_vertex);
    }

    for (int i = 0; i < N_CUBE_FACES; i++)
    {
        face_t cube_face = g_cube_faces[i];
        array_push(g_mesh2.faces, cube_face);
    }
}

void load_obj_file_data(char* filename)
{
    FILE* fr = NULL;
    char ch;
    float num = 0.f;
    unsigned long count = 0;
    char buffer[256];

    fopen_s(&fr, filename, "r");
    if (fr == NULL)
    {
        printf("Can't open %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fr) != NULL)
    {
        char* context = NULL;
        char first = buffer[0];
        char second = buffer[1];
        vec3_t v = { 0, 0, 0 };
        if (first == 'v' && second == ' ')
        {
            float arr[3] = {0.f, 0.f, 0.f};
            int cnt = 0;
            char* ptr = strtok_s(buffer, " v", &context);
            while (ptr != NULL)
            {
                arr[cnt++] = (float)atof(ptr);

                //printf(ptr);
                ptr = strtok_s(NULL, " v", &context);
            }

            v.x =  arr[0]; 
            v.y = -arr[1]; 
            v.z =  arr[2];
            array_push(g_mesh2.vertices, v);
        }

        face_t f = { 0, 0, 0 };
        if (first == 'f' && second == ' ')
        {
            int arr[3] = {0,0,0};
            int cnt = 0;
            char* ptr = strtok_s(buffer, " f", &context);
            while (ptr != NULL)
            {
                arr[cnt++] = atoi(ptr);
                //printf(ptr);
                ptr = strtok_s(NULL, " f", &context);
            }

            f.a = arr[0]; 
            f.b = arr[1]; 
            f.c = arr[2];
            f.color = White;
            array_push(g_mesh2.faces, f);
        }
        count++;
    }

    fclose(fr);
    // printf("FILE %s has %lu character\n", filename, count);
}

void load_obj_file_data2(char* filename)
{
    FILE* file = NULL;
    fopen_s(&file, filename, "r");
    if (file == NULL)
    {
        printf("Can't open %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0)
        {
            vec3_t vertex;
            sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(g_mesh2.vertices, vertex);
        }

        // Face Information
        if (strncmp(line, "f ", 2) == 0)
        {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf_s(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );
            face_t face =
            {
                .a = vertex_indices[0],
                .b = vertex_indices[1],
                .c = vertex_indices[2],
                .color = White,
            };
            array_push(g_mesh2.faces, face);
        }
    }
}