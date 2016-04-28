#ifdef OPENGL_ES
precision mediump float;
#endif


///////////////////////////////////////////////////////////
// Uniforms

uniform sampler2D u_diffuseTexture;

///////////////////////////////////////////////////////////
// Varyings
varying vec2 v_texCoord;

void main()
{
 
    gl_FragColor.rgb = texture2D(u_diffuseTexture, v_texCoord).rgb;
 
    gl_FragColor.a = 1.0;
}
