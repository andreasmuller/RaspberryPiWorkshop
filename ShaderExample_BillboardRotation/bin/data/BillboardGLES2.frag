precision highp float;

uniform sampler2D src_tex_unit0;
uniform float useTexture;
uniform float useColors;
uniform vec4 color;

varying float depth;
varying vec4 colorVarying;
varying vec2 texCoordVarying;

varying float rotAngle;

void main (void) {
	
	vec4 c;
	if(useColors>0.5){
		c = colorVarying;
	}else{
		c = color;
	}
	
	float angle		= rotAngle;
	vec2 offset		= vec2(.5, .5);
	vec2 v2 = gl_PointCoord;	
	
	// not sure this is right
	// I just search on how to rotate
	// a texture...
	float cosX, sinX;
	float cosY, sinY;
	sinX = sin(angle);
	cosX = cos(angle);
	sinY = sin(angle);
	cosY = cos(angle);
	
	mat2 rotationMatrix = mat2( cosX, -sinX,
							    sinY, cosX);
	
	vec2 newcoords = ((v2-offset) * (rotationMatrix));
	newcoords += offset;
	
	gl_FragColor = texture2D(src_tex_unit0, gl_PointCoord)*c;
}