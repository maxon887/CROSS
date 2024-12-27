#ifdef DIRECT_LIGHT
struct Light {
	vec3 direction;
	vec4 color;
};
#endif

#ifdef POINT_LIGHT
struct Light {
	vec3 position;
	vec4 color;
	
	float intensity;
};
#endif

#ifdef SPOT_LIGHT
struct Light {
	vec3 direction;
	vec3 position;
	vec4 color;
	
	float intensity;
	
	float cut_off;
	float outer_cut_off;
};
#endif

uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularMap;
uniform float uShininess;

uniform Light uLight;
uniform vec4 uAmbientLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
#ifdef DIRECT_LIGHT
	vec3 lightDirection = normalize(-uLight.direction);
#endif
#ifdef POINT_OR_SPOT_LIGHT
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
#endif

	vec4 texel = texture2D(uDiffuseTexture, vTexCoords);
	//ambient
	vec4 ambient = uAmbientLight * texel;
	//diffuse
	vec3 normal = normalize(vNormal);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec4 diffuse = uLight.color * diffEffect * texel;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = uLight.color * specEffect * texture2D(uSpecularMap, vTexCoords);

#ifdef POINT_OR_SPOT_LIGHT
	//attenaution
	float dist = length(uLight.position - vFragPosition);
	float attenaution = 1.0 / (1.0 + uLight.intensity * dist + uLight.intensity * dist * dist);
	ambient *= attenaution;
	diffuse *= attenaution;
	specular *= attenaution;
#endif
#ifdef SPOT_LIGHT
	float theta = dot(lightDirection, normalize(-uLight.direction));
	float epsilon = uLight.cut_off - uLight.outer_cut_off;
	float intensity = clamp((theta - uLight.outer_cut_off) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;
#endif

	gl_FragColor = ambient + diffuse + specular;
} 
