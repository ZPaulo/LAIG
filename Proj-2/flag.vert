uniform float timer;
uniform float windV;

varying vec4 coord;
 
const float PI = 3.14159265;

void main() {
 	
    gl_TexCoord[0] = gl_MultiTexCoord0;

    float angle = gl_MultiTexCoord0.s*windV;
    vec4 newC = vec4(gl_Vertex.x, gl_Vertex.y + sin((angle+timer*windV)*PI)*0.06, gl_Vertex.z, 1.0);

    gl_Position = gl_ModelViewProjectionMatrix * newC;

 	coord = newC;
}