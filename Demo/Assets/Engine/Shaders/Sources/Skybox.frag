uniform samplerCube uCubemap;

varying vec3 vPosition;

void main() {
	gl_FragColor = textureCube(uCubemap, vPosition);
} 