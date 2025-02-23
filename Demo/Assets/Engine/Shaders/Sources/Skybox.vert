uniform mat4 uCustomMVP;

attribute vec3 aPosition;

varying vec3 vPosition;

void main() {
	gl_Position = uCustomMVP * vec4(aPosition, 1.0);
	vPosition = aPosition;
}