#version 120
#extension GL_ARB_texture_rectangle : enable

uniform float time;

void main( void )
{
	float tmpCos = (cos(time*2.0)+1.0)*0.5;
    gl_FragColor = vec4( tmpCos, 1.0-tmpCos, 1.0, 1.0);
}