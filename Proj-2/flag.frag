uniform sampler2D baseTexture;

varying vec4 coord;
 
const float shaderColor = 0.7;

void main(){
  
	gl_FragColor = texture2D(baseTexture, gl_TexCoord[0].st)+ coord.y*(shaderColor,shaderColor,shaderColor,1.0);	
	
}