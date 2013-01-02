precision highp float;

uniform vec4 color;

varying float depth;
varying vec4 colorVarying;
varying vec2 texCoordVarying;

uniform float time;
		
//	GL ES needs a precision specifier in front of each variable created? 

void main()
{
	lowp float tmpCos = (cos(time*2.0)+1.0)*0.5;
    gl_FragColor = vec4( tmpCos, 1.0-tmpCos, 1.0, 1.0);

    //gl_FragColor = vec4( 1.0, 0.0, 0.1, 1.0);
}