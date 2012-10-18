uniform vec4 color;

void main()
{
    gl_FrontColor = color;
    gl_Position = ftransform();
}