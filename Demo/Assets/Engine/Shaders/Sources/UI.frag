uniform sampler2D uTexture;

varying vec2 vTexCoords;
varying vec4 vColor;

void main() {
	gl_FragColor = texture2D(uTexture, vTexCoords) * vColor;
} 