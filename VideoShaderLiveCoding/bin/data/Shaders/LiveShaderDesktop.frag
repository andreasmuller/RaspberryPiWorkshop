#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect 	tex_unit0;
uniform float 			time;

void main( void )
{
	//float tmpCos = (cos(u_time*2.0)+1.0)*0.5;
    //gl_FragColor = vec4( tmpCos, 1.0-tmpCos, 1.0, 1.0);

	//gl_FragColor = texture2DRect( tex_unit0, gl_TexCoord[0].st );

	float newTime = time * 2.0;

	vec2 newTexCoord;
	newTexCoord.s = gl_TexCoord[0].s + (cos(newTime + (gl_TexCoord[0].s*0.01)) * 10.0);
	newTexCoord.t = gl_TexCoord[0].t + (sin(newTime + (gl_TexCoord[0].t*0.01)) * 10.0);

	vec2 texCoordRed 	= newTexCoord;
	vec2 texCoordGreen 	= newTexCoord;
	vec2 texCoordBlue 	= newTexCoord;

	texCoordRed 	+= vec2( cos((newTime * 2.76)), sin((newTime * 2.12)) ) * 10.0;// * sin(u_time);
	texCoordGreen 	+= vec2( cos((newTime * 2.23)), sin((newTime * 2.40)) ) * 10.0;// * sin(u_time);
	texCoordBlue 	+= vec2( cos((newTime * 2.98)), sin((newTime * 2.82)) ) * 10.0;// * sin(u_time);	

	float colorR = texture2DRect( tex_unit0, texCoordRed ).r;
	float colorG = texture2DRect( tex_unit0, texCoordGreen).g;
	float colorB = texture2DRect( tex_unit0, texCoordBlue).b;		

	gl_FragColor = vec4( colorR, colorG, colorB, 1.0 );
}