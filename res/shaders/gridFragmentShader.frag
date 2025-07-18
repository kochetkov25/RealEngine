#version 440

in  vec3 pos;

out vec4 fragmentColor;


vec4 grid(vec3 pos, float scale, float div, vec3 col) 
{
    /*scale coords*/
    vec2 coord = pos.xz * scale * 1000;
    vec2 derivative = fwidth(coord);
    /*positioning in 0 - 1 coords*/
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    /*change alpha channel in dependence on distance*/
    vec4 color = vec4(col, 1.0 - min(line, 1.0));

    if (abs(coord.x) < derivative.x) /*x axis*/
    {
        color.r = 1.0;
    }else if(abs(coord.y) < derivative.y) /*z axis*/
    {
        color.b = 1.0;
    }else /*other*/
    {
        color.rgb = vec3(0.7529, 0.7529, 0.7529);
    }
    color.a *= div;
    return color;
}


void main()
{
	fragmentColor.rgba  = (grid(pos, 1,  0.8, vec3(0.6, 0.6, 0.6))).rgba;
    fragmentColor.rgba += (grid(pos, 5,  0.4, vec3(0.005, 0.005, 0.005))).rgba;
}