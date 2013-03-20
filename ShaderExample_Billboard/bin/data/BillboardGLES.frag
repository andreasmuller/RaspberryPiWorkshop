precision highp float;

uniform sampler2D src_tex_unit0;
uniform float useTexture;
uniform float useColors;
uniform vec4 color;

varying float depth;
varying vec4 colorVarying;
varying vec2 texCoordVarying;
uniform sampler2D tex0;
		
void main(){
	vec4 c;
	if(useColors>0.5){
		c = colorVarying;
	}else{
		c = color;
	}
	gl_FragColor = texture2D(src_tex_unit0, gl_PointCoord)*c;	
}