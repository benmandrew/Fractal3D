//FRAGMENT SHADER
#version 330 core

in vec2 gl_FragCoord;
out vec4 gl_FragColor;

uniform vec2 windowSize;
uniform float time;

uniform int iteration_num;

uniform float cx, cy;
uniform float zoom, moveX, moveY;

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
	float newX = 1.5 * (gl_FragCoord.x - windowSize.x * 0.5) / (zoom * windowSize.x * 0.5) + moveX;
	float newY = (gl_FragCoord.y - windowSize.y * 0.5) / (zoom * windowSize.y * 0.5) + moveY;
	float oldX, oldY;
	int i;
	for (i = 0; i < iteration_num; i++) {
		oldX = newX;
		oldY = newY;
		newX = oldX * oldX - oldY * oldY + cx;
		newY = 2 * oldX * oldY + cy;
		if ((newX * newX + newY * newY) > 4.0) break;
	}
	if (i < iteration_num) gl_FragColor = vec4(hsv2rgb(vec3(i / float(iteration_num), 1.0, 0.5)), 0.0);
	else gl_FragColor = vec4(0.0);
}