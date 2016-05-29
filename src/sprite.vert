#version 130

// in
uniform mat4 viewMatrix;
uniform mat4 rotMatrix;
attribute vec2 vertex;
attribute vec2 tex;
uniform float z;
uniform vec2 pos;
uniform vec2 origin;
uniform vec2 rotationOrigin;
uniform vec2 scale;

// out
varying vec2 uvMap;

void main(void) {
	// The vertex
	vec4 v = vec4(vertex, z, 1);

	// Scale
	v.xy *= scale;

	// Apply the rotation matrix
	v.xy -= rotationOrigin;
	v *= rotMatrix;
	v.xy += rotationOrigin;

	// Translate
	v.xy += pos - origin;

	// Position
	gl_Position = viewMatrix * v;

	// Pipe through the texture variables
	uvMap = tex;
}

