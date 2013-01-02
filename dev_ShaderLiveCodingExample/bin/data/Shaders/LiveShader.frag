precision highp float;

uniform sampler2D src_tex_unit0;

uniform vec4 color;

varying float depth;
varying vec4 colorVarying;
varying vec2 texCoordVarying;

uniform float time;
		
void main()
{
	//lowp float tmpCos = (cos(time*2.0)+1.0)*0.5;
    //gl_FragColor = vec4( tmpCos, 1.0-tmpCos, 1.0, 1.0);

	mediump float newTime = time * 2.0;

	vec2 newTexCoord;
	newTexCoord.s = texCoordVarying.s + (cos(newTime + (texCoordVarying.s*20.0)) * 0.01);
	newTexCoord.t = texCoordVarying.t + (sin(newTime + (texCoordVarying.t*20.0)) * 0.01);

	mediump vec2 texCoordRed 	= newTexCoord;
	mediump vec2 texCoordGreen 	= newTexCoord;
	mediump vec2 texCoordBlue 	= newTexCoord;

	texCoordRed 	+= vec2( cos((newTime * 2.76)), sin((newTime * 2.12)) )* 0.01;
	texCoordGreen 	+= vec2( cos((newTime * 2.23)), sin((newTime * 2.40)) )* 0.01;
	texCoordBlue 	+= vec2( cos((newTime * 2.98)), sin((newTime * 2.82)) )* 0.01;	

	mediump float colorR = texture2D( src_tex_unit0, texCoordRed ).r;
	mediump float colorG = texture2D( src_tex_unit0, texCoordGreen).g;
	mediump float colorB = texture2D( src_tex_unit0, texCoordBlue).b;		

	mediump vec4 outColor = vec4( colorR, colorG, colorB, 1.0 );

	gl_FragColor = outColor;

	//gl_FragColor = texture2D(src_tex_unit0, newTexCoord);
    //gl_FragColor = vec4( 1.0, 0.0, 0.1, 1.0);
}