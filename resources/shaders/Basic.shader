#shader vertex
#version 330 core

// take in data from vertex buffer
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

// output data to fragment shader (or next stage in pipeline)
out vec2 v_TexCoord;

// take in data from cpu
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

// output data to frame buffer
layout(location = 0) out vec4 color;

// take in data from vertex shader
in vec2 v_TexCoord;

// takes in data from the cpu
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
};