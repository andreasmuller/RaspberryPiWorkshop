precision highp float;

uniform sampler2D src_tex_unit0;
uniform float useTexture;
uniform float useColors;
uniform vec4 color;

varying float depth;
varying vec4 colorVarying;
varying vec2 texCoordVarying;

uniform sampler2D tex0;
uniform sampler2D tex1;	
uniform vec2 resolution;
uniform float time;

// http://www.iquilezles.org/apps/shadertoy/?p=Multitexture
void main(){
	//gl_FragColor = texture2D(tex0, texCoordVarying)*0.9;
	
	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
    // a rotozoom
    vec2 cst = vec2( cos(.5*time), sin(.5*time) );
    mat2 rot = 0.5*cst.x*mat2(cst.x,-cst.y,cst.y,cst.x);
    vec3 col1 = texture2D(tex0,rot*p).xyz;

    // scroll
    vec3 col2 = texture2D(tex1, 0.5*p+sin(0.1*time)).xyz;

    // blend layers
    vec3 col = col2*col1;

    gl_FragColor = vec4(col,1.0);
    
    //gl_FragColor = texture2D(tex0, texCoordVarying)*1.0;
}

