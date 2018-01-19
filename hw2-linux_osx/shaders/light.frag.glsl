# version 330 core
// Do not use any version older than 330!

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Inputs to the fragment shader are the outputs of the same name of the vertex shader.
// Note that the default output, gl_Position, is inaccessible!
in vec3 mynormal; 
in vec4 myvertex; 

// You will certainly need this matrix for your lighting calculations
uniform mat4 modelview;

// This first defined output of type vec4 will be the fragment color
out vec4 fragColor;

uniform vec3 color;

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.
// I use ambient, diffuse, specular, shininess. 
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec4 ComputeLight( 
	const in vec3 direction, 
	const in vec4 lightcolor, 
	const in vec3 normal, 
	const in vec3 halfvec, 
	const in vec4 mydiffuse, 
	const in vec4 myspecular, 
	const in float myshininess ){

		float nDotL = dot(normal, direction);
		vec4 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

        float nDotH = dot( normal, halfvec );
		vec4 phong = myspecular * lightcolor * pow(max(nDotH, 0.0), myshininess);

		vec4 retval = lambert + phong;
		return retval;

}

void main (void) 
{       
    if (enablelighting) {
        vec4 sigmaLighting = vec4(1,0,0,1);

        sigmaLighting = ambient + emission;

        // They eye is always at (0,0,0) looking down -z axis 
        // Also compute current fragment position and direction to eye 
        const vec3 eyepos = vec3(0,0,0) ; 
        vec4 newPosition = modelview * myvertex ; 
        vec3 mypos = newPosition.xyz / newPosition.w ;
        vec3 eyedirn = normalize(eyepos - mypos) ; 

        // Compute normal, needed for shading. 
        // This was the most difficult portion of the lighting computation for me
        // get inverse transpose of the modelview
        mat3 inverseTransposeModelview = mat3(transpose(inverse( modelview ) ));
        
        // multiply it by the current normal and then normalize to a unit vector
        vec3 normal = normalize( inverseTransposeModelview * mynormal );

        for (int i = 0; i < numused; i++) {

            vec3 direction;
            vec3 halfVec;

            if (lightposn[i].w == 0) {
                // directional light
                direction = normalize ( lightposn[i].xyz) ;                 
            } else {
                // point light
                vec3 position = lightposn[i].xyz / lightposn[i].w; 
                direction =  normalize ( position - mypos );
            }
            // for both lights we need a half vector and to compute the light
            halfVec = normalize (direction + eyedirn) ;  
            vec4 col = ComputeLight(direction, lightcolor[i], normal, halfVec, diffuse, specular, shininess);

            // add the lights together
            sigmaLighting += col;
        }
        // return the final combination of lights
        fragColor = sigmaLighting;
    } else {
        fragColor = vec4(color, 1.0f);
    }
}