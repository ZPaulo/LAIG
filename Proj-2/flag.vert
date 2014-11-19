uniform float timer;

varying vec4 coord;
 
const float PI = 3.14159265;

void main() {
 	
	gl_TexCoord[0] = gl_MultiTexCoord0;

    float angle = gl_MultiTexCoord0.s*3;
    vec4 newC = vec4(gl_Vertex.x, gl_Vertex.y + sin((angle+timer*3)*PI)*0.06, gl_Vertex.z, 1.0);

    gl_Position = gl_ModelViewProjectionMatrix * newC;

	
gl_FrontColor = vec4(0,0,0,0);
 	coord = newC;
}