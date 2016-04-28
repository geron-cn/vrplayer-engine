
///////////////////////////////////////////////////////////
// Atributes
attribute vec4 a_position;

attribute vec2 a_texCoord;


///////////////////////////////////////////////////////////
// Uniforms
uniform mat4 u_worldViewProjectionMatrix;


///////////////////////////////////////////////////////////
// Varyings
varying vec2 v_texCoord;


void main()
{
    gl_Position = u_worldViewProjectionMatrix * a_position;

    v_texCoord = 1.0 - a_texCoord;
}
