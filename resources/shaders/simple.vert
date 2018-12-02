#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;	// texture coordinates _ ass4

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

// values to be passed onto the fragment shader
out vec3 pass_Normal;
out vec3 pass_FragPos;	// fragment position _ ass3
out vec3 pass_CameraPos;	// eye (camera) position _ ass3
out vec2 pass_TexCoord;	// texture coordinates _ ass4

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Normal = mat3(ModelMatrix) * in_Normal;
	pass_FragPos = vec3(ModelMatrix * vec4(in_Position, 1.0));
	pass_CameraPos = (ModelMatrix * ViewMatrix * vec4(in_Position, 1.0)).xyz;
	pass_TexCoord = in_TexCoord;	// ass4
}
