#version 330 core

// All of the following variables could be defined in the OpenGL
// program and passed to this shader as uniform variables. This
// would be necessary if their values could change during runtim.
// However, we will not change them and therefore we define them 
// here for simplicity.

//vec3 I = vec3(1, 1, 1);          // point light intensity
//vec3 Iamb = vec3(0.8, 0.8, 0.8); // ambient light intensity
//vec3 kd = vec3(1, 0.2, 0.2);     // diffuse reflectance coefficient
//vec3 ka = vec3(0.3, 0.3, 0.3);   // ambient reflectance coefficient
//vec3 ks = vec3(0.8, 0.8, 0.8);   // specular reflectance coefficient
vec3 lightPos = vec3(5, 5, 5);   // light position in world coordinates

uniform vec3 eyePos;

in vec4 fragWorldPos;
in vec3 fragWorldNor;

out vec4 fragColor;

uniform vec3 offset; // Offset for the pattern
uniform float scale; // Scale for the pattern
uniform vec3 color1; // Color of the first set of squares
uniform vec3 color2; // Color of the second set of squares


void main(void)
{
	// Compute lighting. We assume lightPos and eyePos are in world
	// coordinates. fragWorldPos and fragWorldNor are the interpolated
	// coordinates by the rasterizer.

	bool x = int((fragWorldPos.x + offset.x) * scale) % 2 == 0;
    bool y = int((fragWorldPos.y + offset.y) * scale) % 2 == 0;
    bool z = int((fragWorldPos.z + offset.z) * scale) % 2 == 0;
    bool xorXY = (x != y);


	fragColor = xorXY != z ? vec4(color1, 1.0) : vec4(color2, 1.0);
	//fragColor = vec4(specularColor + ambientColor, 1); //or try this, don't forget to change the active program in main.cpp
}
