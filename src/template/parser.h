#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>

namespace parser
{
    //Notice that all the structures are as simple as possible
    //so that you are not enforced to adopt any style or design.
    struct p_vec3f
    {
        float x, y, z;
    };

    struct p_vec3i
    {
        int x, y, z;
    };

    struct p_vec4f
    {
        float x, y, z, w;
    };

    struct p_camera
    {
        p_vec3f position;
        p_vec3f gaze;
        p_vec3f up;
        p_vec4f near_plane;
        float near_distance;
        int image_width, image_height;
        std::string image_name;
    };

    struct p_point_light
    {
        p_vec3f position;
        p_vec3f intensity;
    };

    struct p_material
    {
        bool is_mirror;
        p_vec3f ambient;
        p_vec3f diffuse;
        p_vec3f specular;
        p_vec3f mirror;
        float phong_exponent;
    };

    struct p_face
    {
        int v0_id;
        int v1_id;
        int v2_id;
    };

    struct p_mesh
    {
        int material_id;
        std::vector<p_face> faces;
    };

    struct p_triangle
    {
        int material_id;
        p_face indices;
    };

    struct p_sphere
    {
        int material_id;
        int center_vertex_id;
        float radius;
    };

    struct p_scene
    {
        //Data
        p_vec3i background_color;
        float shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<p_camera> cameras;
        p_vec3f ambient_light;
        std::vector<p_point_light> point_lights;
        std::vector<p_material> materials;
        std::vector<p_vec3f> vertex_data;
        std::vector<p_mesh> meshes;
        std::vector<p_triangle> triangles;
        std::vector<p_sphere> spheres;

        //Functions
        void loadFromXml(const std::string &filepath);
    };
}

#endif
