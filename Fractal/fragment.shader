//FRAGMENT SHADER
#version 330 core

const int MAX_MARCHING_STEPS = 255;
//float MIN_DIST = 0.1;
const float MAX_DIST = 1000.0;
const float EPSILON = 0.00001;

const int RECURSE_NUM = 10;
const float POWER = 8;

struct Lighting {
	float dist;
	int steps;
	vec4 normal;
	vec4 pos;
};

in vec2 gl_FragCoord;
out vec4 gl_FragColor;

uniform vec2 windowSize;
uniform float time;
uniform mat4 transform;
uniform mat4 foldTransform;

uniform float MIN_DIST;

float mandelbulbSDF(vec4 p) {
	vec4 z = p;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < RECURSE_NUM; i++) {
		r = length(z);
		if (r > 10) break;

		float theta = acos(z.z / r);
		float phi = atan(z.y, z.x);
		dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

		float zr = pow(r, POWER);
		theta = theta * POWER;
		phi = phi * POWER;

		z = zr * vec4(
			sin(theta) * cos(phi),
			sin(phi) * sin(theta),
			cos(theta),
			1.0);
		z += p;
	}
	return 0.5 * log(r) * r / dr;
}

float sphereSDF(vec4 p) {
	return length(p) - 1.0;
}

float boxSDF(vec4 p, vec4 b) {
	vec4 d = abs(p) - b;
	return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float sceneSDF(vec4 p) {
	return mandelbulbSDF(p);
}

vec4 estimateNormal(vec4 p) {
	return normalize(vec4(
		sceneSDF(vec4(p.x + EPSILON, p.y, p.z, 0.0)) - sceneSDF(vec4(p.x - EPSILON, p.y, p.z, 0.0)),
		sceneSDF(vec4(p.x, p.y + EPSILON, p.z, 0.0)) - sceneSDF(vec4(p.x, p.y - EPSILON, p.z, 0.0)),
		sceneSDF(vec4(p.x, p.y, p.z + EPSILON, 0.0)) - sceneSDF(vec4(p.x, p.y, p.z - EPSILON, 0.0)),
		0.0
	));
}

Lighting shortestDistanceToSurface(vec4 marchingDirection, float start, float end) {
	float depth = start;
	vec4 p;
	vec4 cam;
	vec4 fold;
	for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
		p = depth * marchingDirection;
		cam = transform * vec4(p.xyz, 1.0);
		fold = foldTransform * p;
		//if (fold.x + fold.y < 0) p.xy = -p.yx; // fold 1
		//if (fold.x + fold.z < 0) p.xz = -p.zx; // fold 2
		//if (fold.y + fold.z < 0) p.zy = -p.yz; // fold 3
		float dist = sceneSDF(cam);
		if (dist < EPSILON) {
			return Lighting(depth, i, estimateNormal(cam), p);
		}
		depth += dist;
		if (depth >= end) {
			return Lighting(depth, i, estimateNormal(cam), p);
		}
	}
	return Lighting(depth, MAX_MARCHING_STEPS, estimateNormal(transform * p), p);
}

vec4 rayDirection(float fieldOfView) {
	vec2 xy = gl_FragCoord - windowSize / 2.0;
	float z = windowSize.y / tan(radians(fieldOfView) / 2.0);
	return normalize(vec4(xy, -z, 0.0));
}

vec4 normalToLight(vec4 normal) {
	vec4 colour = vec4(0.0);
	if (normal.x > 0.0) colour += vec4(1.0, 0.0, 0.0, 0.0) * normal.x;
	else colour += vec4(0.0, 1.0, 0.0, 0.0) * (-normal.x);
	if (normal.y > 0.0) colour += vec4(0.5, 0.0, 0.5, 0.0) * normal.y;
	else colour += vec4(1.0, 1.0, 0.0, 0.0) * (-normal.y);
	if (normal.z > 0.0) colour += vec4(0.0, 0.8, 1.0, 0.0) * normal.z;
	else colour += vec4(1.0, 0.53, 0.0, 0.0) * (-normal.z);
	return normalize(colour);
}

void main() {
	vec4 dir = rayDirection(60.0);
	Lighting light = shortestDistanceToSurface(dir, MIN_DIST, MAX_DIST);

	float occlusion = 1.0 - float(light.steps) / (float(light.steps) + 100.0);

	if (light.dist > MAX_DIST - EPSILON) {
		gl_FragColor = vec4(1 - occlusion);
		return;
	}
	gl_FragColor = vec4(pow(occlusion, 10.0));
	//gl_FragColor = vec4(pow(occlusion * normalToLight(light.normal), vec3(2.0)));
	//gl_FragColor = vec4(occlusion * length(light.pos / (light.pos + vec3(4.0))));
}