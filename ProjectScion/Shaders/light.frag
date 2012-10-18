uniform sampler2D texture;

void main()
{
    gl_FragColor = 0.75*texture2D(texture, gl_TexCoord[0].xy);
}