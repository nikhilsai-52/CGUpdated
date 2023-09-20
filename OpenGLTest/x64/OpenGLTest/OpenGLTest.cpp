// Based on templates from learnopengl.com
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Vector3D.h>
#include<vector>
#include <rayhit.h>
#include<algorithm>
#include<Light.h>
#define GLFW_INCLUDE_NONE
#include <GL/gl.h>


#include <iostream>

void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, TexCoord);\n"
"}\n\0";

using color = Vector3D;
const double EPSILON = 0.0001;


double write_colorR(std::ostream& out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    return static_cast<int>(255.999 * pixel_color.x());
       
}

double write_colorG(std::ostream& out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    return static_cast<int>(255.999 * pixel_color.y());

}

double write_colorB(std::ostream& out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    return static_cast<int>(255.999 * pixel_color.z());

}



/* display color RGB
 double displayColorR(color pix_color) {
    return pix_color.x();
}
 double displayColorG(color pix_color) {
     return pix_color.y();
 }
 double displayColorB(color pix_color) {
     return pix_color.z();
 }
 */

double hit_sphere(const point3D& center, double radius, const ray& r) {
    Vector3D oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    auto root = ( - b - sqrt(discriminant)) / (2.0 * a);
    //std::cout << "a: " << a << ", b: " << b << ", c: " << c << ", discriminant: " << discriminant << "\n";//debug
    //return (discriminant >= 0);
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return root;
    }

}

bool hit_plane(const point3D& pointOnPlane, const Vector3D& normal, const ray& r, double& t) {
    auto denominator = dot(normal, r.direction());
    if (denominator > 1e-6) { // Check for non-parallel ray and plane
        auto p0l0 = pointOnPlane - r.origin();
        t = dot(p0l0, normal) / denominator;
        return (t >= 0);
    }
    return false;
}

//color ray_color(const ray& r,int depth) {
//    if (depth <= 0) {
//        return color(0, 0, 0); // Return black if we've reached the maximum recursion depth.
//    }
//
//    // Sphere 1 dimensions
//    point3D sphere_center1(-0.5, 0.5, 2);
//    double sphere_radius1 = 0.5;
//
//    // Plane dimensions
//    point3D plane_point(0, -1,0); // A point on the plane
//    Vector3D plane_normal(0, 1, 0); // Normal vector of the plane
//
//    // Sphere 2 dimensions
//    point3D sphere_center2(-1, 0, 3);
//    double sphere_radius2 = 1;
//
//    auto t1 = hit_sphere(sphere_center1, sphere_radius1, r);
//    auto t2 = 0.0; // Initialize to zero, will be updated if ray hits the plane
//    auto t3 = hit_sphere(sphere_center2, sphere_radius2, r);
//
//    // Check for plane intersection
//    if (hit_plane(plane_point, plane_normal, r, t2)) {
//       // if (t2 < t1 && t2 < t3) {
//            return color(0, 0, 1); // Blue color for the plane
//        //}
//    }
//
//    if (t1 > 0) {
//        Vector3D N = unit_vector(r.at(t1) - sphere_center1);
//        color reflection_color = ray_color(ray(r.at(t1), r.direction() - 2 * dot(r.direction(), N) * N), depth - 1);
//        return (0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1)+ +0.5 * reflection_color);
//    }
//
//    if (t3 > 0) {
//        Vector3D N = unit_vector(r.at(t3) - sphere_center2);
//        return (0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1));
//    }
//
//    std::cout << "No Sphere or Plane Hit.\n";
//    return color(1, 1, 1); // White color for no hit
//}

color ray_color(const ray& r) {
    /*
    if (hit_sphere(point3D(0, 0, -1), 0.5, r))
        return color(0.0, 1.0, 0.0);
    return color(1.0, 1.0, 1.0); 
    */

    // two spheres

    //Sphere 1 dimensions
    point3D sphere_center1(0, 1, 2);
    double sphere_radius1 = 0.5;
    // Plane dimensions
    point3D sphere_center2(100.5,0, 2);
    double sphere_radius2 = 100;
    // Sphere 2 dimensions
    point3D sphere_center3(-0.5, 0, 2);
    double sphere_radius3 = 0.5;

    //hit Sphere 1
    auto t1 = hit_sphere(sphere_center1, sphere_radius1, r);
    //hit Sphere 2
    auto t3 = hit_sphere(sphere_center3, sphere_radius3, r);

    if (t1 > 0) {
        //Vector3D N = unit_vector(r.at(t1) - sphere_center1);
        ///**/Vector3D loc1 = *new Vector3D(-8, 5, 0);
        //Vector3D lightDir = unit_vector(loc1 - r.origin());
        //double lightIntensity = dot(r.direction(), lightDir);*/
        //color surface_col = *new color(0, 1, 1);
        ////return(surface_col);
        //return Light::illuminate(r,surface_col,r.at(t1), N);
        ////return(0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1));*/
        ////double diffuseFactor = std::max(dot(N, lightDir), 0.0);
        ////color diffuseColor = *new Vector3D(0.5, 0.5, 1);/* Diffuse color of the material */
        ////color diffuseContribution = lightIntensity * diffuseFactor * diffuseColor;

        //return diffuseContribution;
        return (color(0, 1, 0));

    }

    //hit plane
    auto t2 = hit_sphere(sphere_center2, sphere_radius2, r);
    if (t2 > 0) {
        Vector3D N = unit_vector(r.at(t2) - sphere_center2);
        return(0.5 * color(0,0,1));
    }

    

    if (t3 > 0) {
        /*Vector3D N = unit_vector(r.at(t3) - sphere_center3);
        color surface_col = *new color(0, 1, 0);
        return Light::illuminate(r, surface_col, r.at(t1), N);*/
        return(color(0, 0, 1));
        //return(0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1));

    }

    


    //if (hit_sphere(sphere_center1, sphere_radius1, r)) {
    //    std::cout << "Sphere Hit!\n";
    //    return color(0, 1, 0); // Green color for sphere hit
    //}

    //if (hit_sphere(sphere_center2, sphere_radius2, r)) {
    //    std::cout << "Sphere Hit!\n";
    //    return color(0,0,1); // Green color for sphere hit
    //}
    
    //std::cout << "No Sphere Hit.\n";
    return color(0.43, 0.43, 0.43); // White color for no hit
    /*Vector3D unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);*/
}





int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Display RGB Array", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create the image (RGB Array) to be displayed
    const int width = 512; // keep it in powers of 2!
    const int height = 512; // keep it in powers of 2!

    // Image

    //auto aspect_ratio = 16.0 / 9.0;
    //const width = 512;

    //// Calculate the image height, and ensure that it's at least 1.
    //auto height = static_cast<int>(width / aspect_ratio);
    //auto height = (height < 1) ? 1 : height;

    
    //light
    Light light = *new Light;



    //camera
    double focal_length = 1.0; //change the variable
    double viewh = 2.0; // view auto height
    double vieww = viewh * ((double)(width) / height); //viewport width
    auto camera_center = point3D(0, 0, 0);

    //vectors across view
    auto vu = Vector3D(vieww, 0, 0);
    auto vv = Vector3D(0, -viewh, 0);

    //vector diff
    auto diff_u = vu / width;
    auto diff_v = vv / height;

    auto v_upperleft = camera_center - Vector3D(0, 0, focal_length) - vu / 2 - vv / 2;
    auto zerozero_pixel = v_upperleft + 0.5 * (diff_u + diff_v) ;
   
    unsigned char image[width * height * 3];
    for (int i = 0; i < height; i++)
    {
        std::clog << "\r scanlines remaining: " << (height - i) << std::flush;

        for (int j = 0; j < width; j++)
        {
            
            auto pix_center = zerozero_pixel + (i * diff_u) + (j * diff_v);
            
            auto ray_dir = pix_center - camera_center;
            //std::cout << pix_center << ray_dir;
            ray r(camera_center, ray_dir);

            color pix_color = ray_color(r);

            int idx = (i * width + j) * 3;
            image[idx] = write_colorR(std::cout,pix_color);
            image[idx + 1] = write_colorG(std::cout, pix_color); 
            image[idx + 2] = write_colorB(std::cout, pix_color);
        }
    }

   

   // std::clog << "\r Done.    \n";

    unsigned char* data = &image[0];
   
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
