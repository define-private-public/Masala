#version 130

// in
varying vec2 uvMap;
uniform sampler2D spritesheet;
uniform float visibility;
uniform bool drawOutline;
uniform vec4 outlineColor;

void main(void) {
	vec4 t;

	// Either draw the outline or the sprite texture
	if (drawOutline) {
		// Draw a single color for a line-loop
		t = outlineColor;
	} else {
		// Draw a texture
		t = texture2D(spritesheet, uvMap);
	}
	
	// change the visibility
	t.a *= visibility;
	
	// Set the color
	gl_FragColor = t;
}

